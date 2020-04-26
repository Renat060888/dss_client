
#include <thread>

#include "from_ms_common/communication/amqp_client_c.h"
#include "from_ms_common/communication/amqp_controller.h"
#include "from_ms_common/common/ms_common_utils.h"
#include "from_ms_common/system/logger.h"

#include "dss_client.h"
#include "common_vars.h"
#include "common_types_private.h"
#include "player_handler.h"
#include "commands/dss_core/cmd_core_ping.h"
#include "commands/player/cmd_player_ping.h"
#include "commands/player/cmd_player_register.h"
#include "commands/player/cmd_player_context_open.h"
#include "commands/player/cmd_player_context_close.h"

#include "fakeobjrepr.h"

namespace dss_client {

using namespace std;
using namespace common_types;

static constexpr int64_t PING_INTERVAL_MILLISEC = 1000;
static constexpr int64_t PONG_TIMEOUT_MILLISEC = 5000;

static DssClient * g_instance = nullptr;
static int32_t g_instanceRefCounter = 0;

// client(N) <-> core(1) <-> node(N) (agent, controller) <-> player(N) (agent, controller)

// --------------------------------------------------------------------
// private
// --------------------------------------------------------------------
class PrivateImplementationDC : public IClientControl {
public:
    PrivateImplementationDC()
        : interface(nullptr)
        , threadClientMaintenance(nullptr)
        , shutdownCalled(false)
        , clientUniqueIdForDSS(common_vars::INVALID_CLIENT_ID)
        , clientUniqueIdForPlayer(common_vars::INVALID_CLIENT_ID)
        , registrationOnDSSInProgress(false)
        , registrationOnPlayerInProgress(false)
        , lastPlayerPongMillisec(0)
        , onlinePlayer(false)
    {
        state = std::make_shared<DssClient::SState>();
    }

    ~PrivateImplementationDC(){


    }

    // ------------------------------------------
    // public data section
    // ------------------------------------------
    DssClient::PState state;

    // ------------------------------------------
    // private data section
    // ------------------------------------------

    // data
    bool shutdownCalled;
    PPlayerHandler playerHandler;

    SCommandServices commandServices;
    std::vector<PCommand> commandsInProgress;

    TDssClientUniqueId clientUniqueIdForDSS;
    TPlayerClientUniqueId clientUniqueIdForPlayer;
    bool registrationOnDSSInProgress;
    bool registrationOnPlayerInProgress;

    PCommandDSSPing commandPingToDss;
    PCommandPlayerPing commandPingToPlayer;
    int64_t lastPlayerPongMillisec;
    int64_t lastDSSPongMillisec;
    bool onlinePlayer;
    bool onlineDSS;

    // service
    DssClient * interface;
    std::thread * threadClientMaintenance;
    PNetworkProvider networkProvider;
    PNetworkClient networkClient; // TODO: highly likely is a not needed

    void threadMaintenance(){
        while( ! shutdownCalled ){

            networkProvider->runNetworkCallbacks();
            commandsProcessing();
            remoteServicesMonitoring();
            ping();

            std::this_thread::sleep_for( std::chrono::milliseconds(10) );
        }
    }

    inline void commandsProcessing(){
        for( auto iter = commandsInProgress.begin(); iter != commandsInProgress.end(); ){
            PCommand cmd = ( * iter );

            if( cmd->isReady() ){
                iter = commandsInProgress.erase( iter );
            }
            else{
                ++iter;
            }
        }
    }

    inline void remoteServicesMonitoring(){
        if( onlinePlayer ){
            if( (common_utils::getCurrentTimeMillisec() - lastPlayerPongMillisec) > PONG_TIMEOUT_MILLISEC ){
                VS_LOG_INFO << PRINT_HEADER << " Player OFFLINE at " << common_utils::getCurrentDateTimeStr() << endl;
                onlinePlayer = false;

                // set player handler in UNAVAILABLE state

                interface->m_signalPlayerOnline( false );
            }
        }
        else{
            if( (common_utils::getCurrentTimeMillisec() - lastPlayerPongMillisec) < PONG_TIMEOUT_MILLISEC ){
                VS_LOG_INFO << PRINT_HEADER << " Player ONLINE at " << common_utils::getCurrentDateTimeStr() << endl;
                onlinePlayer = true;
                interface->m_signalPlayerOnline( true );
            }
        }

        if( onlineDSS ){
            if( (common_utils::getCurrentTimeMillisec() - lastDSSPongMillisec) > PONG_TIMEOUT_MILLISEC ){
                VS_LOG_INFO << PRINT_HEADER << " DSS OFFLINE" << endl;
                onlineDSS = false;
                interface->m_signalDSSOnline( false );
            }
        }
        else{
            if( (common_utils::getCurrentTimeMillisec() - lastDSSPongMillisec) < PONG_TIMEOUT_MILLISEC ){
                VS_LOG_INFO << PRINT_HEADER << " DSS ONLINE" << endl;
                onlineDSS = true;
                interface->m_signalDSSOnline( true );
            }
        }
    }

    inline void ping(){
        static int64_t lastPingAtMillisec = 0;
        if( (common_utils::getCurrentTimeMillisec() - lastPingAtMillisec) > PING_INTERVAL_MILLISEC ){
            if( commandPingToPlayer->isReady() ){
                commandPingToPlayer->execAsync();
            }

            // TODO: ping a DSS

            lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
        }
    }

    // DSS
    virtual void pongByDSSCatched() override {
        lastDSSPongMillisec = common_utils::getCurrentTimeMillisec();
    }

    virtual void registerInDSS() override {
        if( (clientUniqueIdForDSS != common_vars::INVALID_CLIENT_ID) || registrationOnDSSInProgress ){
            return;
        }

        registrationOnDSSInProgress = true;
        PCommandPlayerRegister cmd = std::make_shared<CommandPlayerRegister>( & commandServices, commandServices.networkClientForDss );
        cmd->m_userPid = ::getpid();
        cmd->m_userIpStr = common_utils::getIpAddressStr();
        cmd->execAsync();
        commandsInProgress.push_back( cmd );
    }

    virtual void setIdFromDSS( const common_types::TDssClientUniqueId & _id ) override {
        clientUniqueIdForDSS = _id;
        commandPingToDss->m_userIdToDSS = _id;
        registrationOnDSSInProgress = false;

        VS_LOG_INFO << PRINT_HEADER << " registered in DSS with id [" << _id << "]" << endl;
    }

    virtual void updateNodes( const std::vector<void *> & _nodeStates ) override {


    }

    // Player
    virtual void pongByPlayerCatched() override {
        lastPlayerPongMillisec = common_utils::getCurrentTimeMillisec();
    }

    virtual void registerInPlayer() override {
        if( (clientUniqueIdForPlayer != common_vars::INVALID_CLIENT_ID) || registrationOnPlayerInProgress ){
            return;
        }

        registrationOnPlayerInProgress = true;
        PCommandPlayerRegister cmd = std::make_shared<CommandPlayerRegister>( & commandServices, commandServices.networkClientForPlayer );
        cmd->m_userPid = ::getpid();
        cmd->m_userIpStr = common_utils::getIpAddressStr();
        cmd->execAsync();
        commandsInProgress.push_back( cmd );
    }

    virtual void setIdFromPlayer( const TPlayerClientUniqueId & _id ) override {
        clientUniqueIdForPlayer = _id;
        commandPingToPlayer->m_userIdToPlayer = _id;
        registrationOnPlayerInProgress = false;

        VS_LOG_INFO << PRINT_HEADER << " registered in Player with id [" << _id << "]" << endl;
    }

    virtual void updatePlayer( bool _stateHasCome, const SPlayingServiceState & _state ) override {

        if( _stateHasCome ){
            if( playerHandler ){
                playerHandler->updateState( _state );
            }
            else{
                playerHandler = std::make_shared<PlayerHandler>();
                playerHandler->updateState( _state );
                interface->m_signalPlayerOnline( true );
            }
        }
        else{
            if( playerHandler ){
                interface->m_signalPlayerOnline( false );
                playerHandler.reset();
            }
        }
    }

    void callbackContextOpened( ::common_types::TContextId _ctxId ){
        cout << "OPEN CTX: " << _ctxId << endl;

        PCommandPlayerContextOpen cmd = std::make_shared<CommandPlayerContextOpen>( & commandServices, commandServices.networkClientForPlayer );
        cmd->m_userId = clientUniqueIdForPlayer;
        cmd->m_contextName = "bla";

        if( ! cmd->execAsync() ){
            VS_LOG_INFO << PRINT_HEADER << " cmd failed, reason [" << cmd->getLastError() << "]" << endl;
        }

        commandsInProgress.push_back( cmd );
    }

    void callbackContextClosed(){
        cout << "CLOSE CTX" << endl;

        PCommandPlayerContextClose cmd = std::make_shared<CommandPlayerContextClose>( & commandServices, commandServices.networkClientForPlayer );
        cmd->m_userId = clientUniqueIdForPlayer;

        if( ! cmd->execAsync() ){
            VS_LOG_INFO << PRINT_HEADER << " cmd failed, reason [" << cmd->getLastError() << "]" << endl;
        }

        commandsInProgress.push_back( cmd );
    }

    PNetworkClient createDssConnection( const PNetworkClient & _networkClient ){
        const string clientName = "ip" + common_utils::getIpAddressStr() + "_" + "pid" + std::to_string( ::getpid() );        

        // client -> core
        SAmqpRouteParameters routes;
        routes.predatorExchangePointName = "dss_dx_clients";
        routes.predatorQueueName = "dss_q_client_mailbox_" + clientName;
        routes.predatorRoutingKeyName = "dss_rk_to_client_" + clientName;
        routes.targetExchangePointName = "dss_dx_core";
        routes.targetQueueName = "dss_q_core_mailbox";
        routes.targetRoutingKeyName = "dss_rk_to_core";

        AmqpController::SInitSettings settings2;
        settings2.client = _networkClient;
        settings2.route = routes;

        PAmqpController controller = std::make_shared<AmqpController>( INetworkEntity::INVALID_CONN_ID );
        const bool rt = controller->init( settings2 );
        if( ! rt ){
            state->lastError = controller->getState().lastError;
            return nullptr;
        }

        return controller;
    }

    PNetworkClient createPlayerConnection( const PNetworkClient & _networkClient ){
        const string clientName = "ip" + common_utils::getIpAddressStr() + "_" + "pid" + std::to_string( ::getpid() );

        // client -> player
        SAmqpRouteParameters routes;
        routes.predatorExchangePointName = "dss_dx_clients";
        routes.predatorQueueName = "dss_q_client_mailbox_" + clientName;
        routes.predatorRoutingKeyName = "dss_rk_to_client_" + clientName;
        routes.targetExchangePointName = "player_dx_agent";
        routes.targetQueueName = "player_q_agent_mailbox";
        routes.targetRoutingKeyName = "player_rk_to_agent";

        AmqpController::SInitSettings settings2;
        settings2.client = _networkClient;
        settings2.route = routes;

        PAmqpController controller = std::make_shared<AmqpController>( INetworkEntity::INVALID_CONN_ID );
        const bool rt = controller->init( settings2 );
        if( ! rt ){
            state->lastError = controller->getState().lastError;
            return nullptr;
        }

        return controller;
    }

    PNetworkClient createNetworkClient( const DssClient::SInitSettings & _settings ){
        PAmqpClient client = std::make_shared<AmqpClient>( INetworkEntity::INVALID_CONN_ID );

        AmqpClient::SInitSettings clientSettings;
        clientSettings.serverHost = _settings.amqpBrokerHost;
        clientSettings.amqpVirtualHost = _settings.amqpBrokerVirtualHost;
        clientSettings.port = _settings.amqpBrokerPort;
        clientSettings.login = _settings.amqpLogin;
        clientSettings.pass = _settings.amqpPass;
        clientSettings.deliveredMessageExpirationSec = 10;

        if( ! client->init(clientSettings) ){
            state->lastError = client->getState().m_lastError;
            return nullptr;
        }

        return client;
    }
};

// --------------------------------------------------------------------
// public
// --------------------------------------------------------------------
DssClient::DssClient()
    : m_impl(new PrivateImplementationDC())
{
    m_impl->interface = this;
}

DssClient::~DssClient(){

    m_impl->shutdownCalled = true;

    common_utils::threadShutdown( m_impl->threadClientMaintenance );

    delete m_impl;
    m_impl = nullptr;
}

DssClient * DssClient::getInstance(){

    if( g_instance ){
        g_instanceRefCounter++;
        return g_instance;
    }
    else{
        g_instance = new DssClient();
        g_instanceRefCounter++;

        VS_LOG_INFO << PRINT_HEADER << " instance created" << endl;

        return g_instance;
    }
}

void DssClient::releaseInstance( DssClient * & _instance ){

    if( ! _instance ){
        return;
    }

    assert( g_instanceRefCounter > 0 );

    g_instanceRefCounter--;
    _instance = nullptr;

    if( 0 == g_instanceRefCounter ){
        delete g_instance;
        g_instance = nullptr;

        VS_LOG_INFO << PRINT_HEADER << " instance destroyed" << endl;
    }
}

bool DssClient::init( const SInitSettings & _settings ){

    m_impl->state->settings = _settings;

    // subscribe to objrepr-context events
    FakeObjrepr::singleton().m_signalCtxOpened.connect( boost::bind( & PrivateImplementationDC::callbackContextOpened, m_impl, boost::placeholders::_1) );
    FakeObjrepr::singleton().m_signalCtxClosed.connect( boost::bind( & PrivateImplementationDC::callbackContextClosed, m_impl ) );

    // network
    m_impl->networkClient = m_impl->createNetworkClient( _settings );
    m_impl->commandServices.clientController = m_impl;
    m_impl->commandServices.networkClientForDss = m_impl->createDssConnection( m_impl->networkClient );
    m_impl->commandServices.networkClientForPlayer = m_impl->createPlayerConnection( m_impl->networkClient );
    if( ! m_impl->commandServices.networkClientForDss || ! m_impl->commandServices.networkClientForPlayer ){
        VS_LOG_ERROR << PRINT_HEADER << " network connection failed, reason [" << m_impl->state->lastError << "]" << endl;
        return false;
    }
    m_impl->networkProvider = std::dynamic_pointer_cast<INetworkProvider>( m_impl->commandServices.networkClientForDss );

    // ping
    m_impl->commandPingToDss = std::make_shared<CommandDSSPing>( & m_impl->commandServices, m_impl->commandServices.networkClientForDss );
    m_impl->commandPingToPlayer = std::make_shared<CommandPlayerPing>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );

    // async processing
    m_impl->threadClientMaintenance = new std::thread( & PrivateImplementationDC::threadMaintenance, m_impl );

    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

PPlayerHandler DssClient::getPlayerHandler(){

    return m_impl->playerHandler;
}

PHandlerNodeSimula DssClient::getNodeSimulaHandler( common_types::TNodeId _id ){


}

std::vector<PHandlerNodeSimula> DssClient::getNodeSimulaHandlers(){

}

PHandlerNodeReal DssClient::getNodeRealHandler( common_types::TNodeId _id ){


}

std::vector<PHandlerNodeReal> DssClient::getNodeRealHandlers(){

}

const DssClient::PConstState DssClient::getState(){
    return m_impl->state;
}

}






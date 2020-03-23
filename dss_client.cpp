
#include <thread>

#include <microservice_common/communication/amqp_client_c.h>
#include <microservice_common/communication/amqp_controller.h>
#include <microservice_common/common/ms_common_utils.h>
#include <microservice_common/system/logger.h>

#include "dss_client.h"
#include "common_vars.h"
#include "common_types_private.h"
#include "commands/cmd_ping.h"
#include "commands/cmd_register.h"
#include "commands/cmd_context_open.h"
#include "commands/cmd_context_close.h"

namespace dss_client {

using namespace std;

static DssClient * g_instance = nullptr;
static int32_t g_instanceRefCounter = 0;

// client(N) <-> core(1) <-> node(N) (agent, controller) <-> player(N) (agent, controller)

// --------------------------------------------------------------------
// private
// --------------------------------------------------------------------
class PrivateImplementation : public common_types::IClientControl {
public:
    PrivateImplementation()
        : interface(nullptr)
        , threadClientMaintenance(nullptr)
        , shutdownCalled(false)
        , clientUniqueIdForDSS(common_vars::INVALID_CLIENT_ID)
        , clientUniqueIdForPlayer(common_vars::INVALID_CLIENT_ID)
        , registraionOnDSSInProgress(false)
        , registraionOnPlayerInProgress(false)
        , lastPongMillisec(0)
        , online(false)
    {
        state = std::make_shared<DssClient::SState>();
    }

    ~PrivateImplementation(){


    }

    // ------------------------------------------
    // public data section
    // ------------------------------------------
    DssClient::PState state;

    // ------------------------------------------
    // private data section
    // ------------------------------------------

    // data
    DssClient * interface;
    std::thread * threadClientMaintenance;
    PNetworkProvider networkProvider;
    bool shutdownCalled;

    common_types::SCommandServices commandServices;
    std::vector<PCommand> commandsInProgress;

    common_types::TDssClientUniqueId clientUniqueIdForDSS;
    common_types::TPlayerClientUniqueId clientUniqueIdForPlayer;
    bool registraionOnDSSInProgress;
    bool registraionOnPlayerInProgress;

    PCommandPing commandPingToDss;
    PCommandPing commandPingToPlayer;
    int64_t lastPongMillisec;
    bool online;

    // service
    PNetworkClient networkClient;

    void threadMaintenance(){

        while( ! shutdownCalled ){
            // "rotate" network engine
            networkProvider->runNetworkCallbacks();

            // clean executed command
            for( auto iter = commandsInProgress.begin(); iter != commandsInProgress.end(); ){
                PCommand cmd = ( * iter );

                if( cmd->isReady() ){
                    iter = commandsInProgress.erase( iter );
                }
                else{
                    ++iter;
                }
            }

            // online checking
            if( online ){
                if( (common_utils::getCurrentTimeMillisec() - lastPongMillisec) > 5000 ){
                    VS_LOG_INFO << PRINT_HEADER << " Player OFFLINE" << endl;
                    online = false;
                    interface->m_signalOnline( false );
                }
            }
            else{
                if( (common_utils::getCurrentTimeMillisec() - lastPongMillisec) < 5000 ){
                    VS_LOG_INFO << PRINT_HEADER << " Player ONLINE" << endl;
                    online = true;
                    interface->m_signalOnline( true );
                }
            }

            // ping
            static int64_t lastPingAtMillisec = 0;
            if( (common_utils::getCurrentTimeMillisec() - lastPingAtMillisec) > 1000 ){
                if( commandPingToPlayer->isReady() ){
                    commandPingToPlayer->execAsync();
                }
                lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
            }

            std::this_thread::sleep_for( std::chrono::milliseconds(10) );
        }
    }

    virtual void pongCatched() override {
        lastPongMillisec = common_utils::getCurrentTimeMillisec();
    }

    // DSS
    virtual bool isRegisteredInDSS() override {

        bool registerInProgress = true;
        bool idReceived = true;

        return ( ! idReceived && ! registerInProgress );
    }

    virtual void registerInDSS() override {


    }

    virtual void updateNodes( const std::vector<void *> & _nodeStates ) override {


    }

    // Player
    virtual bool isRegisteredInPlayer() override {

        if( clientUniqueIdForPlayer != common_vars::INVALID_CLIENT_ID ){
            return true;
        }
        else{
            return registraionOnPlayerInProgress;
        }
    }

    virtual void registerInPlayer() override {

        registraionOnPlayerInProgress = true;
        PCommandRegister cmd = std::make_shared<CommandRegister>( & commandServices, commandServices.networkClientForPlayer );
        cmd->m_userPid = ::getpid();
        cmd->m_userIpStr = common_utils::getIpAddressStr();
        cmd->execAsync();
        commandsInProgress.push_back( cmd );
    }

    virtual void setIdFromPlayer( const common_types::TDssClientUniqueId & _id ) override {

        clientUniqueIdForPlayer = _id;
        commandPingToPlayer->m_userIdToPlayer = _id;
        registraionOnPlayerInProgress = false;

        VS_LOG_INFO << PRINT_HEADER << " registered in Player with id [" << _id << "]" << endl;
    }

    virtual void updatePlayer( const void * & _playerState ) override {


        // TODO: do
    }

    void callbackContextOpened( ::common_types::TContextId _ctxId ){

        PCommandContextOpen cmd = std::make_shared<CommandContextOpen>( & commandServices, commandServices.networkClientForPlayer );
        cmd->m_clientId = clientUniqueIdForPlayer;
        cmd->m_contextName = "bla";

        if( ! cmd->execAsync() ){
            VS_LOG_INFO << PRINT_HEADER << " cmd failed, reason [" << cmd->getLastError() << "]" << endl;
        }

        commandsInProgress.push_back( cmd );
    }

    void callbackContextClosed(){

        PCommandContextClose cmd = std::make_shared<CommandContextClose>( & commandServices, commandServices.networkClientForPlayer );
        cmd->m_clientId = clientUniqueIdForPlayer;

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
        routes.targetExchangePointName = "dss_dx_player";
        routes.targetQueueName = "dss_q_player_mailbox";
        routes.targetRoutingKeyName = "dss_rk_to_player";

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
        clientSettings.deliveredMessageExpirationSec = 60;

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
    : m_impl(new PrivateImplementation())
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
    m_impl->commandPingToDss = std::make_shared<CommandPing>( & m_impl->commandServices, m_impl->commandServices.networkClientForDss );
    m_impl->commandPingToPlayer = std::make_shared<CommandPing>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    m_impl->commandPingToPlayer->m_toDSS = false;

    // async processing
    m_impl->threadClientMaintenance = new std::thread( & PrivateImplementation::threadMaintenance, m_impl );

    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

const DssClient::PConstState DssClient::getState(){
    return m_impl->state;
}

}


#include <thread>

#include <microservice_common/communication/amqp_client_c.h>
#include <microservice_common/communication/i_amqp_controller.h>
#include <microservice_common/common/ms_common_utils.h>
#include <microservice_common/system/logger.h>

#include "dss_client.h"
#include "common_vars.h"
#include "common_types_private.h"
#include "commands/cmd_ping.h"
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
class PrivateImplementation {
public:
    PrivateImplementation()
        : interface(nullptr)
        , threadClientMaintenance(nullptr)
        , clientUniqueId(common_vars::INVALID_CLIENT_ID)
        , shutdownCalled(false)
    {
        state = std::make_shared<DssClient::SState>();
    }

    ~PrivateImplementation(){


    }

    // public data section
    DssClient::PState state;

    // private data section
    DssClient * interface;
    std::thread * threadClientMaintenance;
    common_types::SCommandServices commandServices;
    common_types::TClientUniqueId clientUniqueId;
    bool shutdownCalled;
    PCommandPing commandPing;

    std::vector<PCommand> commandsInProgress;

    // service
    void threadMaintenance(){

        while( ! shutdownCalled ){

            // clean out-dated commands
            for( auto iter = commandsInProgress.begin(); iter != commandsInProgress.end(); ){
                PCommand cmd = ( * iter );

                if( cmd->isReady() ){
                    iter = commandsInProgress.erase( iter );
                }
                else{
                    ++iter;
                }
            }

//            const bool rt = commandPing->execAsync();
//            const bool rt2 = commandPing->isReady();




            std::this_thread::sleep_for( std::chrono::milliseconds(10) );
        }
    }

    void callbackContextOpened( ::common_types::TContextId _ctxId ){

        PCommandContextOpen command = std::make_shared<CommandContextOpen>( & commandServices );
        command->m_clientId = clientUniqueId;
        command->m_contextName = "bla";

        command->execAsync();

        commandsInProgress.push_back( command );
    }

    void callbackContextClosed(){

        PCommandContextClose command = std::make_shared<CommandContextClose>( & commandServices );
        command->m_clientId = clientUniqueId;

        command->execAsync();

        commandsInProgress.push_back( command );
    }

    PNetworkClient createNetworkConnection( const DssClient::SInitSettings & _settings ){

        //
        PAmqpClient client = std::make_shared<AmqpClient>( INetworkEntity::INVALID_CONN_ID );

        AmqpClient::SInitSettings clientSettings;
        clientSettings.serverHost = _settings.amqpBrokerHost;
        clientSettings.amqpVirtualHost = _settings.amqpBrokerVirtualHost;
        clientSettings.port = _settings.amqpBrokerPort;
        clientSettings.login = _settings.amqpLogin;
        clientSettings.pass = _settings.amqpPass;
        clientSettings.deliveredMessageExpirationSec = 60;

        if( ! client->init(clientSettings) ){
            state->lastError = client->getLastError();
            return nullptr;
        }

        const string clientName = "ip" + common_utils::getIpAddressStr() + "_" + "pid" + std::to_string( ::getpid() );        

        // client -> core
        SAmqpRouteParameters routes;
        routes.predatorExchangePointName = "dss_dx_clients";
        routes.predatorQueueName = "dss_q_client_mailbox_" + clientName;
        routes.predatorRoutingKeyName = "dss_rk_to_client_" + clientName;
        routes.targetExchangePointName = "dss_dx_core";
        routes.targetRoutingKeyName = "dss_rk_to_core";
        routes.targetQueueName = "dss_q_core_mailbox";

        AmqpController::SInitSettings settings2;
        settings2.client = client;
        settings2.route = routes;

        PAmqpController controller = std::make_shared<AmqpController>( INetworkEntity::INVALID_CONN_ID );
        const bool rt = controller->init( settings2 );
        if( ! rt ){
            state->lastError = controller->getState().lastError;
            return nullptr;
        }

        return controller;
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
    m_impl->commandServices.network = m_impl->createNetworkConnection( _settings );
    if( ! m_impl->commandServices.network ){
        VS_LOG_ERROR << PRINT_HEADER << " network connection failed, reason [" << m_impl->state->lastError << "]" << endl;
        return false;
    }

    // ping
    m_impl->commandPing = std::make_shared<CommandPing>( & m_impl->commandServices );

    // logger ( library exist in a main system, and therefore logging rules will be imposed from the 'outside' )

    m_impl->commandPing->m_clientId = m_impl->clientUniqueId;
    m_impl->commandPing->execAsync();

    // async processing
    m_impl->threadClientMaintenance = new std::thread( & PrivateImplementation::threadClientMaintenance, m_impl );

    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

const DssClient::PConstState DssClient::getState(){
    return m_impl->state;
}

}

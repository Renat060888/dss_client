#ifndef DSS_CLIENT_H
#define DSS_CLIENT_H

#include <boost/signals2.hpp>

#include "handler_node_simula.h"
#include "handler_node_real.h"
#include "handler_player.h"

namespace dss_client {

// the client's logic is maximum "stupid" - while connection is established what "good" and what "bad" treated by server
// the only reason a client can prohibit something by itself - it's connection absence to server
// the client only show system's state and transfers commands

// NOTE: логика клиента максимально "тупая" - пока есть соединение с сервером, что "хорошо", а что "плохо" определяется сервером
// NOTE: единственная причина, по которой клиент может что то запретить сам - это отсутсвие соединения с сервером
// NOTE: клиент только отображает состояние системы и передает команды

class DssClient
{
public:
    struct SInitSettings {
        SInitSettings()
            : amqpBrokerPort(0)
        {}
        std::string amqpBrokerHost;
        std::string amqpBrokerVirtualHost;
        int amqpBrokerPort;
        std::string amqpLogin;
        std::string amqpPass;
    };

    struct SState {
        SState()
            : connected(false)
        {}
        SInitSettings settings;
        std::string lastError;
        bool connected;
    };
    using PState = std::shared_ptr<SState>;
    using PConstState = std::shared_ptr<const SState>;

    // singleton
    static DssClient * getInstance();
    static void releaseInstance( DssClient * & );

    // callbacks
    boost::signals2::signal<void( bool _online )> m_signalOnline;
    boost::signals2::signal<void( std::string _msg )> m_signalMessage;
    boost::signals2::signal<void( bool _online, common_types::TNodeId _id )> m_signalNodeSimulationOnline;
    boost::signals2::signal<void( bool _online, common_types::TNodeId _id )> m_signalNodeRealOnline;
    boost::signals2::signal<void( bool _online )> m_signalPlayerOnline;

    // system
    bool init( const SInitSettings & _settings );
    const PConstState getState();

    // handlers
    PHandlerPlayer getPlayerHandler();

    PHandlerNodeSimula getNodeSimulaHandler( common_types::TNodeId _id );
    std::vector<PHandlerNodeSimula> getNodeSimulaHandlers();

    PHandlerNodeReal getNodeRealHandler( common_types::TNodeId _id );
    std::vector<PHandlerNodeReal> getNodeRealHandlers();


private:
    // non-copyable
    DssClient();
    ~DssClient();

    DssClient( const DssClient & ) = delete;
    DssClient & operator=( const DssClient & ) = delete;

    // private stuff
    class PrivateImplementation * m_impl;
};

}

#endif // DSS_CLIENT_H


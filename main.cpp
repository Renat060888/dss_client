
#include <thread>

#include "dss_client.h"

// NOTE: main for library unit testing

int main( int argc, char ** argv, char ** env ){

    dss_client::DssClient * client = dss_client::DssClient::getInstance();

    dss_client::DssClient::SInitSettings settings;
    settings.amqpBrokerHost = "localhost";
    settings.amqpBrokerVirtualHost = "/";
    settings.amqpBrokerPort = 5672;
    settings.amqpLogin = "guest";
    settings.amqpPass = "guest";

    if( ! client->init(settings) ){
        return 1;
    }



    while( true ){
        //dummy
        std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    }



    return 0;
}

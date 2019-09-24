/**
 * @brief MQTTServer
 * 
 */

#ifndef _H_MQTT_SERVER
#define _H_MQTT_SERVER

#include <set>

#include "MQTTInstance.h"
#include "MQTTClient.h"
#include "iServerSocketListener.h"

namespace afm {
    namespace communications {
        using MQTTClients = std::set<MQTTClientSPtr>;

        class MQTTServer : public MQTTInstance, public iServerSocketListener
        {
            public:
                MQTTServer();
                virtual ~MQTTServer();

                virtual bool initialize(const MQTTOptions &options) override;
                virtual bool start(iSocketSPtr pSocket) override;

                /**
                 * IServerSocketListener interface implementation
                 */
                virtual void onClientConnected(iSocketSPtr pClient) override;

            private:
                MQTTClients m_clients;
        };
    }
}

#endif
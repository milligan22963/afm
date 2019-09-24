/**
 * @brief MQTTClient
 * 
 */

#ifndef _H_MQTT_CLIENT
#define _H_MQTT_CLIENT

#include <set>

#include "MQTTInstance.h"

namespace afm {
    namespace communications {
        class MQTTClient : public MQTTInstance
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(const MQTTOptions &options) override;
                virtual bool start(iSocketSPtr pSocket) override;

            private:
                std::string m_clientId;
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}

#endif
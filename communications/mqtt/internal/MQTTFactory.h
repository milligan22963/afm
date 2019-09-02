/**
 * @brief MQTTFactory
 * 
 * Class for creating MQTT messages
 * 
 */

#ifndef _H_MQTT_FACTORY
#define _H_MQTT_FACTORY

#include <memory>
#include "iMQTTListener.h"
#include "iMQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTFactory;

        using MQTTFactorySPtr = std::shared_ptr<MQTTFactory>;

        class MQTTFactory
        {
            public:
                MQTTFactory();
                virtual ~MQTTFactory();
                
                static MQTTFactorySPtr getInstance();

                iMQTTPacketSPtr createPacket(MQTTPacketType packetType);
                iMQTTPacketSPtr createPacket(const MQTTBuffer &buffer);
        };
    }
}
#endif
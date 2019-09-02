/**
 * @brief IMQTTListener
 * 
 * Interface for MQTT Client receipients
 * 
 */

#ifndef _H_IMQTTLISTENER
#define _H_IMQTTLISTENER

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <json.hpp>
#include "iMQTTPacket.h"
#include "MQTTTypes.h"

namespace afm {
    namespace communications {
        class iMQTTListener
        {
            public:
                virtual ~iMQTTListener() {};

                virtual void onConnected(bool success) = 0;
                virtual void onSubscriptionAdded(bool success) = 0;
                virtual void onSubscriptionRemoved(bool success) = 0;
                virtual void onDisconnected(bool success) = 0;
                virtual void onMessageReceived(const iMQTTPacketSPtr pPacket) = 0;
                virtual void onMessageDelivered(const iMQTTPacketSPtr pPacket) = 0;
        };

        using iMQTTListenerSPtr = std::shared_ptr<iMQTTListener>;
    }
}
#endif
/**
 * @brief IMQTTProcessorListener
 * 
 * Internal interface for MQTT Client/Server implementations
 * 
 */

#ifndef _H_IMQTTPROCESSOR_LISTENER
#define _H_IMQTTPROCESSOR_LISTENER

#include <memory>

#include "iMQTTPacket.h"

namespace afm {
    namespace communications {
        class iMQTTProcessListener
        {
            public:
                virtual ~iMQTTProcessListener() {}

                virtual void onConnected(const std::string &clientSocketId) = 0;
                virtual void onMessageReceived(const iMQTTPacketSPtr pPacket) = 0;
                virtual void onMessageDelivered(const iMQTTPacketSPtr pPacket) = 0;
                virtual void onDisconnected() = 0;
                virtual void onError() = 0;
        };

        using iMQTTProcessListenerSPtr = std::shared_ptr<iMQTTProcessListener>;
    }
}
#endif
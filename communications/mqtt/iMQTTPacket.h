/**
 * @brief IMQTTPacket
 * 
 * Interface for MQTT Packets
 * 
 */

#ifndef _H_IMQTT_PACKET
#define _H_IMQTT_PACKET

#include <memory>
#include "MQTTTypes.h"
#include "MQTTPacketOptions.h"

namespace afm {
    namespace communications {
        enum MQTTPacketType
        {
            MQTT_Reserved = 0,
            MQTT_Connect,
            MQTT_ConnectAck,
            MQTT_Publish,
            MQTT_PublishAck,
            MQTT_PublishReceive,
            MQTT_PublishRelease,
            MQTT_PublishComplete,
            MQTT_Subscribe,
            MQTT_SubscribeAck,
            MQTT_UnSubscribe,
            MQTT_UnSubscribeAck,
            MQTT_PingRequest,
            MQTT_PingResponse,
            MQTT_Disconnect,
            END_MQTT_PACKETS
        };

        class iMQTTPacket
        {
            public:
                virtual ~iMQTTPacket() {};

                virtual bool initialize(const MQTTOptions &options) = 0;
                virtual bool encodePacket(MQTTBuffer &buffer) = 0;
                virtual bool decodePacket(const MQTTBuffer &buffer) = 0;
                virtual MQTTPacketType getType() const = 0;
                virtual bool getPacketField(const std::string &field, std::string &value) = 0;
                virtual bool getPacketField(const std::string &field, uint8_t &value) = 0;
                virtual bool getPacketField(const std::string &field, uint16_t &value) = 0;
                virtual bool getPacketField(const std::string &field, uint32_t &value) = 0;
                virtual bool getPacketField(const std::string &field, MQTT_QOS &value) = 0;
        };

        using iMQTTPacketSPtr = std::shared_ptr<iMQTTPacket>;
    }
}
#endif
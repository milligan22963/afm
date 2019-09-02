/**
 * @brief MQTTProcessor
 * 
 * Class for MQTT basic MQTTProcessing
 * 
 */

#ifndef _H_MQTTPROCESSOR
#define _H_MQTTPROCESSOR

#include <atomic>
#include <list>
#include "iSocket.h"
#include "iSocketListener.h"
#include "iMQTTProcessorListener.h"

namespace afm {
    namespace communications {
        extern const std::string sc_processorType;
        extern const std::string sc_mqttClient;
        extern const std::string sc_mqttServer;
        
        class MQTTProcessor : public iSocketListener, public std::enable_shared_from_this<MQTTProcessor>
        {
            public:
                MQTTProcessor();
                virtual ~MQTTProcessor();

                virtual bool initialize(const MQTTOptions &options);

                virtual bool sendPacket(const iMQTTPacketSPtr pPacket);
                virtual void addListener(iMQTTProcessListenerSPtr pListener);
                virtual void removeListener(iMQTTProcessListenerSPtr pListener);

                /**
                 * ISocketListener interface implementation
                 */
                virtual void onConnected(const std::string &clientId) override;
                virtual void onDataReceived(const SocketBuffer &socketBuffer) override;
                virtual void onDataWritten(const SocketBuffer &socketBuffer) override;
                virtual void onError(int socketError) override;
                virtual void onDisconnected() override;

                virtual bool shutdown();
    
            protected:
                void processing();

            private:
                iSocketSPtr                         m_connection = nullptr;
                std::list<iMQTTProcessListenerSPtr> m_listeners;
        };

        using MQTTProcessorSPtr = std::shared_ptr<MQTTProcessor>;
    }
}
#endif
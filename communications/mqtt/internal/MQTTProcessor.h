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
#include <map>
#include <string>
#include "iSocket.h"
#include "iSocketListener.h"
#include "iMQTTProcessorListener.h"

namespace afm {
    namespace communications {

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
                virtual void onConnected(const std::string &clientSocketId) override;
                virtual void onDataReceived(const std::string &clientSocketId, const SocketBuffer &socketBuffer) override;
                virtual void onDataWritten(const std::string &clientSocketId, const SocketBuffer &socketBuffer) override;
                virtual void onError(const std::string &clientSocketId, int socketError) override;
                virtual void onDisconnected(const std::string &clientSocketId) override;

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
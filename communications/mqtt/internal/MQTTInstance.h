/**
 * @brief MQTTInstance
 * 
 */

#ifndef _H_MQTT_INSTANCE
#define _H_MQTT_INSTANCE

#include <atomic>
#include <memory>
#include <string>

#include "iSocket.h"
#include "iSocketListener.h"
#include "MQTTTypes.h"

namespace afm {
    namespace communications {
        class MQTTInstance : public iSocketListener, public std::enable_shared_from_this<MQTTInstance>
        {
            public:
                MQTTInstance();
                virtual ~MQTTInstance();

                virtual bool initialize(const MQTTOptions &options);
                virtual bool start(iSocketSPtr pSocket);

                virtual void shutdown();
                virtual uint16_t getNextMessageId();

                /**
                 * ISocketListener interface implementation
                 */
                virtual void onConnected() override;
                virtual void onDataReceived(const SocketBuffer &socketBuffer) override;
                virtual void onDataWritten(const SocketBuffer &socketBuffer) override;
                virtual void onError(int socketError) override;
                virtual void onDisconnected() override;

            protected:
                const MQTTOptions &getOptions() const { return m_instanceOptions; }
                
                template <typename Derived>
                std::shared_ptr<Derived> shared_from_base()
                {
                    return std::static_pointer_cast<Derived>(shared_from_this());
                }


            private:
                bool            m_hasPersistence = false;
                bool            m_isClient = true;
                uint64_t        m_nextMessageId = 0;
                std::string     m_persistanceFile = "/tmp";
                std::string     m_clientId = "none";
                uint32_t        m_maxBacklog = 100;
                iSocketSPtr     m_pSocket = nullptr;
                MQTTOptions     m_instanceOptions;
        };

        using MQTTInstanceSPtr = std::shared_ptr<MQTTInstance>;
    }
}

#endif
/**
 * @brief MQTTInstance
 * 
 */

#include <set>
#include "common/JSONUtil.h"
#include "MQTTPacketOptions.h"
#include "MQTTInstance.h"
#include "socketFactory.h"

namespace afm {
    namespace communications {
        MQTTInstance::MQTTInstance()
        {

        }

        MQTTInstance::~MQTTInstance()
        {
            shutdown();
        }

        bool MQTTInstance::initialize(const MQTTOptions &options)
        {
            std::set<bool> results;

            m_instanceOptions = options;

            MQTTOptions persistenceFields;
            if (common::extractValue(options, sc_persistence, persistenceFields) == true) {
                m_hasPersistence = true;
                results.insert(common::extractValue(persistenceFields, sc_persistencePath, m_persistanceFile));
                results.insert(common::extractValue(persistenceFields, sc_persistenceBacklog, m_maxBacklog));
            }

            bool success = results.find(false) == results.end() ? true : false;

            // restore any persisted messages for this client
            // establish next messageId to use
            if (success == true) {

            } else {
                m_hasPersistence = false;
            }

            return success;
        }

        bool MQTTInstance::start(iSocketSPtr pSocket)
        {
            bool success = false;

            m_pSocket = pSocket;

            if (m_pSocket->initialize(m_instanceOptions) == true) {
                success = true;
            }
            
            return success;
        }

        void MQTTInstance::shutdown()
        {
            if (m_pSocket != nullptr) {
                m_pSocket->shutdown();
                m_pSocket = nullptr;
            }
        }

        uint16_t MQTTInstance::getNextMessageId()
        {
            return ((++m_nextMessageId) % UINT16_MAX) + 1;
        }

        /**
         * ISocketListener interface implementation
         */
        void MQTTInstance::onConnected()
        {

        }

        void MQTTInstance::onDataReceived(const SocketBuffer &socketBuffer)
        {

        }

        void MQTTInstance::onDataWritten(const SocketBuffer &socketBuffer)
        {

        }

        void MQTTInstance::onError(int socketError)
        {

        }

        void MQTTInstance::onDisconnected()
        {
            
        }
    }
}
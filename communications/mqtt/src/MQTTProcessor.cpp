/**
 * @brief MQTTProcessor
 * 
 * Class for MQTT basic MQTTProcessing
 * 
 */

#include <iostream>
#include <unistd.h>

#include "MQTTPacketOptions.h"
#include "MQTTProcessor.h"
#include "socketFactory.h"
#include "MQTTFactory.h"
#include "JSONUtil.h"

namespace afm {
    namespace communications {

        MQTTProcessor::MQTTProcessor()
        {

        }

        MQTTProcessor::~MQTTProcessor()
        {
            shutdown();
        }

        bool MQTTProcessor::initialize(const MQTTOptions &options)
        {
            bool success = false;

            std::string instanceType;
            if (common::extractValue(options, sc_instanceType, instanceType) == true) {
                if (instanceType == sc_mqttClient) {
                    m_connection = SocketFactory::getInstance()->createSocket(eSocketType::eClientSocket);
                } else {
                    m_connection = SocketFactory::getInstance()->createSocket(eSocketType::eServerSocket);
                }
                if (m_connection->initialize(options) == true) {
                    m_connection->addListener(shared_from_this());
                    success = true;
                } else {
                    m_connection = nullptr;
                }
            }

            return success;
        }

        bool MQTTProcessor::sendPacket(const iMQTTPacketSPtr pPacket)
        {
            bool success = false;

            SocketBuffer data;

            if (pPacket->encodePacket(data) == true) {
                success = m_connection->write(data);
            }
            return success;
        }

        bool MQTTProcessor::shutdown()
        {
            m_listeners.clear();

            if (m_connection != nullptr) {
                m_connection->removeListener(shared_from_this());
                m_connection->shutdown();
                m_connection = nullptr;
            }

            return true;
        }

        void MQTTProcessor::addListener(iMQTTProcessListenerSPtr pListener)
        {
            m_listeners.push_back(pListener);
        }

        void MQTTProcessor::removeListener(iMQTTProcessListenerSPtr pListener)
        {
            m_listeners.remove(pListener);
        }

        void MQTTProcessor::onConnected(const std::string &clientSocketId)
        {
            // Inform of socket level connection
            for (auto listener : m_listeners) {
                listener->onConnected(clientSocketId);
            }
        }

        void MQTTProcessor::onDataReceived(const std::string &clientSocketId, const SocketBuffer &socketBuffer)
        {
            // convert this buffer to a message and pass it to the listener(s)
            iMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(socketBuffer);

            if (pPacket != nullptr) {
                for (auto listener : m_listeners) {
                    listener->onMessageReceived(clientSocketId, pPacket);
                }
            } else {
                // failed
            }
        }

        void MQTTProcessor::onDataWritten(const std::string &clientSocketId, const SocketBuffer &socketBuffer)
        {
            iMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(socketBuffer);

            if (pPacket != nullptr) {
                for (auto listener : m_listeners) {
                    listener->onMessageDelivered(clientSocketId, pPacket);
                }
            } else {
                // failed
            }
        }

        void MQTTProcessor::onError(const std::string &clientSocketId, int socketError)
        {
            for (auto listener : m_listeners) {
                listener->onError(clientSocketId);
            }
        }

        void MQTTProcessor::onDisconnected(const std::string &clientSocketId)
        {
            for (auto listener : m_listeners) {
                listener->onDisconnected(clientSocketId);
            }
        }
    }
}

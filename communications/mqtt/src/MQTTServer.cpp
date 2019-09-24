/**
 * @brief MQTTServer
 * 
 */

#include "MQTTServer.h"

#include "socketFactory.h"

namespace afm {
    namespace communications {
        MQTTServer::MQTTServer()
            : MQTTInstance()
        {

        }

        MQTTServer::~MQTTServer()
        {
            m_clients.clear();
        }

        bool MQTTServer::initialize(const MQTTOptions &options)
        {
            return MQTTInstance::initialize(options);
        }

        bool MQTTServer::start(iSocketSPtr pSocket)
        {
            if (pSocket == nullptr) {
                pSocket = SocketFactory::getInstance()->createSocket(eSocketType::eServerSocket);
            }

            pSocket->addListener(shared_from_base<MQTTServer>());

            return MQTTInstance::start(pSocket);
        }

        /**
         * IServerSocketListener interface implementation
         */
        void MQTTServer::onClientConnected(iSocketSPtr pClient)
        {
            // We should wait until they send over the connection message and then create
            // it with the appropriate client id
            MQTTClientSPtr pMQTTClient = std::make_shared<MQTTClient>();

            pMQTTClient->initialize(getOptions());

            pMQTTClient->start(pClient);

            m_clients.insert(pMQTTClient);
        }
    }
}
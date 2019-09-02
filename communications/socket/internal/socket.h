/**
 * Name: socket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_SOCKET
#define _H_AFM_SOCKET

#include <string>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "iSocket.h"

namespace afm {
  namespace communications {
    class Socket : public iSocket, public std::enable_shared_from_this<Socket>
    {
    public:
        Socket();
        virtual ~Socket();

        virtual bool initialize(const SocketOptions &options) override;
        virtual void shutdown() override;

        virtual void addListener(iSocketListenerSPtr pSocketListener) final;
        virtual void removeListener(iSocketListenerSPtr pSocketListener) final;


        virtual bool write(const SocketBuffer &data) override;
        virtual SocketBuffer read() override;
        virtual SocketBuffer readWait(uint32_t milliseconds) override;
        virtual SocketBuffer transfer(const SocketBuffer &data) override;
        virtual SocketBuffer transferWait(const SocketBuffer &data, uint32_t milliseconds) override;

    protected:
        bool connect();
        int32_t getSocketHandle() const { return m_socketHandle; }
        void setSocketHandle(int32_t socketHandle);
        uint16_t getPort() const { return m_port; }
        std::string getUrl() const { return m_url; }
        std::string getUrlAddress();
        const iSocketListeners &getSocketListeners() const { return m_socketListeners; }
        
        template <typename Derived>
        std::shared_ptr<Derived> shared_from_base()
        {
            return std::static_pointer_cast<Derived>(shared_from_this());
        }

        /**
         * @brief creates a processing thread in the context of the derived class
         */
        virtual std::thread createProcessingThread();

       /**
         * @brief used to process incoming data
         */
        void read_processing();

        virtual void socketFailure();

    private:
        ssize_t read(SocketBuffer &buffer);
        ssize_t readWait(SocketBuffer &buffer, uint32_t milliseconds);

        static const uint8_t sm_connectionDelay = 15;

    private:
      std::atomic<bool>   m_threadRunning;
      std::atomic<bool>   m_socketConnected;
      std::atomic<bool>   m_reconnect;
      int32_t             m_socketHandle = sc_closedSocket;
      struct sockaddr_in  m_socketAddress;
      std::string         m_socketClientId = "";
      std::string         m_url = "localhost";
      uint16_t            m_port = 9001;
      int                 m_lastError = 0;
      iSocketListeners    m_socketListeners;
      std::thread         m_processingThread;
    };

    using SocketSPtr = std::shared_ptr<Socket>; 
  }
}
#endif
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#define WIN32_LEAN_AND_MEAN
 
//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
 
// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
  
#define DEFAULT_BUFFER_LENGTH    512

class TcpClient {
public:
    TcpClient(const char* servername, const char *port);
    bool Start();
    // Free the resouces
    void Stop();
    // Send message to server
    bool Send(const char* szMsg, int len);
private:
    const char* szServerName;
	const char* szPort;
    SOCKET ConnectSocket;
};

#endif

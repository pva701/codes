#include "TcpClient.h"
 
TcpClient::TcpClient(const char* servername, const char* port)
{
    szServerName = servername;
	szPort = port;
    ConnectSocket = INVALID_SOCKET;
}
 
bool TcpClient::Start() {
    WSADATA wsaData;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }

    struct addrinfo    *result = NULL,
                    *ptr = NULL,
                    hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        
    hints.ai_socktype = SOCK_STREAM;    
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(szServerName, szPort, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return false;
    }

    ptr = result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
 
        // Connect to server
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
 
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }
 
        freeaddrinfo(result);
 
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return false;
        }
 
        return true;
    }
 
    // Free the resouces
void TcpClient::Stop() {
    int iResult = shutdown(ConnectSocket, SD_SEND);

    if (iResult == SOCKET_ERROR)
        printf("shutdown failed: %d\n", WSAGetLastError());

    closesocket(ConnectSocket);
    WSACleanup();
}
 
    // Send message to server
bool TcpClient::Send(const char* szMsg, int len)
{
    int iResult = send(ConnectSocket, szMsg, len, 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        Stop();
        return false;
    }
    return true;
}
 
#include "serversocket.h"
#include "clientsocket.h"
#include <sstream>

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class ServerSocket::ServerSocket_priv
{
	friend class ServerSocket;
private:
	ServerSocket_priv()
	{
		ListenSocket = INVALID_SOCKET;
	}
    SOCKET ListenSocket;
};

ServerSocket::ServerSocket()
{
	m_pServerSocket = new ServerSocket_priv();
}

ServerSocket::~ServerSocket()
{
	delete m_pServerSocket;
}

int ServerSocket::Init( int iPort )
{
	WSADATA wsaData;
    int iResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    if( iResult != 0 )
	{
        printf( "WSAStartup failed with error: %d\n", iResult );
        return 1;
    }

    ZeroMemory( &hints, sizeof( hints ));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
	std::stringstream strBuffer;
	strBuffer << iPort;
    iResult = getaddrinfo( NULL, strBuffer.str().c_str(), &hints, &result );
    if ( iResult != 0 )
	{
        printf( "getaddrinfo failed with error: %d\n", iResult );
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    m_pServerSocket->ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
    if( m_pServerSocket->ListenSocket == INVALID_SOCKET )
	{
        printf( "socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo( result );
        WSACleanup();
        return 1;
    }

  // Setup the TCP listening socket
    iResult = bind( m_pServerSocket->ListenSocket, result->ai_addr, (int)result->ai_addrlen );
    if( iResult == SOCKET_ERROR )
	{
        printf( "bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        Close();
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
	return 0;
}

ClientSocket* ServerSocket::WaitForConections()
{
    SOCKET iClientSocket = INVALID_SOCKET;
	int iResult;

    iResult = listen( m_pServerSocket->ListenSocket, SOMAXCONN );
    if (iResult == SOCKET_ERROR)
	{
        printf( "listen failed with error: %d\n", WSAGetLastError());
        Close();
        WSACleanup();
        return 0;
    }

	// Accept a client socket
    iClientSocket = accept( m_pServerSocket->ListenSocket, NULL, NULL );
    if( iClientSocket == INVALID_SOCKET )
	{
        printf( "accept failed with error: %d\n", WSAGetLastError());
        Close();
        WSACleanup();
        return 0;
    }
	return new ClientSocket( iClientSocket );
}

void ServerSocket::Close()
{
	// No longer need server socket
    closesocket(m_pServerSocket->ListenSocket);
	 WSACleanup();
}


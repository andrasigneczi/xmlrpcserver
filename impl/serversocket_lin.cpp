#include "serversocket.h"
#include "clientsocket.h"
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class ServerSocket::ServerSocket_priv
{
	friend class ServerSocket;
private:
	ServerSocket_priv()
	{
		m_iListenSocket = -1;
	}
   	int m_iListenSocket;
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
	int forceRebind = 1;
	struct sockaddr_in server;

	m_pServerSocket->m_iListenSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( -1 == m_pServerSocket->m_iListenSocket )
	{
		perror( "socket failed with error" );
		return 1;
	}

	if( -1 == setsockopt( m_pServerSocket->m_iListenSocket, SOL_SOCKET, SO_REUSEADDR, &forceRebind, sizeof( int )))
	{
		perror( "setsockopt failed with error" );
		return 1;
	}

	memset((void *) &server, 0, sizeof( struct sockaddr_in ));
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = INADDR_ANY;

	if( -1 == bind( m_pServerSocket->m_iListenSocket, (const struct sockaddr *) &server, sizeof( struct sockaddr_in )))
	{
		perror( "bind failed with error" );
		return 1;
	}
	return 0;
}

ClientSocket* ServerSocket::WaitForConections()
{
	struct sockaddr_in client;
	socklen_t clientaddrlen = 0;
	int iClientSocket;
	if( -1 == listen( m_pServerSocket->m_iListenSocket, 0 ))
	{
		perror( "listen failed with error" );
		return 0;
	}

	iClientSocket = accept( m_pServerSocket->m_iListenSocket, (struct sockaddr *) &client, &clientaddrlen );
	if( -1 == iClientSocket )
	{
		perror( "accept failed with error" );
		return 0;
	}
	return new ClientSocket( iClientSocket );
}

void ServerSocket::Close()
{
	if( -1 == close(m_pServerSocket->m_iListenSocket ))
		perror( "socket close failed with error" );
}

#include "clientsocket.h"

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

class ClientSocket::ClientSocket_priv
{
	friend class ClientSocket;
private:
	ClientSocket_priv( int iSocket )
	: stream( recvbuf, 0 )
	{
		memset( recvbuf, 0, recvbuflen );
		m_iSocket = iSocket;
	}

	static const size_t recvbuflen = 4096;
	char recvbuf[ recvbuflen ];
	int m_iSocket;
	
	CharStream stream;
};

ClientSocket::ClientSocket( int iSocket )
{
	m_pClientSocket_priv = new ClientSocket_priv( iSocket );
}

ClientSocket::~ClientSocket()
{
	delete m_pClientSocket_priv;
}

CharStream& ClientSocket::Recv()
{
	int iResult = recv( m_pClientSocket_priv->m_iSocket, m_pClientSocket_priv->recvbuf, m_pClientSocket_priv->recvbuflen, 0 );
    if( iResult > 0 )
	{
		printf( "Bytes received: %d\n", iResult );
		m_pClientSocket_priv->stream.SetLength( iResult );
	}
	else
	{
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket( m_pClientSocket_priv->m_iSocket );
        WSACleanup();
        throw Socket_Recv_Error();
	}
	return m_pClientSocket_priv->stream;
}

int ClientSocket::Send( const std::string& strBuffer )
{
	int iSendResult = send( m_pClientSocket_priv->m_iSocket, strBuffer.c_str(), strBuffer.length(), 0 );
    if( iSendResult == SOCKET_ERROR )
	{
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket( m_pClientSocket_priv->m_iSocket );
        WSACleanup();
        //return iSendResult;
		throw Socket_Send_Error();
    }
	return iSendResult;
}

void ClientSocket::Close()
{
	// shutdown the connection since we're done
    int iResult = shutdown( m_pClientSocket_priv->m_iSocket, SD_SEND );
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_pClientSocket_priv->m_iSocket);
        WSACleanup();
        return;
    }
    closesocket(m_pClientSocket_priv->m_iSocket);
	//WSACleanup();
}


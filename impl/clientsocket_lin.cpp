#include "clientsocket.h"

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
		// printf( "Bytes received: %d\n", iResult );
		m_pClientSocket_priv->stream.SetLength( iResult );
	}
	else
	{
        perror("recv failed\n");
        close( m_pClientSocket_priv->m_iSocket );
        throw Socket_Recv_Error();
	}
	return m_pClientSocket_priv->stream;
}

int ClientSocket::Send( const std::string& strBuffer )
{
	int iSendResult = send( m_pClientSocket_priv->m_iSocket, strBuffer.c_str(), strBuffer.length(), 0 );
    if( iSendResult == -1 )
	{
        perror("send failed\n");
        close( m_pClientSocket_priv->m_iSocket );
		throw Socket_Send_Error();
    }
	return iSendResult;
}

void ClientSocket::Close()
{
	// shutdown the connection since we're done
    int iResult = shutdown( m_pClientSocket_priv->m_iSocket, SHUT_RDWR );
    if (iResult == -1)
        perror("shutdown failed\n");
    close(m_pClientSocket_priv->m_iSocket);
}


/* 
 * File:   socket.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 21., 08:21
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef CLIENTSOCKET_H
#define	CLIENTSOCKET_H

#include "clientsocket.h"
#include "charstream.h"

/*
 *************************************
 ********* Socket exceptions *********
 *************************************
*/
class Socket_Error {};
class Socket_Recv_Error : public Socket_Error {};
class Socket_Send_Error : public Socket_Error {};

/*
 ********************************
 ********* ClientSocket *********
 ********************************
*/
class ClientSocket
{
public:
	~ClientSocket();

	CharStream& Recv();
	int Send( const std::string& strBuffer );
	void Close();

private:
	friend class ServerSocket;
	ClientSocket( int iSocket );
	class ClientSocket_priv;
	ClientSocket_priv* m_pClientSocket_priv;
};

#endif // SOCKET_H

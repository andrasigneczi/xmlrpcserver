/* 
 * File:   serversocket.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 21., 08:21
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef SERVERSOCKET_H
#define	SERVERSOCKET_H

class ClientSocket;

/*
 ********************************
 ********* ServerSocket *********
 ********************************
*/
class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();
	int Init( int iPort );
	ClientSocket* WaitForConections();
	void Close();

private:
	class ServerSocket_priv;
	ServerSocket_priv* m_pServerSocket;
};
#endif //
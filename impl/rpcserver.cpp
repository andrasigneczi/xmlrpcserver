#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif
//#include <string.h>

#include "rpcserver.h"
#include "rpcagent.h"
#include <iostream>

#include "serversocket.h"

void DeleteRpcAgent( RpcAgent* pAgent )
{
	//usleep( 10000 );
	pAgent->Join();
	delete pAgent;
}

RpcServer::RpcServer( int iConnectionPort, const int iAgentNumberLimit /*= INT_MAX*/ )
: m_iAgentNumberLimit( iAgentNumberLimit )
, m_iConnectionPort( iConnectionPort )
{
}

RpcServer::~RpcServer()
{
}

void RpcServer::MainEntry()
{
	ServerSocket oServer;
	if( oServer.Init( m_iConnectionPort ))
		return;

	for( int i = 0; i < m_iAgentNumberLimit; i++ )
	{
		ClientSocket* pClient = oServer.WaitForConections();
		
		if( !pClient )
			return;

		RpcAgent* pAgent = new RpcAgent( pClient );
		pAgent->Start();
		std::thread( DeleteRpcAgent, pAgent ).detach();
	}
	oServer.Close();
}


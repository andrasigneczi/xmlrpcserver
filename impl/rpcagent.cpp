#ifndef WIN32
#include <sys/socket.h>
#endif


#include "rpcagent.h"
#include "httpheader.h"
#include "xml.h"
#include "methodbuilder.h"
#include "method.h"

#include <iostream>
#include <sstream>
//#include <mutex>
#include "clientsocket.h"


RpcAgent::RpcAgent( ClientSocket* pClientSocket )
{
	m_pClientSocket = pClientSocket;
}

RpcAgent::~RpcAgent()
{
	delete m_pClientSocket;
	delete m_pThread;
}

void RpcAgent::Start()
{
	m_pThread = new std::thread( &RpcAgent::MainEntry, std::ref( *this ));
}

void RpcAgent::MainEntry()
{
	try
	{
		ReceiveCommand();
	}
	catch( Socket_Error )
	{
		std::cout <<"Invalid connection\n";
	}
}

void RpcAgent::ReceiveCommand()
{
	CharStream& stream = m_pClientSocket->Recv();
	ssize_t iStreamLength = stream.length();
	HttpHeader oHeader( stream );
	ssize_t iHLength = oHeader.ContentLength();

	// header not found: try to use content witout it
	if( iHLength == -1 )
	{
		iHLength = iStreamLength;
		stream.Reset( iStreamLength );
	}
	
	if( iHLength > 0 && stream.length() > 0 )
	{
		MethodBuilder methodBuilder;;
		XmlParser xmlParser( &methodBuilder );
		try
		{
			if( xmlParser.Open( stream ))
				xmlParser.Read( stream );
		}
		catch( Xml_Illegal_Header )
		{

		}

		if( xmlParser.MethodCallResultsCount() == 0 )
		{
			std::string strBuff;
			const std::string strRes( "<html><body><h3>Invalid method call format.</h3></body></html>" );
			// Should we send  HTTP header?
#ifdef USE_HEADER_FOR_RESPONSE
			std::stringstream strResLength;
			strResLength << strRes.length();
			strBuff = std::string( "HTTP/1.1 200 OK\n" ) +
									"Date: Tue, 20 Aug 2013 11:30:04 GMT\n" +
									"Server: Apache/2.2.22 (Ubuntu)\n" +
									"X-Powered-By: PHP/5.3.10-1ubuntu3.7\n" +
									"Vary: Accept-Encoding\n" +
									"Content-Length: " + strResLength.str() + "\n" +
									"Connection: close\n" +
									"Content-Type: text/xml\n" +
									"\n" + strRes;
#else
			strBuff = strRes;
#endif
			m_pClientSocket->Send( strBuff );
		}

		while( xmlParser.MethodCallResultsCount() > 0 )
		{
			const std::string& strRes = xmlParser.GetMethodCallResult();
			std::string strBuff;

			// Should we send  HTTP header?
#ifdef USE_HEADER_FOR_RESPONSE
			std::stringstream strResLength;
			strResLength << strRes.length();
			strBuff = std::string( "HTTP/1.1 200 OK\n" ) +
									"Date: Tue, 20 Aug 2013 11:30:04 GMT\n" +
									"Server: Apache/2.2.22 (Ubuntu)\n" +
									"X-Powered-By: PHP/5.3.10-1ubuntu3.7\n" +
									"Vary: Accept-Encoding\n" +
									"Content-Length: " + strResLength.str() + "\n" +
									"Connection: close\n" +
									"Content-Type: text/xml\n" +
									"\n" + strRes;
#else
			strBuff = strRes;
#endif

			m_pClientSocket->Send( strBuff );
		}
	}
	m_pClientSocket->Close();
}

void RpcAgent::Join()
{
	m_pThread->join();
}

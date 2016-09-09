#include <stdexcept>
#include <sstream>
#include <iostream>

#include "objserver.h"
#include "xml.h"
#include "rpcserver.h"

#include <stdlib.h>
#include "testserverobjects.h"

// for memory leak test
// #define VALGRIND
#ifdef VALGRIND
#include <unistd.h>
#endif

void Terminate()
{
}

void DeleteSingletons()
{
	ObjServer::Destroy();
	XmlParserClosed::Destroy();
	XmlParserOpened::Destroy();
	XmlParserPaused::Destroy();
}

int main( int argc, char* argv[] )
{
    std::set_terminate( Terminate );
	atexit( DeleteSingletons );
	
	// Registration of first object
	typedef MethodReturnValue (TestServerObject1::*TSOMF)( const std::vector<MethodParameter*>& vecParameters );
	TestServerObject1 oTO;
	MemeberFunction_Impl<TestServerObject1*, TSOMF> MF( &oTO, &TestServerObject1::Add );
	std:: cout << ( ObjServer::Instance()->Register( "TestServerObject.Add", &MF ) ? "registered\n" : "registration fault\n" );

	// Registration of second object
	typedef MethodReturnValue (TestServerObject2::*TSO2MF)( const std::vector<MethodParameter*>& vecParameters );
	TestServerObject2 oTO2;
	MemeberFunction_Impl<TestServerObject2*, TSO2MF> MF2( &oTO2, &TestServerObject2::DuplicateParameter );
	std:: cout << ( ObjServer::Instance()->Register( "TestServerObject2.DupParam", &MF2 ) ? "registered\n" : "registration fault\n" );

	int iConnectionPort = 1234;

	if( argc > 1 )
	{
		try
		{
			iConnectionPort = std::stoi( argv[ 1 ] );
			std::cout << "Server port: " << iConnectionPort;
		}
		catch( std::exception )
		{
			std::cout << "Invalid port number. Default port will be used (1234).\n";
		}
	}
	else
	{
		std::cout << "usage: xmlrpcserver.exe [port]. Default port will be used (1234).\n";
	}

#ifdef VALGRIND
	RpcServer oRpcServer( iConnectionPort, 10000 );
#else
	RpcServer oRpcServer( iConnectionPort );
#endif
	std::thread oServerThread( &RpcServer::MainEntry, std::ref( oRpcServer ));
	oServerThread.join();

#ifdef VALGRIND
	usleep( 2000000 );
#endif
	return 1;
}

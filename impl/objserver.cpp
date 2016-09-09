#include "objserver.h"
#include <string>
#include <vector>

ObjServer* ObjServer::m_pSingleton = 0;

ObjServer::ObjServer()
{
}

ObjServer* ObjServer::Instance()
{
	if( m_pSingleton == 0 )
		m_pSingleton = new ObjServer();
	return m_pSingleton;
}

void ObjServer::Destroy()
{
  if( m_pSingleton )
    delete m_pSingleton;
  m_pSingleton = 0;
}

const std::string ObjServer::Call( const std::string& strName, const std::vector<MethodParameter*>& vecMethodParameters )
{
	MapMemberType::const_iterator it = m_mapRegisteredFunctions.find( strName );
	if( it == m_mapRegisteredFunctions.end())
		throw UnKnown_Method_Error();
	return it->second->Call( vecMethodParameters );
}

bool ObjServer::Register( const std::string& strName, MemberFunction* pFunction )
{
	MapMemberType::const_iterator it = m_mapRegisteredFunctions.find( strName );
	if( it != m_mapRegisteredFunctions.end())
		return false;
	m_mapRegisteredFunctions.insert( PairMemberType( strName, pFunction ));
	return true;
}



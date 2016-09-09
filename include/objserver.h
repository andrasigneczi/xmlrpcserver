/*
 * File:   objserver.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 17., 9:27
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef OBJSERVER_H
#define	OBJSERVER_H

#include <map>
#include <string>
#include <vector>

#include "method.h"
#include "memberfunction.h"

/*
 *****************************
 ********* ObjServer *********
 *****************************
*/
class ObjServer
{
public:
	static ObjServer* Instance();
	static void Destroy();
	const std::string Call( const std::string& strName, const std::vector<MethodParameter*>& vecMethodParameters );
	bool Register( const std::string& strName, MemberFunction* pFunction );
	
private:
	ObjServer();
	static ObjServer* m_pSingleton;
	typedef std::map<std::string, MemberFunction*> MapMemberType;
	typedef std::pair<std::string, MemberFunction*> PairMemberType;
	MapMemberType m_mapRegisteredFunctions;
};

#endif // OBJSERVER_H
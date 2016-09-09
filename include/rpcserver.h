/* 
 * File:   rpcserver.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 20., 13:27
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef RPCSERVER_H
#define	RPCSERVER_H

#include "thread.h"
#include <string>
#include <climits>

/*
 *****************************
 ********* RpcServer *********
 *****************************
*/
class RpcServer
{
public:
	RpcServer( int iConnectionPort, const int iAgentEndNum = INT_MAX );
	~RpcServer();
	void MainEntry();

private:
	std::string m_strEndingStatus;
	const int m_iAgentNumberLimit;
	const int m_iConnectionPort;
};

#endif	/* RPCSERVER_H */


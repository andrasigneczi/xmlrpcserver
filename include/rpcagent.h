/*
 * File:   rpcagent.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 20., 16:34
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef RPCAGENT_H
#define	RPCAGENT_H

#include "rpcserver.h"
class ClientSocket;

/*
 *****************************
 ********* RpcAgent **********
 *****************************
*/
class RpcAgent
{
	friend class RpcServer;
	RpcAgent( ClientSocket* );
	void Start();
	void ReceiveCommand();
	
public:
	~RpcAgent();
	void MainEntry();
	void Join();
	
private:
	ClientSocket* m_pClientSocket;
	std::string m_strEndingStatus;
	std::thread* m_pThread;
};

#endif	/* RPCAGENT_H */

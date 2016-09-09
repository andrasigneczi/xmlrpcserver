/* 
 * File:   method.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 18., 14:13
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef METHOD_H
#define	METHOD_H

#include <string>
#include <vector>
#include <sstream>

#include "methodparameter.h"

/*
 *******************************
 ********* Method **************
 *******************************
*/
class Method
{
	friend class MethodBuilder;
	Method();
	void SetName( const std::string& strName );
	void SetParameters( std::vector<MethodParameter*>* pParameters );
	void Finished();

	void DumpParameters(); // for debug
	void DeleteParameters();
	
public:
	~Method();
	std::string DoCall();
	const std::string& GetName() const;
	bool IsFinished();

private:
	bool m_bFinished;
	std::string m_strName;
	std::vector<MethodParameter*>* m_pParameters;
};

/*
 ******************************************
 ********* MethodReturnValue **************
 ******************************************
*/
class MethodReturnValue
{
public:
	~MethodReturnValue();
	explicit MethodReturnValue( int iValue );
	explicit MethodReturnValue( double dValue );
	explicit MethodReturnValue( bool bValue );
	explicit MethodReturnValue( std::string strValue );
	explicit MethodReturnValue( MethodParameter* pParameter, bool bDestroy = false );
	explicit MethodReturnValue( std::vector<MethodParameter*>* pParameters, bool bDestroy = false );

	MethodReturnValue( const MethodReturnValue& o );
	operator const std::string();

private:
	MethodReturnValue& operator=( const MethodReturnValue& );
private:
	mutable std::stringstream* m_pstrRetValue;
};


#endif	/* METHOD_H */


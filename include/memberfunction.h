/*
 * File:   memberfunction.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 22., 17:20
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef MEMBERFUNCTION_H
#define	MEMBERFUNCTION_H

#include <string>
#include <vector>

#include "method.h"

/*
 **********************************
 ********* MemberFunction *********
 **********************************
*/
class MemberFunction
{
public:
	virtual std::string Call( const std::vector<MethodParameter*>& ) = 0;
};

/*
 ***************************************
 ********* MemberFunction_Impl *********
 ***************************************
*/
template<class T,class F>
class MemeberFunction_Impl : public MemberFunction
{
public:
	MemeberFunction_Impl( T pObject, F pMemberFunction );
	std::string Call( const std::vector<MethodParameter*>& );
	
private:
	T m_pObject;
	F m_pMemberFunction;
};

template<class T,class F>
MemeberFunction_Impl<T,F>::MemeberFunction_Impl( T pObject, F pMemberFunction )
{
	m_pObject = pObject;
	m_pMemberFunction = pMemberFunction;
}

template<class T,class F>
std::string MemeberFunction_Impl<T,F>::Call( const std::vector<MethodParameter*>& vecParameters )
{
	return (m_pObject->*m_pMemberFunction)( vecParameters );
}

#endif // MEMBERFUNCTION_H

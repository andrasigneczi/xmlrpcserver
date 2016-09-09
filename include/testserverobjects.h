/* 
 * File:   thread.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 22., 13:07
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef TESTSERVEROBJECTS_H
#define	TESTSERVEROBJECTS_H

#include "method.h"

/*
 *************************************
 ********* TestServerObject1 *********
 *************************************
*/
class TestServerObject1
{
public:
	MethodReturnValue Add( const std::vector<MethodParameter*>& vecParameters );
};

/*
 *************************************
 ********* TestServerObject2 *********
 *************************************
*/
class TestServerObject2
{
public:
	MethodReturnValue DuplicateParameter( const std::vector<MethodParameter*>& vecParameters );

private:
	MethodParameter* CopyArray( const MethodParameter* pArray, bool bDuplicate );
	MethodParameter* CopyStruct( const MethodParameter* pStruct, bool bDuplicate );
	MethodParameter* CopyItem( const MethodParameter* pItem, bool bDuplicate );
};

#endif // TESTSERVEROBJECTS_H
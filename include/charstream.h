/* 
 * File:   charstream.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 17., 17:38
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef CHARSTREAM_H
#define	CHARSTREAM_H

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

#include <sys/types.h>
#include <stdexcept>
#include "windows_defs.h"

/*
 ********************************
 ********* CharStream ***********
 ********************************
*/
class CharStream
{
public:
	CharStream( const char* const pcBuffer, ssize_t iDataLength );
	void Reset( const char* const pcBuffer, ssize_t iDataLength );
	void Reset( ssize_t iDataLength );
	void SetLength( ssize_t iDataLength );
	char get() throw( std::out_of_range );
	ssize_t length() const;
	void operator++() throw( std::out_of_range );
	
private:
	const char* m_pcBuffer;
	ssize_t     m_iSize;
};


#endif	/* CHARSTREAM_H */


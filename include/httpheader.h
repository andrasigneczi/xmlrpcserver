/* 
 * File:   httpheader.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 17., 10:31
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef HTTPHEADER_H
#define	HTTPHEADER_H

#include <sys/types.h>
#include <string>
#include <map>
#include "charstream.h"

/*
 **********************************
 ********* HttpHeader *************
 **********************************
*/
class HttpHeader
{
public:
	HttpHeader( CharStream& stream );
	ssize_t ContentLength() const;
	const std::string& UserAgent() const;
	const std::string& Host() const;
	const std::string& ContentType() const;
	
private:
	typedef std::map<const std::string, const std::string> values_t;
	values_t    m_mapValues;
	static const std::string m_strIllegalKey;
};


#endif	/* HTTPHEADER_H */


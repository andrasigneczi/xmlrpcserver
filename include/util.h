/* 
 * File:   util.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 17., 18:11
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef STRING_H
#define	STRING_H

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

static inline std::string& ltrim( std::string& s )
{
	s.erase( s.begin(), std::find_if( s.begin(), s.end(), std::not1( std::ptr_fun<int, int>( std::isspace ))));
	return s;
}

static inline std::string& rtrim( std::string& s )
{
        s.erase( std::find_if( s.rbegin(), s.rend(), std::not1( std::ptr_fun<int, int>( std::isspace ))).base(), s.end());
        return s;
}

static inline std::string& trim( std::string& s )
{
        return ltrim( rtrim( s ));
}

#endif	/* STRING_H */


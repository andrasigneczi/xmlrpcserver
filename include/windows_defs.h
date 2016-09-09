/* 
 * File:   windows_defs.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 21., 9:27
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef WINDOWS_DEFS_H
#define	WINDOWS_DEFS_H

#ifdef WIN32
typedef int ssize_t;
#endif

#endif // WINDOWS_DEFS_H

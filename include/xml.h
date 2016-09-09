/* 
 * File:   xml.h
 * Author: andrej
 *
 * Created on 2013. augusztus 17., 9:27
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef XML_H
#define	XML_H

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

#include <sys/types.h>
#include "charstream.h"
#include <vector>

class XmlParserState;

class XmlParserError{};
class Xml_Illegal_Header : public XmlParserError {};
class Xml_Parse_Error    : public XmlParserError {};
class Xml_Invalid_Tag    : public XmlParserError {};

class MethodBuilder;

/*
 **********************************************************
 **********************************************************
 *********************** XmlParser ************************
 **********************************************************
 **********************************************************
*/
class XmlParser
{
public:
	XmlParser( MethodBuilder* pMethod );
	bool Open( CharStream& stream );
	void Close();
	bool Read( CharStream& stream );
	int  MethodCallResultsCount();
	std::string GetMethodCallResult();
	
private:
	friend class XmlParserState;
	void ChangeState( XmlParserState* s );
	const std::string& GetCurrentLine() const;
	void SetCurrentLine( const std::string& strCurrentLine );
	void SetXmlVersion( const std::string& strXmlVersion );
	void SetXmlEncoding( const std::string& strXmlEncoding );

	bool InitTag( const std::string& strTag );
	bool SetTagValue( const std::string& strAttrib );
	bool CloseTag( const std::string& strTag );

private:
	XmlParserState *m_pState;
	std::string     m_strCurrentLine;
	std::string     m_strXmlVersion;
	std::string     m_strXmlEncoding;
	MethodBuilder*  m_pMethodBuilder;

	std::vector<std::string> m_vecMethodCallResults;
};

/*
 **********************************************************
 **********************************************************
 ********************* XmlParserState *********************
 **********************************************************
 **********************************************************
*/
class XmlParserState
{
public:
	virtual bool Open( XmlParser* p, CharStream& stream ) { return false; }
	virtual void Close( XmlParser* p ) {}
	virtual bool Read( XmlParser* p, CharStream& stream ){ return false; }
	
protected:
	XmlParserState();
	virtual ~XmlParserState() {};
	void ChangeState( XmlParser* t, XmlParserState* s );
	
	virtual bool ParseCurrentLine( XmlParser* p, const std::string& strCurrentLine ) throw( Xml_Parse_Error, Xml_Invalid_Tag );
	virtual bool ParseXmlHeader( XmlParser* p, const std::string& strCurrentLine ) throw( Xml_Illegal_Header );
	void SetCurrentLine( XmlParser* p, const std::string& strCurrentLine );
	const std::string& GetCurrentLine( XmlParser* p ) const;
	
protected:
	
private:
	std::string* ParseXmlHeader_attrib( const std::string& strCurrentLine, const std::string& strAttrName )  throw( Xml_Illegal_Header );
};

/*
 **********************************************************
 **********************************************************
 ********************* XmlParserClosed ********************
 **********************************************************
 **********************************************************
*/
class XmlParserClosed : public XmlParserState
{
	XmlParserClosed();
public:
	static XmlParserState* Instance();
	static void Destroy();
	virtual bool Open( XmlParser* p, CharStream& stream );
	
private:
	static XmlParserClosed* m_pState;
};

/*
 **********************************************************
 ********************* XmlParserOpened ********************
 **********************************************************
*/
class XmlParserOpened : public XmlParserState
{
	XmlParserOpened();
public:
	static XmlParserState* Instance();
	static void Destroy();
	virtual bool Read( XmlParser*, CharStream& stream );
	virtual void Close( XmlParser* p );
	
private:
	static XmlParserOpened* m_pState;
};

/*
 **********************************************************
 ********************* XmlParserPaused ********************
 **********************************************************
 */
class XmlParserPaused : public XmlParserState
{
	XmlParserPaused();
public:
	static XmlParserState* Instance();
	static void Destroy();
	virtual bool Read( XmlParser*, CharStream& stream );
	virtual void Close( XmlParser* p );
	
private:
	static XmlParserPaused* m_pState;
};

#endif	/* XML_H */


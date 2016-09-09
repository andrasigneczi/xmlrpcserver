#include "xml.h"
//#include <regex>
#include <memory>
#include <iostream>
#include <stack>
#include <map>
#include "util.h"
#include "methodbuilder.h"
#include "method.h"

XmlParserClosed* XmlParserClosed::m_pState = 0;
XmlParserOpened* XmlParserOpened::m_pState = 0;
XmlParserPaused* XmlParserPaused::m_pState = 0;

XmlParser::XmlParser( MethodBuilder* pMethodBuilder )
{
	m_pMethodBuilder = pMethodBuilder;
	m_pState = XmlParserClosed::Instance();
}

bool XmlParser::Open( CharStream& stream )
{
	return m_pState->Open( this, stream );
}

void XmlParser::Close()
{
	m_pState->Close( this );
}

bool XmlParser::Read( CharStream& stream )
{
	return m_pState->Read( this, stream );
}

void XmlParser::ChangeState( XmlParserState* s )
{
	m_pState = s;
}

const std::string& XmlParser::GetCurrentLine() const
{
	return m_strCurrentLine;
}

void XmlParser::SetCurrentLine( const std::string& strCurrentLine )
{
	m_strCurrentLine = strCurrentLine;
}

void XmlParser::SetXmlVersion( const std::string& strXmlVersion )
{
	m_strXmlVersion = strXmlVersion;
}

void XmlParser::SetXmlEncoding( const std::string& strXmlEncoding )
{
	m_strXmlEncoding = strXmlEncoding;
}

bool XmlParser::InitTag( const std::string& strTag )
{
	return m_pMethodBuilder->InitNode( strTag );
}

bool XmlParser::SetTagValue( const std::string& strAttrib )
{
	// todo encodinggal a stringeket visszaalakítani
	return m_pMethodBuilder->SetNodeValue( strAttrib );
}

bool XmlParser::CloseTag( const std::string& strTag )
{
	if( m_pMethodBuilder->CloseNode( strTag ))
	{
		if( m_pMethodBuilder->MethodReadyToCall())
		{
			Method* pMethod = m_pMethodBuilder->ReleaseMethod();
			m_vecMethodCallResults.push_back( pMethod->DoCall());
			delete pMethod;
		}
		return true;
	}

	return false;
}

int XmlParser::MethodCallResultsCount()
{
	return m_vecMethodCallResults.size();
}

std::string XmlParser::GetMethodCallResult()
{
	const std::string strRetValue = *m_vecMethodCallResults.begin();
	m_vecMethodCallResults.erase( m_vecMethodCallResults.begin());
	return strRetValue;
}

XmlParserState::XmlParserState()
{
	
}

void XmlParserState::ChangeState( XmlParser* t, XmlParserState* s )
{
	t->ChangeState( s );
}

bool XmlParserState::ParseCurrentLine( XmlParser* p, const std::string& strCurrentLine ) throw( Xml_Parse_Error, Xml_Invalid_Tag )
{
	enum TagState
	{
		TS_DEFAULT,
		TS_OPENTAGNAME,
		TS_CLOSETAGNAME,
	};

	TagState eTagState = TS_DEFAULT;
	
	std::string strTag;
	std::string strAttrib;

	for( size_t i = 0; i < strCurrentLine.length(); i++ )
	{
		switch( strCurrentLine[ i ] )
		{
			case ' ':
			case '\t':
				if( eTagState == TS_DEFAULT )
					strAttrib += strCurrentLine[ i ];
				else
					throw Xml_Parse_Error();
				break;
			case '<':
				if( eTagState == TS_DEFAULT )
				{
					eTagState = TS_OPENTAGNAME;
					trim( strAttrib );
					if( strAttrib.length())
					{
						p->SetTagValue( strAttrib );
						strAttrib = "";
					}
				}
				else
				{
					throw Xml_Parse_Error();
				}
				break;
			case '/':
				if( i > 0 && strCurrentLine[ i - 1 ] == '<' && eTagState == TS_OPENTAGNAME )
					eTagState = TS_CLOSETAGNAME;
				else if( eTagState == TS_DEFAULT )
					strAttrib += strCurrentLine[ i ];
				else
					throw Xml_Parse_Error();
				break;
			case '>':
				if( eTagState != TS_OPENTAGNAME && eTagState != TS_CLOSETAGNAME )
					throw Xml_Parse_Error();
				
				if( strTag.length() == 0 )
					throw Xml_Parse_Error();
				
				if( eTagState == TS_OPENTAGNAME )
					p->InitTag( strTag );
				else
					p->CloseTag( strTag );
				eTagState = TS_DEFAULT;
				strTag = "";

				// stackTypes.push( eTagState );
				break;
			default:
			{
				switch( eTagState )
				{
					case TS_DEFAULT:
						strAttrib += strCurrentLine[ i ];
						break;
					case TS_OPENTAGNAME:
					case TS_CLOSETAGNAME:
						strTag += strCurrentLine[ i ];
						break;
				}
			}
		}
		
	}
	
	trim( strTag );
	trim( strAttrib );
	
	if( eTagState == TS_DEFAULT && strTag.length() == 0 && strAttrib.length() == 0 )
		return true;
	throw Xml_Parse_Error();
	return false;
}

std::string* XmlParserState::ParseXmlHeader_attrib( const std::string& strCurrentLine, const std::string& strAttrName )  throw( Xml_Illegal_Header )
{
	size_t iPos = strCurrentLine.find( strAttrName );
	if( iPos != std::string::npos )
	{
		iPos += strAttrName.length();
		std::string strXmlVersion;
		while( iPos < strCurrentLine.length() && strCurrentLine[ iPos ] != '"' )
			strXmlVersion += strCurrentLine[ iPos++ ];
		if( iPos == strCurrentLine.length() /*|| strCurrentLine[ iPos ] != '"'*/ )
			throw Xml_Illegal_Header();
		return new std::string( strXmlVersion );
	}
	return 0;
}

bool XmlParserState::ParseXmlHeader( XmlParser* t, const std::string& strCurrentLine )  throw( Xml_Illegal_Header )
{
	if( strCurrentLine.find( "<?xml " ) != 0 )
		return false;
	
	if( strCurrentLine.find( "?>" ) != strCurrentLine.length() - 2 )
		return false;
	
	// version="1.0"
	// encoding="iso-8859-1"
	std::auto_ptr<std::string> pAttrib( ParseXmlHeader_attrib( strCurrentLine, " version=\"" ));
	if( pAttrib.get() )
		t->SetXmlVersion( *pAttrib );
	
	pAttrib = std::auto_ptr<std::string>( ParseXmlHeader_attrib( strCurrentLine, " encoding=\"" ));
	if( pAttrib.get() )
		t->SetXmlEncoding( *pAttrib );
	return true;
}

void XmlParserState::SetCurrentLine( XmlParser* p, const std::string& strCurrentLine )
{
	p->SetCurrentLine( strCurrentLine );
}

const std::string& XmlParserState::GetCurrentLine( XmlParser* p ) const
{
	return p->GetCurrentLine();
}

/*
 * *****************************************
 * ***********XmlParserClosed***************
 * *****************************************
*/
XmlParserState* XmlParserClosed::Instance()
{
	if( !m_pState )
		m_pState = new XmlParserClosed();
	return m_pState;
}

void XmlParserClosed::Destroy()
{
	if( m_pState )
		delete m_pState;
	m_pState = 0;
}

XmlParserClosed::XmlParserClosed()
{

}

bool XmlParserClosed::Open( XmlParser* p, CharStream& stream )
{
	std::string strCurrentLine( GetCurrentLine( p ));
	bool bBreakFor = false;
	for( ;!bBreakFor && stream.length() > 0; ++stream )
	{
		switch( stream.get())
		{
			case '\r':
				break;
			case '\n':
				if( ParseXmlHeader( p, strCurrentLine ))
				{
					strCurrentLine = "";
					bBreakFor = true;
				}
				else
				{
					return false;
				}
				break;
			default:
				strCurrentLine += stream.get();
		}
	}
	
	SetCurrentLine( p, strCurrentLine );
	ChangeState( p, XmlParserOpened::Instance());
	return true;
}

/*
 * *****************************************
 * ***********XmlParserOpened***************
 * *****************************************
 */
XmlParserState* XmlParserOpened::Instance()
{
	if( !m_pState )
		m_pState = new XmlParserOpened();
	return m_pState;
}

void XmlParserOpened::Destroy()
{
	if( m_pState )
		delete m_pState;
	m_pState = 0;
}

XmlParserOpened::XmlParserOpened()
{

}

bool XmlParserOpened::Read( XmlParser* p, CharStream& stream )
{
	std::string strCurrentLine( GetCurrentLine( p ));
	for( ;stream.length() > 0; ++stream )
	{
		switch( stream.get())
		{
			case '\r':
				break;
			case '\n':
				if( ParseCurrentLine( p, strCurrentLine ))
					strCurrentLine = "";
				else
					return false;
				break;
			default:
				strCurrentLine += stream.get();
		}
	}
	
	SetCurrentLine( p, strCurrentLine );
	ChangeState( p, XmlParserPaused::Instance());
	return true;
}
void XmlParserOpened::Close( XmlParser* p )
{
	// todo close
	ChangeState( p, XmlParserClosed::Instance());
}

/*
 * *****************************************
 * ***********XmlParserPaused***************
 * *****************************************
 */
XmlParserState* XmlParserPaused::Instance()
{
	if( !m_pState )
		m_pState = new XmlParserPaused();
	return m_pState;
}

void XmlParserPaused::Destroy()
{
	if( m_pState )
		delete m_pState;
	m_pState = 0;
}

XmlParserPaused::XmlParserPaused()
{
}

bool XmlParserPaused::Read( XmlParser*, CharStream& stream )
{
	return false;
}

void XmlParserPaused::Close( XmlParser* p )
{
	// todo
	// ...
	
	ChangeState( p, XmlParserClosed::Instance());
}
	

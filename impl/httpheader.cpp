#include "httpheader.h"
#include "util.h"

/*
POST /servertest.php HTTP/1.0
User_Agent: My Egg Client
Host: localhost
Content-Type: text/xml
Content-Length: 185

*/

// GET / HTTP/1.1
const std::string HttpHeader::m_strIllegalKey;

using namespace std;

HttpHeader::HttpHeader( CharStream& stream )
{
	string strName;
	string strBuffer;
	
	bool bName = true;
	bool bFirstLine = true;
	string strFirstLine;
	bool bFinished = false;
			
	for( ;stream.length() > 0 && !bFinished; ++stream )
	{
		switch( stream.get() )
		{
			case ':':
				if( bFirstLine )
				{
					strBuffer += stream.get();
					continue;
				}
				if( bName )
				{
					strName = strBuffer;
					trim( strName );
					strBuffer = "";
					bName = !bName;
				}
				else
				{
					strBuffer += stream.get();
				}
				break;
			case '\r':
				continue;
			case '\n':
				if( bFirstLine )
				{
					bFirstLine = false;
					strFirstLine = strBuffer;
					strBuffer = "";
					continue;
				}
				if( !bName )
					m_mapValues.insert( pair<const string, const string>( strName, trim( strBuffer )));
				else if( strBuffer.length() == 0 )
					bFinished = true;
				strBuffer = "";
				bName = true;
				break;
			default:
				//if( bFirstLine )
				//	continue;
				strBuffer += stream.get();
		}
	}

	/*
	if( bFinished )
	{
		
		string ContentLength;
		values_t::const_iterator it = m_mapValues.find( "Content-Length");
		if( it != m_mapValues.end())
			ContentLength = it->second;
	}
*/
}

ssize_t HttpHeader::ContentLength() const
{
	values_t::const_iterator it = m_mapValues.find( "Content-Length" );
	ssize_t iHLength = -1;
	if( it != m_mapValues.end())
	{
		try
		{
			iHLength = std::stoi( it->second );
		}
		catch( std::invalid_argument )
		{
		}
		catch( std::out_of_range )
		{
		}
	}
	
	return iHLength;
}

const std::string& HttpHeader::UserAgent() const
{
	values_t::const_iterator it = m_mapValues.find( "Content-Length" );
	if( it != m_mapValues.end())
		return it->second;
	
	return m_strIllegalKey;
}

const std::string& HttpHeader::Host() const
{
	values_t::const_iterator it = m_mapValues.find( "Content-Length" );
	if( it != m_mapValues.end())
		return it->second;
	
	return m_strIllegalKey;
}

const std::string& HttpHeader::ContentType() const
{
	values_t::const_iterator it = m_mapValues.find( "Content-Length" );
	if( it != m_mapValues.end())
		return it->second;
	
	return m_strIllegalKey;
}

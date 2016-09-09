#include "method.h"
#include <iostream>
#include "objserver.h"


Method::Method()
{
	m_bFinished = false;
	m_pParameters = 0;
}

Method::~Method()
{
	DeleteParameters();
}

void Method::SetName( const std::string& strName )
{
	m_strName = strName;
}

void Method::SetParameters( std::vector<MethodParameter*>* pParameters )
{
	DeleteParameters();
	m_pParameters = pParameters;
}

void Method::Finished()
{
	m_bFinished = true;
	//DumpParameters();
}

void Method::DumpParameters()
{
	for( unsigned int i = 0; i < m_pParameters->size(); i++ )
		std::cout << std::string( *(*m_pParameters)[ i ] ) << "\n";
}

void Method::DeleteParameters()
{
	if( !m_pParameters )
		return;
	
	for( unsigned int i = 0; i < m_pParameters->size(); i++ )
		delete (*m_pParameters)[ i ];
	delete m_pParameters;
	m_pParameters = 0;
}

std::string Method::DoCall()
{
	try
	{
		if( !IsFinished())
			throw Method_Error();
		
		return ObjServer::Instance()->Call( m_strName, *m_pParameters );
	}
	catch( Method_Error )
	{
	}

	return std::string( 
		"<?xml version=\"1.0\"?>\n"
		"<methodResponse>\n"
		"<fault>\n"
		"	<value>\n"
		"	<struct>\n"
		"		<member>\n"
		"		<name>faultCode</name>\n"
		"		<value><string>" ) + m_strName + "</string></value>\n"
		"		</member>\n"
		"		<member>\n"
		"		<name>faultString</name>\n"
		"		<value><string>Method execution error!</string></value>\n"
		"		</member>\n"
		"	</struct>\n"
		"	</value>\n"
		"</fault>\n"
		"</methodResponse>\n";
}

bool Method::IsFinished()
{
	return m_bFinished;
}

MethodReturnValue::~MethodReturnValue()
{
	if( m_pstrRetValue )
		delete m_pstrRetValue;
}

MethodReturnValue::MethodReturnValue( int iValue )
{
	m_pstrRetValue = new std::stringstream;
	*m_pstrRetValue << 
		" <param>\n" <<
		" <array>\n" <<
		" <data>\n" <<
		"  <value>\n" <<
		"   <int>" << iValue << "</int>\n" <<
		"  </value>\n" <<
		"  </data>\n" <<
		"  </array>\n" <<
		" </param>\n";
}

MethodReturnValue::MethodReturnValue( double dValue )
{
	m_pstrRetValue = new std::stringstream;
	*m_pstrRetValue << 
		" <param>\n" <<
		" <array>\n" <<
		" <data>\n" <<
		"  <value>\n" <<
		"   <double>" << dValue << "</double>\n" <<
		"  </value>\n" <<
		"  </data>\n" <<
		"  </array>\n" <<
		" </param>\n";
}

MethodReturnValue::MethodReturnValue( bool bValue )
{
	m_pstrRetValue = new std::stringstream;
	*m_pstrRetValue << 
		" <param>\n" <<
		" <array>\n" <<
		" <data>\n" <<
		"  <value>\n" <<
		"   <boolean>" << (bValue ? 1 : 0) << "</boolean>\n" <<
		"  </value>\n" <<
		"  </data>\n" <<
		"  </array>\n" <<
		" </param>\n";
}

MethodReturnValue::MethodReturnValue( std::string strValue )
{
	m_pstrRetValue = new std::stringstream;
	*m_pstrRetValue << 
		" <param>\n" <<
		" <array>\n" <<
		" <data>\n" <<
		"  <value>\n" <<
		"   <string>" << strValue << "</string>\n" <<
		"  </value>\n" <<
		"  </data>\n" <<
		"  </array>\n" <<
		" </param>\n";
}

MethodReturnValue::MethodReturnValue( MethodParameter* pParameter, bool bDestroy /*= false*/ )
{
	m_pstrRetValue = new std::stringstream;
	switch( pParameter->GetType())
	{
	case MethodParameter::MPT_ARRAY:
	case MethodParameter::MPT_STRUCT:
			*m_pstrRetValue << 
				" <param>\n" <<
				" <array>\n" <<
				" <data>\n" <<
				"  <value>\n" <<
					pParameter->toXmlTag() <<
				"  </value>\n" <<
				"  </data>\n" <<
				"  </array>\n" <<
				" </param>\n";
		break;
	case MethodParameter::MPT_BOOLEAN:
	case MethodParameter::MPT_DOUBLE:
	case MethodParameter::MPT_INT:
	case MethodParameter::MPT_STRING:
			*m_pstrRetValue << pParameter->toXmlTag();
		break;
	case MethodParameter::MPT_MEMBER:
		throw Invalid_Return_Error();
		break;
	}
	if( bDestroy )
		delete pParameter;
}

MethodReturnValue::MethodReturnValue( std::vector<MethodParameter*>* pParameters, bool bDestroy /*= false*/ )
{
	m_pstrRetValue = new std::stringstream;
	*m_pstrRetValue << "<param>\n<array>\n<data>\n";
	for( unsigned int i = 0; i < pParameters->size(); i++ )
		*m_pstrRetValue << "<value>\n" << (*pParameters)[ i ]->toXmlTag() << "</value>\n";
	*m_pstrRetValue << "</data>\n</array>\n</param>\n";

	for( unsigned int i = 0; i < pParameters->size(); i++ )
		delete (*pParameters)[ i ];
	delete pParameters;
}

MethodReturnValue::operator const std::string()
{
	std::string strResponse = std::string( "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n" ) +
		"<methodResponse>\n" +
		"<params>\n" + 
			m_pstrRetValue->str() + 
		"</params>\n" +
		"</methodResponse>\n";

	return strResponse;
}

MethodReturnValue::MethodReturnValue( const MethodReturnValue& o )
{
	if( this == &o )
		return;
	if( m_pstrRetValue )
		delete m_pstrRetValue;
	m_pstrRetValue = o.m_pstrRetValue;
	o.m_pstrRetValue = 0;
}


#include "methodparameter.h"
#include <iostream>

MethodParameter::MethodParameter()
{
}

MethodParameter::~MethodParameter()
{
}

MethodParameter_Array::MethodParameter_Array()
{
	m_eType = MPT_ARRAY;
}

void MethodParameter_Array::AddItem( MethodParameter* pItem )
{
	m_vecItems.push_back( pItem );
}

MethodParameter_Array::operator const std::string() const
{
	std::string strRetValue;
	for( unsigned int i = 0; i < m_vecItems.size(); i++ )
		strRetValue += *m_vecItems[ i ];
	return "array: { " + strRetValue + "}; ";
}

const std::string MethodParameter_Array::toXmlTag() const
{
	std::string strRetValue;
	for( unsigned int i = 0; i < m_vecItems.size(); i++ )
		strRetValue += "<value>\n" + m_vecItems[ i ]->toXmlTag() + "</value>\n";
	return "<array>\n<data>\n" + strRetValue + "</data>\n</array>\n";
}

MethodParameter* MethodParameter_Array::operator[]( int iPos ) const
{
	return m_vecItems[ iPos ];
}

unsigned int MethodParameter_Array::size() const
{
	return m_vecItems.size();
}

MethodParameter_Array::~MethodParameter_Array()
{
	for( unsigned int i = 0; i < m_vecItems.size(); i++ )
		delete m_vecItems[ i ];
}

MethodParameter_Boolean::MethodParameter_Boolean( bool bValue )
{
	m_eType = MPT_BOOLEAN;
	m_bValue = bValue;
}

MethodParameter_Boolean::operator const std::string() const
{
	return std::string( "boolean: " ) + ( m_bValue ? "TRUE" : "FALSE" ) + "; ";
}

const std::string MethodParameter_Boolean::toXmlTag() const
{
	return std::string( "<boolean>" ) + ( m_bValue ? "1" : "0" ) + "</boolean>\n";
}

const bool MethodParameter_Boolean::BooleanValue() const
{
	return m_bValue;
}

MethodParameter_Double::MethodParameter_Double( double dValue )
{
	m_eType = MPT_DOUBLE;
	m_dValue = dValue;
}

MethodParameter_Double::operator const std::string() const
{
	return std::string( "double: " ) + std::to_string( (long double)m_dValue ) + "; ";
}

const std::string MethodParameter_Double::toXmlTag() const
{
	return std::string( "<double>" ) + std::to_string( (long double)m_dValue ) + "</double>";
}

const double MethodParameter_Double::DoubleValue()  const
{
	return m_dValue;
}

MethodParameter_Int::MethodParameter_Int( int iValue )
{
	m_eType = MPT_INT;
	m_iValue = iValue;
}

MethodParameter_Int::operator const std::string() const
{
	return std::string( "int: " ) + std::to_string( (long long)m_iValue ) + "; ";
}

const std::string MethodParameter_Int::toXmlTag() const
{
	return std::string( "<int>" ) + std::to_string( (long long)m_iValue ) + "</int>";
}

const int MethodParameter_Int::IntValue() const
{
	return m_iValue;
}

MethodParameter_String::MethodParameter_String( const std::string& strValue )
{
	m_eType = MPT_STRING;
	m_strValue = strValue;
}

MethodParameter_String::operator const std::string() const
{
	return std::string( "string: \"" ) + m_strValue + "\"; ";
}

const std::string MethodParameter_String::toXmlTag() const
{
	return std::string( "<string>" ) + m_strValue + "</string>";
}

const std::string MethodParameter_String::StringValue() const
{
	return m_strValue;
}

MethodParameter_Struct::MethodParameter_Struct()
{
	m_eType = MPT_STRUCT;
}

MethodParameter_Struct::operator const std::string() const
{
	std::string strRetValue;
	for( unsigned int i = 0; i < m_vecMembers.size(); i++ )
		strRetValue += *m_vecMembers[ i ];
	return "struct: { " + strRetValue + "}; ";
}

const std::string MethodParameter_Struct::toXmlTag() const
{
	std::string strRetValue;
	for( unsigned int i = 0; i < m_vecMembers.size(); i++ )
		strRetValue += m_vecMembers[ i ]->toXmlTag();
	return "<struct>\n" + strRetValue + "</struct>";
}

const MethodParameter* const MethodParameter_Struct::Member( const std::string& strName ) const
{
	for( unsigned int i = 0; i < m_vecMembers.size(); i++ )
	{
		if( m_vecMembers[ i ]->GetName() == strName )
			return m_vecMembers[ i ]->GetValue();
	}
	throw Illegal_Member_Name();
	return 0;
}

void MethodParameter_Struct::SetMember( MethodParameter* pMember )
{
	m_vecMembers.push_back( pMember );
}

MethodParameter_Struct::~MethodParameter_Struct()
{
	for( unsigned int i = 0; i < m_vecMembers.size(); i++ )
		delete m_vecMembers[ i ];
}

MethodParameter* MethodParameter_Struct::operator[]( int iPos ) const
{
	return m_vecMembers[ iPos ];
}

unsigned int MethodParameter_Struct::size() const
{
	return m_vecMembers.size();
}

MethodParameter_Member::MethodParameter_Member()
{
	m_eType = MPT_MEMBER;
	m_pValue = 0;
}

void MethodParameter_Member::SetName( const std::string& strName )
{
	m_strName = strName;
}

void MethodParameter_Member::SetValue( MethodParameter* pValue )
{
	m_pValue = pValue;
}

MethodParameter_Member::operator const std::string() const
{
	return std::string( "member: { name: " ) + m_strName + "; value: { " + std::string( *m_pValue ) + "}}; ";
}

const std::string MethodParameter_Member::toXmlTag() const
{
	return std::string( "<member>\n<name>" ) + m_strName + "</name>\n<value>" + m_pValue->toXmlTag() + "</value>\n</member>\n";
}

const MethodParameter* const  MethodParameter_Member::GetValue() const
{
	return m_pValue;
}

const std::string& MethodParameter_Member::GetName() const
{
	return m_strName;
}

MethodParameter_Member::~MethodParameter_Member()
{
	if( m_pValue )
		delete m_pValue;
}

namespace std
{
	std::ostream& operator<<( std::ostream& o, const MethodParameter_Struct& p )
	{
		o << (const std::string)p;
		return o;
	}
	
	std::ostream& operator<<( std::ostream& o, const MethodParameter_String& p )
	{
		o << (const std::string)p;
		return o;
	}
	
	std::ostream& operator<<( std::ostream& o, const MethodParameter_Int& p )
	{
		o << (const std::string)p;
		return o;
	}
	
	std::ostream& operator<<( std::ostream& o, const MethodParameter_Double& p )
	{
		o << (const std::string)p;
		return o;
	}
	
	std::ostream& operator<<( std::ostream& o, const MethodParameter_Boolean& p )
	{
		o << (const std::string)p;
		return o;
	}
	
	std::ostream& operator<<( std::ostream& o, const MethodParameter_Array& p )
	{
		o << (const std::string)p;
		return o;
	}
	
}

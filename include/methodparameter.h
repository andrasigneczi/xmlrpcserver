/* 
 * File:   methodparameter.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 22., 17:28
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef METHODPARAMETER_H
#define	METHODPARAMETER_H

#include <string>
#include <vector>
#include <sstream>

/*
 **************************************
 ********* Method exceptions **********
 **************************************
*/
class Method_Error {};
class Illegal_Method_Call  : public Method_Error {};
class Type_Mismatch_Error  : public Method_Error {};
class Illegal_Member_Name  : public Method_Error {};
class UnKnown_Method_Error : public Method_Error {};
class Invalid_Return_Error : public Method_Error {};

/*
 ************************************
 ********* MethodParameter **********
 ************************************
*/
class MethodParameter
{
public:
	enum Type
	{
		MPT_ARRAY,
		MPT_BOOLEAN,
		MPT_DOUBLE,
		MPT_INT,
		MPT_STRING,
		MPT_STRUCT,
		MPT_MEMBER
	};

	virtual ~MethodParameter();
	virtual void SetName( const std::string& strName )     { throw Illegal_Method_Call(); }
	virtual void SetValue( bool )                          { throw Illegal_Method_Call(); }
	virtual void SetValue( double )                        { throw Illegal_Method_Call(); }
	virtual void SetValue( int )                           { throw Illegal_Method_Call(); }
	virtual void SetValue( const std::string )             { throw Illegal_Method_Call(); }
	virtual void SetValue( MethodParameter* pMemberValue ) { throw Illegal_Method_Call(); }
	virtual void SetMember( MethodParameter* pMember )     { throw Illegal_Method_Call(); }
	virtual void AddItem( MethodParameter* pItem )         { throw Illegal_Method_Call(); }

	virtual operator const std::string() const = 0;
	virtual const std::string toXmlTag() const = 0;
	
	virtual const int    IntValue()     const { throw Type_Mismatch_Error(); }
	virtual const double DoubleValue()  const { throw Type_Mismatch_Error(); }
	virtual const bool   BooleanValue() const { throw Type_Mismatch_Error(); }
	
	virtual const std::string StringValue() const { throw Type_Mismatch_Error(); }

	//virtual const std::vector<MethodParameter*>& ArrayItems() const { throw Type_Mismatch_Error(); }
	// MethodParameter* operator[]( int ) { throw Type_Mismatch_Error(); } // for array
	virtual const MethodParameter* const At( int ) const { throw Type_Mismatch_Error(); } // for array
	virtual unsigned int Size() const { throw Type_Mismatch_Error(); } // for array
	virtual const MethodParameter* const Member( const std::string& strName ) const { throw Type_Mismatch_Error(); }      // for struct

	virtual const MethodParameter* const GetValue() const { throw Type_Mismatch_Error(); }      // for member

	virtual const Type GetType() const;         
	virtual const std::string& GetName() const { throw Type_Mismatch_Error(); } // for member
	virtual unsigned int size() const { throw Type_Mismatch_Error(); } // for array
	virtual MethodParameter* operator[]( int ) const { throw Type_Mismatch_Error(); } // for array

protected:
	MethodParameter();
	Type m_eType;            // just for debug
};

/*
 ******************************************
 ********* MethodParameter_Array **********
 ******************************************
*/
class MethodParameter_Array : public MethodParameter
{
public:
	MethodParameter_Array();
	~MethodParameter_Array();
	void AddItem( MethodParameter* pItem );
	operator const std::string() const;
	const std::string toXmlTag() const;
	MethodParameter* operator[]( int ) const;
	unsigned int size() const;
	
private:
	std::vector<MethodParameter*> m_vecItems;
};

/*
 ********************************************
 ********* MethodParameter_Boolean **********
 ********************************************
*/
class MethodParameter_Boolean : public MethodParameter
{
public:
	MethodParameter_Boolean( bool bValue );
	operator const std::string() const;
	const std::string toXmlTag() const;
	const bool BooleanValue() const;
	
private:
	bool m_bValue;
};

/*
 ********************************************
 ********* MethodParameter_Double ***********
 ********************************************
*/
class MethodParameter_Double : public MethodParameter
{
public:
	MethodParameter_Double( double dValue );
	operator const std::string() const;
	const std::string toXmlTag() const;
	const double DoubleValue() const;
	
private:
	double m_dValue;
};

/*
 ********************************************
 ********* MethodParameter_Int **************
 ********************************************
*/
class MethodParameter_Int : public MethodParameter
{
public:
	MethodParameter_Int( int iValue );
	operator const std::string() const;
	const std::string toXmlTag() const;
	const int IntValue() const;
	
private:
	int m_iValue;
};

/*
 ***********************************************
 ********* MethodParameter_String **************
 ***********************************************
*/
class MethodParameter_String : public MethodParameter
{
public:
	MethodParameter_String( const std::string& strValue );
	operator const std::string() const;
	const std::string toXmlTag() const;
	const std::string StringValue() const;
	
private:
	std::string m_strValue;
};

/*
 ***********************************************
 ********* MethodParameter_Struct **************
 ***********************************************
*/
class MethodParameter_Struct : public MethodParameter
{
public:
	MethodParameter_Struct();
	~MethodParameter_Struct();
	void SetMember( MethodParameter* pMember );
	operator const std::string() const;
	const std::string toXmlTag() const;
	const MethodParameter* const Member( const std::string& strName ) const;
	MethodParameter* operator[]( int ) const;
	unsigned int size() const;
	
private:
	//std::map<std::string, MethodParameter*> m_mapMembers;
	std::vector<MethodParameter*> m_vecMembers;
};

/*
 ***********************************************
 ********* MethodParameter_Member **************
 ***********************************************
*/
class MethodParameter_Member : public MethodParameter
{
public:
	MethodParameter_Member();
	~MethodParameter_Member();
	void SetName( const std::string& strName );
	void SetValue( MethodParameter* pValue );
	operator const std::string() const;
	const std::string toXmlTag() const;
	const MethodParameter* const GetValue() const ;
	const std::string& GetName() const;
	
private:
	MethodParameter* m_pValue;
	std::string m_strName;
};

namespace std
{
	std::ostream& operator<<( std::ostream&, const MethodParameter_Struct& );
	std::ostream& operator<<( std::ostream&, const MethodParameter_String& );
	std::ostream& operator<<( std::ostream&, const MethodParameter_Int& );
	std::ostream& operator<<( std::ostream&, const MethodParameter_Double& );
	std::ostream& operator<<( std::ostream&, const MethodParameter_Boolean& );
	std::ostream& operator<<( std::ostream&, const MethodParameter_Array& );
}

inline const MethodParameter::Type MethodParameter::GetType() const
{
	return m_eType;
}

#endif // METHODPARAMETER_H

#include "testserverobjects.h"
#include <iostream>
#include <stdexcept>


MethodReturnValue TestServerObject1::Add( const std::vector<MethodParameter*>& vecParameters )
{
	std::cout << "TestServerObject::Add\n";
	std::cout << "Parameters: \n";
	for( unsigned int i = 0; i < vecParameters.size(); i++ )
		std::cout << (const std::string)*vecParameters[ i ] << "\n";
	try
	{
		int i1 = vecParameters.at( 0 )->IntValue();
		int i2 = vecParameters.at( 1 )->IntValue();

		return MethodReturnValue( i1 + i2 );
	}
	catch( std::out_of_range )
	{
		throw Method_Error();
	}
}

MethodReturnValue TestServerObject2::DuplicateParameter( const std::vector<MethodParameter*>& vecParameters )
{
	std::cout << "TestServerObject::Add\n";
	std::cout << "Parameters: \n";
	for( unsigned int i = 0; i < vecParameters.size(); i++ )
		std::cout << (const std::string)*vecParameters[ i ] << "\n";

	try
	{
		std::vector<MethodParameter*>* pvecRetValue = new std::vector<MethodParameter*>;
		MethodParameter* pArray1 = new MethodParameter_Array();
		MethodParameter* pArray2 = new MethodParameter_Array();
		pvecRetValue->push_back( pArray1 );
		pvecRetValue->push_back( pArray2 );

		for( unsigned int i = 0; i < vecParameters.size(); i++ )
		{
			pArray1->AddItem( CopyItem( vecParameters[ i ], false ));
			pArray2->AddItem( CopyItem( vecParameters[ i ], true ));
		}

		MethodParameter* pInt = new MethodParameter_Int( -10 );
		MethodParameter* pDouble = new MethodParameter_Double( -20.22 );
		MethodParameter* pString = new MethodParameter_String( "haliho" );
		MethodParameter* pStruct = new MethodParameter_Struct();
		MethodParameter* pMember = new MethodParameter_Member();
		pMember->SetName( "Test Name" );
		MethodParameter* pMemberValue = new MethodParameter_String( "Test Imre" );
		pMember->SetValue( pMemberValue );
		pStruct->SetMember( pMember );
		MethodParameter* pBoolean = new MethodParameter_Boolean( false );
		
		pvecRetValue->push_back( pInt );
		pvecRetValue->push_back( pDouble );
		pvecRetValue->push_back( pString );
		pvecRetValue->push_back( pStruct );
		pvecRetValue->push_back( pBoolean );
		return MethodReturnValue( pvecRetValue, true );
	}
	catch( std::out_of_range )
	{
		throw Method_Error();
	}
}

MethodParameter* TestServerObject2::CopyArray( const MethodParameter* pArray, bool bDuplicate )
{
	MethodParameter_Array* pRet = new MethodParameter_Array();
	
	for( unsigned int i = 0; i < pArray->size(); i++ )
		pRet->AddItem( CopyItem( (*pArray)[ i ], bDuplicate ));

	return pRet;
}

MethodParameter* TestServerObject2::CopyStruct( const MethodParameter* pStruct, bool bDuplicate )
{
	MethodParameter_Struct* pRet = new MethodParameter_Struct();
	
	for( unsigned int i = 0; i < pStruct->size(); i++ )
		pRet->SetMember( CopyItem( (*pStruct)[ i ], bDuplicate ));

	return pRet;
}

MethodParameter* TestServerObject2::CopyItem( const MethodParameter* pItem, bool bDuplicate )
{
	switch( pItem->GetType() )
	{
	case MethodParameter::MPT_INT:
		if( !bDuplicate )
			return new MethodParameter_Int( pItem->IntValue() );
		else
			return new MethodParameter_Int( 2 * pItem->IntValue() );
		break;
	case MethodParameter::MPT_DOUBLE:
		if( !bDuplicate )
			return new MethodParameter_Double( pItem->DoubleValue() );
		else
			return new MethodParameter_Double( 2 * pItem->DoubleValue() );
		break;
	case MethodParameter::MPT_BOOLEAN:
		if( !bDuplicate )
			return new MethodParameter_Boolean( pItem->BooleanValue() );
		else
			return new MethodParameter_Boolean( pItem->BooleanValue() );
		break;
	case MethodParameter::MPT_STRING:
		if( !bDuplicate )
			return new MethodParameter_String( pItem->StringValue() );
		else
			return new MethodParameter_String( pItem->StringValue() + pItem->StringValue() );
		break;
	case MethodParameter::MPT_ARRAY:
		return CopyArray( pItem, bDuplicate );
		break;
	case MethodParameter::MPT_STRUCT:
		return CopyStruct( pItem, bDuplicate );
		break;
	case MethodParameter::MPT_MEMBER:
		{
			MethodParameter_Member * pM = new MethodParameter_Member();
			pM->SetName( pItem->GetName());
			pM->SetValue( CopyItem( pItem->GetValue(), bDuplicate ));
			return pM;
		}
		break;
	}
	throw Method_Error();
}


#include "methodbuilder.h"
#include "method.h"
#include <stdexcept>
#include <iostream>

MethodBuilder::MethodBuilder()
{
	m_pMethod = 0;
	m_pParameters = 0;
	const char* acValidNodeTypes[] = { "methodCall", "methodName", "params", "param", "value", "i4",
										"array", "data", "base64", "boolean", "datetime", "double",
										"int", "string", "struct", "member", "name", 0x0 };

	for( int i = 0; acValidNodeTypes[ i ]; i++ )
		m_mapValidNodeTypes.insert( std::pair<std::string, NodeType>( acValidNodeTypes[ i ], (NodeType)i ) );
}

MethodBuilder::~MethodBuilder()
{
	if( m_pParameters )
		delete m_pParameters;
	if( m_pMethod )
		delete m_pMethod;
}

bool MethodBuilder::InitNode(  const std::string& strName )  // e.g. methodName
{
	if( !m_pMethod )
	{
		if( strName != "methodCall" )
			return false;
		m_pMethod = new Method();
		m_pParameters = new std::vector<MethodParameter*>();
		return true;
	}

	ValidParamsIterator itResult = m_mapValidNodeTypes.find( strName );
	if( itResult == m_mapValidNodeTypes.end())
		return false;
	if( !CreateNode( itResult->second ))
		return false;
	return true;
}

bool MethodBuilder::SetNodeValue(  const std::string& strValue ) // e.g. testObj.F9
{
	if( !m_pMethod || m_stackNodes.empty())
		return false;

	Node* pNode = m_stackNodes.top();

	switch( pNode->m_eType )
	{
		case NT_NAME:
		case NT_STRING:
		case NT_I4:
		case NT_INT:
		case NT_DOUBLE:
		case NT_BOOLEAN:
		case NT_METHODNAME:
			break;
		default:
			return false;
	}
	pNode->m_strValue = strValue;
	return true;
}

bool MethodBuilder::MethodReadyToCall() const
{
	if( m_pMethod && m_pMethod->IsFinished())
		return true;
	return false;
}

Method* MethodBuilder::ReleaseMethod()
{
	if( !MethodReadyToCall())
		return 0;
	
	Method* pRetValue = m_pMethod;
	m_pMethod = 0;
	return pRetValue;
}

bool MethodBuilder::CloseNode( const std::string& strName )  // e.g. methodName
{
	if( m_pMethod && m_stackNodes.empty() && strName == "methodCall" )
	{
		m_pMethod->Finished();
		return true;
	}
	
	if( !m_pMethod || m_stackNodes.empty())
		return false;

	ValidParamsIterator itResult = m_mapValidNodeTypes.find( strName );
	if( itResult == m_mapValidNodeTypes.end())
		return false;
	
	Node* pNode = m_stackNodes.top();
	if( pNode->m_eType != itResult->second )
		return false;
	
	switch( pNode->m_eType )
	{
		case NT_METHODCALL:
			// not used here
			break;
		case NT_METHODNAME:
			m_pMethod->SetName( pNode->m_strValue );
			delete pNode;
			m_stackNodes.pop();
			break;
		case NT_PARAMS: // parent: root
			m_pMethod->SetParameters( m_pParameters );
			m_pParameters = 0;
			delete pNode;
			m_stackNodes.pop();
			break;
		case NT_PARAM: // parent: params
			{
				try
				{
					MethodParameter* pParameter = CreateParameter( pNode );
					m_pParameters->push_back( pParameter );
				}
				catch( std::invalid_argument )
                {
					return false;
                }
                catch( std::out_of_range )
				{
					return false;
				}
				m_stackNodes.pop();
			}
			break;
		case NT_VALUE: // parent: param, data, member
		case NT_BOOLEAN:
		case NT_I4:
		case NT_DOUBLE:
		case NT_INT:
		case NT_STRING:
		case NT_STRUCT:
		case NT_ARRAY:
		case NT_DATA:
		case NT_BASE64: // not implemented
		case NT_DATETIME: // not implemented
		case NT_MEMBER:
		case NT_NAME:
			m_stackNodes.pop();
			break;
	}
	return true;
}

bool MethodBuilder::CheckHierarchyRules( NodeType eChild )
{
	// check the hierarchy rules
	switch( eChild )
	{
		case NT_METHODCALL:
			return false;
			break;
		case NT_METHODNAME:
		case NT_PARAMS: // parent: root
			if( !m_stackNodes.empty() || !m_pParameters->empty())
				return false;
			break;
		case NT_PARAM: // parent: params
			if( m_stackNodes.size() != 1 || m_stackNodes.top()->m_eType != NT_PARAMS )
				return false;
			break;
		case NT_VALUE: // parent: param, data, member
			{
				if( m_stackNodes.size() < 2 )
					return false;
				NodeType eType = m_stackNodes.top()->m_eType;
				if( eType != NT_PARAM && eType != NT_DATA && eType != NT_MEMBER )
					return false;
			}
			break;
		case NT_BOOLEAN:
		case NT_I4:
		case NT_DOUBLE:
		case NT_INT:
		case NT_STRING:
		case NT_STRUCT:
			{
				if( m_stackNodes.size() < 3 ) // parent: value
					return false;
				NodeType eType = m_stackNodes.top()->m_eType;
				if( eType != NT_VALUE )
					return false;
			}
			break;
		case NT_ARRAY:
			{
				if( m_stackNodes.size() < 3 ) // parent: value
					return false;
				NodeType eType = m_stackNodes.top()->m_eType;
				if( eType != NT_VALUE )
					return false;
			}
			break;
		case NT_DATA:
			{
				if( m_stackNodes.size() < 4 ) // parent: array
					return false;
				NodeType eType = m_stackNodes.top()->m_eType;
				if( eType != NT_ARRAY )
					return false;
			}
			break;
		case NT_BASE64: // not implemented
			return false;
			break;
		case NT_DATETIME: // not implemented
			return false;
			break;
		case NT_MEMBER:
			{
				if( m_stackNodes.size() < 4 ) // parent: struct
					return false;
				NodeType eType = m_stackNodes.top()->m_eType;
				if( eType != NT_STRUCT )
					return false;
			}
			break;
		case NT_NAME:
			{
				if( m_stackNodes.size() < 5 ) // parent: member
					return false;
				NodeType eType = m_stackNodes.top()->m_eType;
				if( eType != NT_MEMBER )
					return false;
			}
			break;
	}
	return true;
}

MethodBuilder::Node* MethodBuilder::CreateNode( NodeType eChild )
{
	if( !CheckHierarchyRules( eChild ))
		return 0;

	Node* pNode = new Node;
	pNode->m_eType = eChild;

	if( !m_stackNodes.empty())
		m_stackNodes.top()->m_vecChildren.push_back( pNode );
	m_stackNodes.push( pNode );
	return pNode;
}

MethodParameter* MethodBuilder::Walk( Node* pNode, MethodParameter* pParent /*= 0*/ )
{
	switch( pNode->m_eType )
	{
		case NT_I4:
			return new MethodParameter_Int( std::stoi( pNode->m_strValue ));
			break;
		case NT_ARRAY:
			{
				MethodParameter_Array* pArray = new MethodParameter_Array();
				for( unsigned i = 0; i < pNode->m_vecChildren.size(); i++ )
				{
					/*MethodParameter* pChild =*/ Walk( pNode->m_vecChildren[ i ], pArray );
				}
				return pArray;
			}
			break;
		case NT_BOOLEAN:
			return new MethodParameter_Boolean( std::stoi( pNode->m_strValue ) != 0 );
			break;
		case NT_DOUBLE:
			return new MethodParameter_Double( std::stod( pNode->m_strValue ));
			break;
		case NT_INT:
			return new MethodParameter_Int( std::stoi( pNode->m_strValue ));
			break;
		case NT_STRING:
			return new MethodParameter_String( pNode->m_strValue );
			break;
		case NT_STRUCT:
			{
				MethodParameter_Struct* pStruct = new MethodParameter_Struct();
				for( unsigned i = 0; i < pNode->m_vecChildren.size(); i++ )
				{
					MethodParameter* pChild = Walk( pNode->m_vecChildren[ i ], pStruct );
					pStruct->SetMember( pChild );
				}
				return pStruct;
			}
			break;
		case NT_MEMBER:
			{
				MethodParameter_Member* pMember = new MethodParameter_Member();
				for( unsigned i = 0; i < pNode->m_vecChildren.size(); i++ )
				{
					MethodParameter* pChild = Walk( pNode->m_vecChildren[ i ], pMember );
					if( pChild )
						pMember->SetValue( pChild );
				}
				return pMember;
			}
			break;
		case NT_NAME:
			pParent->SetName( pNode->m_strValue );
			break;
		case NT_DATA:
			for( unsigned i = 0; i < pNode->m_vecChildren.size(); i++ )
			{
				MethodParameter* pChild = Walk( pNode->m_vecChildren[ i ] );
				pParent->AddItem( pChild );
			}
			break;
		default:
			for( unsigned i = 0; i < pNode->m_vecChildren.size(); i++ )
			{
				MethodParameter* pChild = Walk( pNode->m_vecChildren[ i ], pParent );
				if( pChild )
					return pChild;
			}
			return 0;
			break;
	}
	return 0;
}

MethodParameter* MethodBuilder::CreateParameter( Node* pNode )
{
	MethodParameter* pParameter = Walk( pNode );
	return pParameter;
}

MethodBuilder::Node::~Node()
{
	
	for( unsigned i = 0; i < m_vecChildren.size(); i++ )
		delete m_vecChildren[ i ];
}

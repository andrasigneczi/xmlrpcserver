/* 
 * File:   methodbuilder.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 18., 14:15
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef METHODBUILDER_H
#define	METHODBUILDER_H

#include <string>
#include <vector>
#include <stack>
#include <map>

class Method;
class MethodParameter;

/*
 **********************************
 ********* MethodBuilder **********
 **********************************
*/
class MethodBuilder
{
public:
	MethodBuilder();
	~MethodBuilder();
	bool InitNode(     const std::string& strName );  // e.g. methodName
	bool SetNodeValue( const std::string& strValue ); // e.g. testObj.F9
	bool CloseNode(    const std::string& strName );  // e.g. methodName
	bool MethodReadyToCall() const;
	Method* ReleaseMethod();
	
private:
	enum NodeType
	{
		NT_METHODCALL = 0,
		NT_METHODNAME,
		NT_PARAMS,
		NT_PARAM,
		NT_VALUE,
		NT_I4,
		NT_ARRAY,
		NT_DATA,
		NT_BASE64,
		NT_BOOLEAN,
		NT_DATETIME,
		NT_DOUBLE,
		NT_INT,
		NT_STRING,
		NT_STRUCT,
		NT_MEMBER,
		NT_NAME
	};
	
	struct Node
	{
		~Node();
		std::vector<Node*> m_vecChildren;
		NodeType           m_eType;
		std::string        m_strValue;
	};

	bool  CheckHierarchyRules( NodeType eChild );
	Node* CreateNode( NodeType eChild );
	MethodParameter* Walk( Node* pNode, MethodParameter* pParent = 0 );
	MethodParameter* CreateParameter( Node* pNode );
	
private:
	Method*                         m_pMethod;
	std::vector<MethodParameter*>*  m_pParameters;
	std::stack<Node*>               m_stackNodes;
	std::map<std::string, NodeType> m_mapValidNodeTypes;

	typedef std::map<std::string, NodeType>::const_iterator ValidParamsIterator;
};

#endif	/* METHODBUILDER_H */


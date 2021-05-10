#pragma once

/** \brief basic xml manipulation class
 *
 */
class ZXML
{
	IXMLDOMDocument*	m_pXMLDoc;

public:
							ZXML();
							ZXML( LPCTSTR url );
							~ZXML();

	int						Open( WCHAR* url );
	int						Close();
	IXMLDOMNodeList*		FindElement( WCHAR* strElement );
	IXMLDOMNodeList*		FindElement( IXMLDOMNode* pNode, WCHAR* strElement );
	int						GetElementText( IXMLDOMNode* pNode, WCHAR* strRet );
	int						GetAttributeText( IXMLDOMNode* pNode, WCHAR* strAttrName, WCHAR* strRet );
};
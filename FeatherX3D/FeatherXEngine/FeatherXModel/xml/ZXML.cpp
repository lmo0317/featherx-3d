#include "FeatherXEnginePrecompile.h"
#include <fcntl.h>

ZXML::ZXML()
{
	m_pXMLDoc = NULL;
	CoInitialize(NULL);
	CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_ALL, IID_IXMLDOMDocument, (void**)&m_pXMLDoc);
}

ZXML::ZXML( LPCTSTR url )
{
}

ZXML::~ZXML()
{
	CoUninitialize();
}

int ZXML::Open( WCHAR* url )
{
	HRESULT	hr;
	BOOL bRet = TRUE;

	try
	{
		FILE* fp;
		fp = _wfopen( url, L"rt" );
		if( !fp ) return FALSE;
		fclose( fp );

		short sResult = FALSE;

		m_pXMLDoc->put_async(FALSE);

		_bstr_t varString = (LPCTSTR)url;
		VARIANT path;
		path.vt = VT_BSTR;
		path.bstrVal = varString;

		hr = m_pXMLDoc->load(path, &sResult);
		if(FAILED(hr)) return FALSE;
	}
	catch (...)
	{
		m_pXMLDoc->Release();
		m_pXMLDoc = NULL;
		bRet = FALSE;
	}

	return bRet;
}

int ZXML::Close()
{
	m_pXMLDoc->Release();
	return 1;
}

IXMLDOMNodeList* ZXML::FindElement( WCHAR* strElement )
{
	IXMLDOMNodeList* pNodeList = NULL;
	
	if(m_pXMLDoc == NULL)
		return pNodeList;

	try 
	{
		_bstr_t bstrPath = strElement;
		m_pXMLDoc->selectNodes(bstrPath, &pNodeList);
	}
	catch(...)
	{
	}

	return pNodeList;
}

IXMLDOMNodeList* ZXML::FindElement( IXMLDOMNode* pNode, WCHAR* strElement )
{
	IXMLDOMNodeList* pNodeList = NULL;
	
	if(pNode == NULL)
		return pNodeList;

	try 
	{
		_bstr_t bstrPath = strElement;
		pNode->selectNodes(bstrPath, &pNodeList);
	}
	catch(...)
	{
	}

	return pNodeList;
}

int	ZXML::GetElementText( IXMLDOMNode* pNode, WCHAR* strRet )
{
	BSTR			bstr = NULL;
	pNode->get_text( &bstr );
	wcscpy( strRet,(const WCHAR*)_bstr_t(bstr,FALSE) );
	return 1;
}

int ZXML::GetAttributeText( IXMLDOMNode* pNode, WCHAR* strAttrName, WCHAR* strRet )
{
	wchar_t					wstrAttr[128];
	IXMLDOMNode*			pAttrNode = NULL;
	IXMLDOMNamedNodeMap*	pMap = NULL;
	VARIANT					varValue;

	try
	{ 
		//int n = mbstowcs( wstrAttr, strAttrName, 128 );
		wcscpy(wstrAttr,strAttrName);
		pNode->get_attributes( &pMap );
		pMap->getNamedItem( wstrAttr, &pAttrNode );
		pAttrNode->get_nodeValue( &varValue );
		wcscpy( strRet,(const WCHAR*)_bstr_t(varValue.bstrVal,FALSE) );
		S_REL( pAttrNode );
		S_REL( pMap );
		return TRUE;
	}
	catch(...)
	{
		S_REL( pAttrNode );
		S_REL( pMap );
		return FALSE;
	}
}

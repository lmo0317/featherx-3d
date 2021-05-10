#include "FeatherXEnginePrecompile.h"

CFeatherXXml::CFeatherXXml()
{
	m_pXMLDoc = NULL;
	CoInitialize(NULL);
	CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_ALL, IID_IXMLDOMDocument, (void**)&m_pXMLDoc);
}

CFeatherXXml::~CFeatherXXml()
{
	CoUninitialize();
}

int CFeatherXXml::Open( WCHAR* url )
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

int CFeatherXXml::Close()
{
	m_pXMLDoc->Release();
	return 1;
}

IXMLDOMNodeList* CFeatherXXml::FindElement( WCHAR* strElement )
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

IXMLDOMNodeList* CFeatherXXml::FindElement( IXMLDOMNode* pNode, WCHAR* strElement )
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

int	CFeatherXXml::GetElementText( IXMLDOMNode* pNode, WCHAR* strRet )
{
	BSTR bstr = NULL;

	if(!pNode)
		return 0;

	pNode->get_text( &bstr );
	wcscpy( strRet,(const WCHAR*)_bstr_t(bstr,FALSE) );

	return 1;
}

int CFeatherXXml::GetAttributeText( IXMLDOMNode* pNode, WCHAR* strAttrName, WCHAR* strRet )
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

		SAFE_RELEASE( pAttrNode );
		SAFE_RELEASE( pMap );

		return TRUE;
	}
	catch(...)
	{
		SAFE_RELEASE( pAttrNode );
		SAFE_RELEASE( pMap );
		return FALSE;
	}
}

void CFeatherXXml::GetElementText(WCHAR* strElement,WCHAR* strRet)
{
	IXMLDOMNodeList* pList = NULL;
	IXMLDOMNode* pNode = NULL;

	wcscpy(strRet,L"");
	pList = FindElement( strElement );	
	pList->get_item( 0, &pNode );	
	GetElementText( pNode, strRet);
}

F3dVector2 CFeatherXXml::_StrToVector2f( WCHAR str[] )
{
	F3dVector2	val(0,0);

	if(_wcsicmp(str,L"") == 0)
		return val;

	WCHAR*		sep = L" [],\n";
	val.x = (float)_wtof( wcstok( str, sep ) );
	val.y = (float)_wtof( wcstok( NULL, sep ) );
	return val;
}

F3dVector CFeatherXXml::_StrToVector3f( WCHAR str[] )
{
	F3dVector val(0,0,0);

	if(_wcsicmp(str,L"") == 0)
		return val;

	WCHAR*		sep = L" [],\n";
	val.x = (float)_wtof( wcstok( str, sep ) );
	val.y = (float)_wtof( wcstok( NULL, sep ) );
	val.z = (float)_wtof( wcstok( NULL, sep ) );
	return val;
}

F3dVector4 CFeatherXXml::_StrToVector4f( WCHAR str[] )
{
	F3dVector4	val(0,0,0,0);

	if(_wcsicmp(str,L"") == 0)
		return val;

	WCHAR*		sep = L" [],\n";
	val.x = (float)_wtof( wcstok( str, sep ) );
	val.y = (float)_wtof( wcstok( NULL, sep ) );
	val.z = (float)_wtof( wcstok( NULL, sep ) );
	val.w = (float)_wtof( wcstok( NULL, sep ) );
	return val;
}


#pragma once

class CFeatherXXml 
{
	IXMLDOMDocument*	m_pXMLDoc;

public:
	CFeatherXXml();
	~CFeatherXXml();

	int	Open( WCHAR* url );
	int Close();
	IXMLDOMNodeList* FindElement( WCHAR* strElement );
	IXMLDOMNodeList* FindElement( IXMLDOMNode* pNode, WCHAR* strElement );
	int GetElementText( IXMLDOMNode* pNode, WCHAR* strRet );
	int GetAttributeText( IXMLDOMNode* pNode, WCHAR* strAttrName, WCHAR* strRet );
	void GetElementText(WCHAR* strElement,WCHAR* strRet);

	F3dVector2 _StrToVector2f( WCHAR str[] );	/// ���ڿ��� Vector2f�� ����
	F3dVector _StrToVector3f( WCHAR str[] );	/// ���ڿ��� Vector3f�� ����
	F3dVector4 _StrToVector4f( WCHAR str[] );	/// ���ڿ��� Vector4f�� ����
};
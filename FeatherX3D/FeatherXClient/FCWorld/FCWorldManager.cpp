#include "FeatherXClientPrecompile.h"

FCWorldManager::FCWorldManager()
{

}

FCWorldManager::~FCWorldManager()
{

}

void FCWorldManager::Process(double dTime,double dt)
{
	m_pWorld->Process(dTime,dt);
}

void FCWorldManager::Render(double dTime,double dt)
{
	m_pWorld->Render(dTime,dt);
}

void FCWorldManager::CreateWorld(WCHAR* strFile)
{
	MAP_INFO mapInfo;
	MAP_INFO_ELEMENT mapElement;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;
	IXMLDOMNodeList*  pNodeList  = NULL;
	WCHAR str[128];
	long nCount = 0;

	//File name
	CFeatherXXml* pXml = CFeatherXXmlParser::GetSingleton()->GetXML();
	pXml->Open(strFile);
	pListChild = pXml->FindElement(L"Stage/Object");
	pListChild->get_length( &nCount );

	for(int i=0;i<nCount;++i)
	{
		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"File");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		mapElement.strFileName=str;

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Type");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		mapElement.type = SelectType(str);

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Position");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		mapElement.vecPostion=pXml->_StrToVector3f(str);

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Rotation");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		mapElement.vecRotation=pXml->_StrToVector3f(str);

		if(mapElement.type == TYPE_EVENT_BOX)
		{
			pListChild->get_item( i, &pNodeChild );
			pNodeList = pXml->FindElement(pNodeChild,L"EVENT_BOX_ID");
			pNodeList->get_item( 0, &pNodeChild );
			pXml->GetElementText( pNodeChild, str );
			mapElement.nEventBoxID = _wtoi(str);
		}

		mapInfo.vecElement.push_back(mapElement);
	}

	m_vecMapInfo.push_back(mapInfo);
	m_pWorld = new FCWorld;
	m_pWorld->Init(mapInfo);
}

int FCWorldManager::SelectType(WCHAR* strType)
{
	if(wcsicmp(strType,L"box") == 0)
	{
		return TYPE_BOX;
	}
	else if (wcsicmp(strType,L"wall_x") == 0)
	{
		return TYPE_WALL_X;
	}
	else if (wcsicmp(strType,L"wall_z") == 0)
	{
		return TYPE_WALL_Z;
	}
	else if (wcsicmp(strType,L"effect") == 0)
	{
		return TYPE_EFFECT;
	}
	else if( wcsicmp(strType,L"event_box") == 0)
	{
		return TYPE_EVENT_BOX;
	}
}
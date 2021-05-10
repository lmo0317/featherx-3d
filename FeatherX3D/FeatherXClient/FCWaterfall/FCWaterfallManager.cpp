#include "FeatherXClientPrecompile.h"

void FCWaterfallManager::Init(WCHAR* strFile)
{
	WATERFALL_INFO		wf_Info;
	WATERFALL_ELEMENT	wfElement;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;
	IXMLDOMNodeList*	pNodeList  = NULL;
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
		wfElement.strFileName=str;

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Time");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		wfElement.nTime = _wtoi(str);

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Position");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		wfElement.vecPostion=pXml->_StrToVector3f(str);

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Rotation");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		wfElement.vecRotation=pXml->_StrToVector3f(str);

		wf_Info.vecElement.push_back(wfElement);
	}
	CreateWaterfall(wf_Info);
}

void FCWaterfallManager::CreateWaterfall(WATERFALL_INFO &wf_Info)
{
	/*
	F3dCWorld* pWorld = F3dCWorldManager::GetSingleton()->GetWorld();
	F3dMatrix matWorld;
	F3dMatrix matRot;
	Identity(matWorld);
	Identity(matRot);

	for(auto it=wf_Info.vecElement.begin(); it!=wf_Info.vecElement.end(); ++it)
	{
		::SetPos(matWorld,it->vecPostion);
		::RotateEuler(matRot,it->vecRotation);
		matWorld = matRot * matWorld;
		pWorld->CreateObject((char*)it->strFileName.c_str(),matWorld);	
	}
	*/
}
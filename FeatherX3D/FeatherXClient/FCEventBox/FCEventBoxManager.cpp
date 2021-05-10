#include "FeatherXClientPrecompile.h"

FCEventBoxManager::FCEventBoxManager()
{

}

FCEventBoxManager::~FCEventBoxManager()
{

}

void FCEventBoxManager::CreateEventBox(WCHAR* strModel,const int nID,F3dVector& vecPos,F3dVector& vecRot)
{
	auto it = m_hashEventBoxInfo.find(nID);
	if(it == m_hashEventBoxInfo.end())
		return;

	_EVENT_BOX_INFO_* pEventBoxInfo = it->second;
	FCEventBox* pEventBox = NULL;
	switch(pEventBoxInfo->nType)
	{
	case _EVENT_BOX_JUMP_:
		pEventBox = new FCEventBoxJump;
		pEventBox->setJump(pEventBoxInfo->nJump);
		break;
	case _EVENT_BOX_START_:
		pEventBox = new FCEventBoxStart;
		break;
	case _EVENT_BOX_END_:
		pEventBox = new FCEventBoxEnd;
		break;
	}

	pEventBox->Init(strModel,vecPos,vecRot);
	m_vecEventBox.push_back(pEventBox);
}

void FCEventBoxManager::Init(WCHAR* strFile)
{
	WCHAR str[128];
	long nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;
	IXMLDOMNodeList*  pNodeList  = NULL;

	CFeatherXXml* pXml = CFeatherXXmlParser::GetSingleton()->GetXML();
	pXml->Open(strFile);

	//effect
	pListChild = pXml->FindElement(L"Body/Info/EVENT_BOX");
	pListChild->get_length( &nCount );

	for(int i=0;i<nCount;++i)
	{
		_EVENT_BOX_INFO_* eventBox = new _EVENT_BOX_INFO_;
		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"ID");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		eventBox->nID = _wtoi(str);

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"TYPE");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		eventBox->nType = getType(str);

		switch(eventBox->nType)
		{
		case _EVENT_BOX_JUMP_:

			pListChild->get_item( i, &pNodeChild );
			pNodeList = pXml->FindElement(pNodeChild,L"JUMP");
			pNodeList->get_item( 0, &pNodeChild );
			pXml->GetElementText( pNodeChild, str );
			eventBox->nJump = _wtoi(str);
			break;
		}

		m_hashEventBoxInfo.insert(std::pair<int,_EVENT_BOX_INFO_*>(eventBox->nID,eventBox));
	}
}

void FCEventBoxManager::Process(double dTime,double dt)
{

}

void FCEventBoxManager::Render(double dTime,double dt)
{

}

void FCEventBoxManager::Destroy()
{

}

F3dVector FCEventBoxManager::GetStartBoxPosition()
{
	for(auto it = m_vecEventBox.begin();it != m_vecEventBox.end(); ++it)
	{
		FCEventBox* pEventBox = (*it);
		if(pEventBox->getType() == _EVENT_BOX_START_)
		{
			return pEventBox->getPos();
		}
	}

	return F3dVector(0,0,0);
}

int FCEventBoxManager::getType(WCHAR* str)
{
	if(wcsicmp(str,L"EVENT_BOX_JUMP") == 0)
	{
		return _EVENT_BOX_JUMP_;
	}
	else if(wcsicmp(str,L"EVENT_BOX_START") == 0)
	{
		return _EVENT_BOX_START_;
	}
	else if(wcsicmp(str,L"EVENT_BOX_END") == 0)
	{
		return _EVENT_BOX_END_;
	}
}
#include "FeatherXClientPrecompile.h"
#include <iostream>
#include <functional>
#include <algorithm>

FCEventBox::FCEventBox():
m_pModel(NULL)
{

}

FCEventBox::~FCEventBox()
{

}

void FCEventBox::Init(WCHAR* strModel,F3dVector& vecPos,F3dVector& vecRot)
{
	m_pModel = CFeatherXModelManager::GetSingleton()->CreateModel(strModel);
	ContactCB_ptr ContactCB = std::bind1st( std::mem_fun(&FCEventBox::CB_Contact),this);
	m_pModel->SetCB_Contact(ContactCB);
}
void FCEventBox::Process()
{

}
void FCEventBox::Render()
{

}
void FCEventBox::Destroy()
{

}

void FCEventBox::CB_Contact(NxActor **actors)
{

}

F3dVector FCEventBox::getPos()
{
	return ::GetPos(m_pModel->GetWorldTransform());
}
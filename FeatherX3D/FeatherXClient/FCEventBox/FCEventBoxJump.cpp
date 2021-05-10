#include "FeatherXClientPrecompile.h"

FCEventBoxJump::FCEventBoxJump():
m_nJump(0)
{

}

FCEventBoxJump::~FCEventBoxJump()
{

}

void FCEventBoxJump::Init(WCHAR* strModel,F3dVector& vecPos,F3dVector& vecRot)
{
	FCEventBox::Init(strModel,vecPos,vecRot);
}

void FCEventBoxJump::Process()
{

	FCEventBox::Process();
}
void FCEventBoxJump::Render()
{

	FCEventBox::Render();
}
void FCEventBoxJump::Destroy()
{

	FCEventBox::Destroy();
}

void FCEventBoxJump::CB_Contact(NxActor **actors)
{
	FCCharacterPlayer* pPlayer = FCCharacterManager::GetSingleton()->GetPlayer();
	F3dCModel *pModel = pPlayer->GetModel();

	for(int i=0;i<2;i++)
	{
		if(pModel->IsMyActor((int)actors[i]->userData))
		{
			pModel->addForce(F3dVector(0,m_nJump,0));
		}
	}

	FCEventBox::CB_Contact(actors);
}
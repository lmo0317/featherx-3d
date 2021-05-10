#include "FeatherXClientPrecompile.h"

FCEventBoxEnd::FCEventBoxEnd()
{

}

FCEventBoxEnd::~FCEventBoxEnd()
{

}

void FCEventBoxEnd::CB_Contact(NxActor **actors)
{
	FCCharacterPlayer* pPlayer = FCCharacterManager::GetSingleton()->GetPlayer();
	F3dCModel *pModel = pPlayer->GetModel();

	for(int i=0;i<2;i++)
	{
		if(pModel->IsMyActor((int)actors[i]->userData))
		{
			//게임 종료 이벤트
			//pModel->addForce(F3dVector(0,100,0));
			FCGameMain* pGameMain = CFeatherXClientManager::GetSingleton()->GetGameMain();
			pGameMain->GameEnd();
		}
	}

	FCEventBox::CB_Contact(actors);
}
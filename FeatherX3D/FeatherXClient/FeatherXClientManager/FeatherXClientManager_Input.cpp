#include "FeatherXClientPrecompile.h"

bool CFeatherXClientManager::ProcessInput()
{
	CFeatherXInput::GetSingleton()->Update();
	/************************************************************************/
	/* MOUSE                                                                */
	/************************************************************************/

	int dx = 0,dy = 0,wheel = 0;
	CFeatherXInput::GetSingleton()->GetMouseMovement(dx,dy);

	wheel = CFeatherXInput::GetSingleton()->GetMouseWheelMovement();
	CFeatherXCamera::GetSingleton()->ZoomCamera(-(wheel/100));

	if(CFeatherXInput::GetSingleton()->ButtonDown(RBUTTON))
	{
		CFeatherXCamera::GetSingleton()->RotateCamera(dx/5.0f,dy/5.0f);
	}

	/************************************************************************/
	/* KEYBOARD                                                             */
	/************************************************************************/
	if(m_pGameMain->IsGameStart() == false)
		return false;

	FCCharacterPlayer* pPlayer = FCCharacterManager::GetSingleton()->GetPlayer();
	F3dCModel* pModel = pPlayer->GetModel();
	float steering = 0;
	float acceleration = 0;
	bool handbrake = false;
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_UP))
	{
		pPlayer->ForWard();
	}	
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_DOWN))
	{
		pPlayer->BackWard();
	}
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_LEFT))
	{
		pPlayer->Left();
	}	
	else if(CFeatherXInput::GetSingleton()->KeyDown(DIK_RIGHT))
	{
		pPlayer->Right();
	}
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_SPACE))
	{
		pPlayer->Break();
	}

	CFeatherXCamera::GetSingleton()->SetUpCamera();
	return true;
}
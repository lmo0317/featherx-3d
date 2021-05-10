#include "FeatherXWindowPrecompile.h"

CSceneGame::CSceneGame()
{
}

CSceneGame::~CSceneGame()
{
}

void CSceneGame::InitCamera()
{
	/************************************************************************/
	/* ZOOM THETA PHI                                                       */
	/************************************************************************/
	CFeatherXCamera::GetSingleton()->SetZoom(10.0f);
	CFeatherXCamera::GetSingleton()->SetTheta(180.0f);
	CFeatherXCamera::GetSingleton()->SetPhi(15.0f);

	/************************************************************************/
	/* MIN MAX SET                                                          */
	/************************************************************************/
	CFeatherXCamera::GetSingleton()->SetMinPhi(-80.0f);
	CFeatherXCamera::GetSingleton()->SetMaxPhi(90.0f);
	CFeatherXCamera::GetSingleton()->SetMinZoom(5.0f);
	CFeatherXCamera::GetSingleton()->SetMaxZoom(100000.0f);
	CFeatherXCamera::GetSingleton()->SetUpCamera();
}

void CSceneGame::CreateWorld()
{
	F3dCWorldManager::GetSingleton()->CreateWorld("world/stage1.xml");
	//CreateLoad();

	//CreateWall(F3dVector(-3,15,40),"models\\box_wall.xml");
	CreateWall(F3dVector(1,25,40),"models\\box_wall.xml");
}

void CSceneGame::CreateLoad()
{
	//CreateBox(F3dVector(0,15,0),F3dVector(0,0,0),"models\box_wall.xml");
}

void CSceneGame::CreateWall(F3dVector& pos,char* modelFile)
{
	for(int i=0;i<4;++i) {
		for(int j=0;j<5;++j) {
			F3dVector vtxPos = F3dVector(pos.x + (j*1.5),pos.y + (i*2),pos.z);
			F3dCModel* pModel = CreateBox(vtxPos,F3dVector(0,0,0),modelFile);
		}
	}
}

F3dCModel* CSceneGame::CreateBox(F3dVector& pos,F3dVector& vecRot,char* modelFile)
{
	F3dCWorld* pWorld = F3dCWorldManager::GetSingleton()->GetMainWorld();
	F3dMatrix matWorld;
	F3dMatrix matRot;
	Identity(matWorld);
	Identity(matRot);
	::SetPos(matWorld,pos);
	::RotateEuler(matRot,vecRot);
	matWorld = matRot * matWorld;

	return pWorld->CreateObject(modelFile,matWorld);
}

bool CSceneGame::InitScene(CFeatherXEngineManager*	FeatherXEngineManager)
{
	m_FeatherXEngineManager = FeatherXEngineManager;
	CreateWorld();

	m_pPlayer = new FCPlayer;
	m_pPlayer->Init(F3dVector(0,22,0));
	InitCamera();
	return true;
}

bool CSceneGame::Process()
{
	m_FeatherXEngineManager->Process();	
	m_pPlayer->Process();
	F3dVector vecPos = m_pPlayer->GetModel()->GetPos();
	CFeatherXCamera::GetSingleton()->SetTarget(vecPos);
	F3dVector vecDir = m_pPlayer->GetModel()->GetDir();
	float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir );
	CFeatherXCamera::GetSingleton()->SetTheta(fAngle + 90);
	return true;
}

bool CSceneGame::Render()
{
	m_FeatherXEngineManager->Render();
	return true;
}

bool CSceneGame::ProcessInput()
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

	F3dCModel* pModel = m_pPlayer->GetModel();
	float steering = 0;
	float acceleration = 0;
	bool handbrake = false;
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_UP))
	{
		F3dVector vecDir = pModel->GetDir();
		pModel->setAngularVelocity(vecDir * 10);
	}	
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_DOWN))
	{
		F3dVector vecDir = pModel->GetDir();
		pModel->setAngularVelocity(-vecDir * 10);
	}
	if(CFeatherXInput::GetSingleton()->KeyDown(DIK_LEFT))
	{
		F3dVector vecDir = pModel->GetDir();
		F3dMatrix mat;
		
		float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir );
		float fRadian = ToRadian(fAngle - 1);

		vecDir.x = sinf( fRadian );
		vecDir.z = cosf( fRadian );

		pModel->SetDir(vecDir);
	}	
	else if(CFeatherXInput::GetSingleton()->KeyDown(DIK_RIGHT))
	{
		F3dVector vecDir = pModel->GetDir();
		F3dMatrix mat;

		float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir );
		float fRadian = ToRadian(fAngle + 1);

		vecDir.x = sinf( fRadian );
		vecDir.z = cosf( fRadian );

		pModel->SetDir(vecDir);
	}
	/************************************************************************/
	/* OpenCV                                                               */
	/************************************************************************/
	float fCVAngle = 0;	
	if(fCVAngle = CFeatherXOpenCVManager::GetSingleton()->GetRotate())
	{
		F3dVector vecDir = pModel->GetDir();
		F3dMatrix mat;
		float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir );
		float fRadian = ToRadian(fAngle + (fCVAngle * 2));
		vecDir.x = sinf( fRadian );
		vecDir.z = cosf( fRadian );
		pModel->SetDir(vecDir);
	}
	CFeatherXCamera::GetSingleton()->SetUpCamera();
	return true;
}
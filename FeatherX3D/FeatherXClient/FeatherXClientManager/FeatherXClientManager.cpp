#include "FeatherXClientPrecompile.h"

CFeatherXClientManager::CFeatherXClientManager():
m_pTimer(NULL),
m_pGameMain(NULL),
m_dOldTime(0)
{
	m_pGameMain = new FCGameMain;
}

CFeatherXClientManager::~CFeatherXClientManager()
{
}

void CFeatherXClientManager::Init(HWND hWnd)
{
	Clock::CreateSingleton();
	CFeatherXEngineManager::CreateSingleton();
	FCEventBoxManager::CreateSingleton();
	FCCharacterManager::CreateSingleton();
	FCWorldManager::CreateSingleton();
	FCUIManager::CreateSingleton();
	Clock::GetSingleton()->SetTimeSource( new TimeSourceHRWin32, 1 );
	m_pTimer = new Magi::Timer(Clock::GetSingleton());

	CFeatherXEngineManager::GetSingleton()->StartEngine(hWnd);
	FCUIManager::GetSingleton()->Init();
	FCCharacterManager::GetSingleton()->Init();
	FCEventBoxManager::GetSingleton()->Init(L"xml/event_box.xml");
	FCWorldManager::GetSingleton()->CreateWorld(L"world/stage1.xml");
	//F3dVector vecStartPos = FCEventBoxManager::GetSingleton()->GetStartBoxPosition();
	
	FCCharacterManager::GetSingleton()->CreatePlayer(F3dVector(0,0,0),F3dVector(1,0,0));
	InitCamera();

	m_pGameMain->GameStart();
}

void CFeatherXClientManager::Destroy()
{
	Clock::DestroySingleton();
	FCEventBoxManager::DestroySingleton();
	FCCharacterManager::DestroySingleton();
	FCWorldManager::DestroySingleton();

	CFeatherXEngineManager::GetSingleton()->Destroy();
}

void CFeatherXClientManager::Process()
{
	Clock::GetSingleton()->FrameStep();
	double dNewTime = m_pTimer->GetTime();		//frame skipping
	double dt = dNewTime - m_dOldTime;
	m_dOldTime = dNewTime;
	m_dDT = dt;

	CFeatherXEngineManager::GetSingleton()->Process(dNewTime,dt);	
	FCCharacterManager::GetSingleton()->Process(dNewTime,dt);
	FCEventBoxManager::GetSingleton()->Process(dNewTime,dt);
	FCWorldManager::GetSingleton()->Process(dNewTime,dt);
	FCUIManager::GetSingleton()->Process( dNewTime, dt );
	
	//FCCharacterPlayer* pPlayer = FCCharacterManager::GetSingleton()->GetPlayer();
	//F3dVector vecPos = pPlayer->GetModel()->GetPos();
	//CFeatherXCamera::GetSingleton()->SetTarget(vecPos);
	//F3dVector vecDir = pPlayer->GetModel()->GetDir();
	//float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir );
	//CFeatherXCamera::GetSingleton()->SetTheta(fAngle + 90);

	m_pGameMain->Process(dNewTime,dt);
}

void CFeatherXClientManager::Render()
{
	double dTime = m_pTimer->GetTime();
	FCCharacterManager::GetSingleton()->Render(dTime,m_dDT);
	FCEventBoxManager::GetSingleton()->Render(dTime,m_dDT);
	FCWorldManager::GetSingleton()->Render(dTime,m_dDT);
	FCUIManager::GetSingleton()->Render( dTime, m_dDT );

	CFeatherXEngineManager::GetSingleton()->Render(dTime,m_dDT);

}

void CFeatherXClientManager::InitCamera()
{
	/************************************************************************/
	/* ZOOM THETA PHI                                                       */
	/************************************************************************/
	CFeatherXCamera::GetSingleton()->SetZoom(200.0f);
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


bool CFeatherXClientManager::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( FCUIManager::GetSingleton() && !FCUIManager::GetSingleton()->MsgProc( hWnd, uMsg, wParam, lParam ) )
	{

	}
	return true;
}
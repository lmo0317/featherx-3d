#include "FeatherXWindowPrecompile.h"

CGameMain::CGameMain()
{
}

CGameMain::~CGameMain()
{
}

bool CGameMain::Init(HWND hWnd)
{
	CFeatherXClientManager::CreateSingleton();
	CFeatherXClientManager::GetSingleton()->Init(hWnd);
	return true;
}

bool CGameMain::Destroy()
{
	CFeatherXClientManager::GetSingleton()->Destroy();
	CFeatherXClientManager::DestroySingleton();
	return true;
}

bool CGameMain::Process()
{
	CFeatherXClientManager::GetSingleton()->ProcessInput();
	CFeatherXClientManager::GetSingleton()->Process();
	return true;
}

bool CGameMain::Render()
{
	CFeatherXClientManager::GetSingleton()->Render();
	return true;
}

bool CGameMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CFeatherXClientManager::GetSingleton()->MsgProc(hWnd,uMsg,wParam,lParam);
}

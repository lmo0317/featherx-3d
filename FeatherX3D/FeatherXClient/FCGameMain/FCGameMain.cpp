#include "FeatherXClientPrecompile.h"

FCGameMain::FCGameMain() :
m_dTime(0)
{

}

FCGameMain::~FCGameMain()
{

}

void FCGameMain::Init()
{

}
void FCGameMain::Process(double dTime,double dt)
{
	if(m_bStart == false)
		return;

	m_dTime += dt;
	FCUIGameMainPtr pGameMain = FCUIManager::GetSingleton()->GetUIGameMain();
	pGameMain->SetTime(m_dTime);
}

void FCGameMain::Render(double dTime,double dt)
{

}

void FCGameMain::GameStart()
{
	m_dTime = 0;
	m_bStart = true;
}

void FCGameMain::GameEnd()
{
	m_bStart = false;
	FCUIManager::GetSingleton()->ShowWindow(L"end",TRUE);

	FCCharacterPlayer* pPlayer = FCCharacterManager::GetSingleton()->GetPlayer();
	pPlayer->Stop();
}

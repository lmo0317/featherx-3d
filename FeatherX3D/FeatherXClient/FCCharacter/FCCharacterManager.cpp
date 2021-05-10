#include "FeatherXClientPrecompile.h"

FCCharacterManager::FCCharacterManager()
{

}

FCCharacterManager::~FCCharacterManager()
{

}

void FCCharacterManager::Init()
{
}

void FCCharacterManager::CreatePlayer(F3dVector& vecPos,F3dVector& vecDir)
{
	m_pPlayer = new FCCharacterPlayer;
	m_pPlayer->Init(vecPos,vecDir);
}

void FCCharacterManager::Process(double dTime,double dt)
{
	m_pPlayer->Process(dTime,dt);
}

void FCCharacterManager::Render(double dTime,double dt)
{
	m_pPlayer->Render(dTime,dt);
}

void FCCharacterManager::Destroy()
{

}
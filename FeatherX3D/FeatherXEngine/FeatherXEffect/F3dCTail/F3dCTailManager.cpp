#include "FeatherXEnginePrecompile.h"

F3dCTailManager::F3dCTailManager()
{

}

F3dCTailManager::~F3dCTailManager()
{

}

void F3dCTailManager::Init()
{

}

void F3dCTailManager::Process(double dTime,double dt)
{
	for(auto it = m_vecTail.begin();it != m_vecTail.end();++it)
	{
		(*it)->Process(dTime,dt);
	}
}

void F3dCTailManager::Render(double dTime,double dt)
{
	for(auto it = m_vecTail.begin();it != m_vecTail.end();++it)
	{
		(*it)->Render(dTime,dt);
	}
}

void F3dCTailManager::Destroy()
{
	for(auto it = m_vecTail.begin();it != m_vecTail.end();++it)
	{
		(*it)->Finish();
	}
}

F3dCTail* F3dCTailManager::CreateTail()
{
	F3dCTail* pTail = new F3dCTail;
	m_vecTail.push_back(pTail);

	return pTail;
}
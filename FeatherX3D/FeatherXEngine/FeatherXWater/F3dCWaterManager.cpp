#include "FeatherXEnginePrecompile.h"

F3dCWaterManager::F3dCWaterManager()
{

}

F3dCWaterManager::~F3dCWaterManager()
{

}

void F3dCWaterManager::Init()
{
	F3dCWater* pWater = new F3dCWater;
	pWater->Init();
	m_vecWater.push_back(pWater);
}

void F3dCWaterManager::Destroy()
{
	for(auto it = m_vecWater.begin();it != m_vecWater.end();++it)
	{
		(*it)->Destroy();
	}
}

void F3dCWaterManager::Process()
{
	for(auto it = m_vecWater.begin();it != m_vecWater.end();++it)
	{
		(*it)->Process();
	}
}

void F3dCWaterManager::Render()
{
	for(auto it = m_vecWater.begin();it != m_vecWater.end();++it)
	{
		(*it)->Render();
	}
}
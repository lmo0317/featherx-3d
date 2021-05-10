#include "FeatherXEnginePrecompile.h"

F3dCWorld::F3dCWorld():
m_pWorld(NULL)
{

}

F3dCWorld::~F3dCWorld()
{

}

void F3dCWorld::Init()
{
	m_pWorld = new fcoWorld;
	m_pWorld->InitGeometry();
	mpManager::GetSingleton()->CreateTerrain(m_pWorld);
}

void F3dCWorld::Process()
{

}

void F3dCWorld::Render()
{
	F3dCRenderMgr::GetSingleton()->SetRenderWorld(this);
}

void F3dCWorld::Destroy()
{

}
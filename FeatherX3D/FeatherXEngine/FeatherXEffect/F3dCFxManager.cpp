#include "FeatherXEnginePrecompile.h"

F3dCFxManager::F3dCFxManager()
{
}

F3dCFxManager::~F3dCFxManager()
{

}

F3dCParticle* F3dCFxManager::Create(WCHAR* strFile)
{
	//���� �Է��� �޾Ƽ� ���� ������  Particle �ʱ�ȭ �ؾ���
	F3dCParticle* pParticle = new F3dCParticle;
	pParticle->Init(strFile);
	m_vecFx.push_back(pParticle);

	return pParticle;
}

void F3dCFxManager::Init()
{
}

void F3dCFxManager::Render()
{
	for(auto it = m_vecFx.begin();it != m_vecFx.end();it++)
	{
		F3dCParticle* pParticle = *it;
		pParticle->Render();
	}
}

void F3dCFxManager::Process(double dTime,double dt)
{
	for(auto it = m_vecFx.begin();it != m_vecFx.end();it++)
	{
		F3dCParticle* pParticle = *it;
		pParticle->Process(dTime);
	}	
}

void F3dCFxManager::Destroy()
{

}

void F3dCFxManager::SetPlay()
{
	for(auto it = m_vecFx.begin();it != m_vecFx.end();it++)
	{
		F3dCParticle* pParticle = *it;
		pParticle->SetPlay();
	}
}
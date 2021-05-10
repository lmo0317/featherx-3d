#include "FeatherXEnginePrecompile.h"

CFeatherXModelManager::CFeatherXModelManager() :
m_pParser(NULL)
{
	m_pParser = new ZCParser();
}

CFeatherXModelManager::~CFeatherXModelManager()
{

}

void CFeatherXModelManager::Init()
{
}

void CFeatherXModelManager::Render()
{	
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_FILLMODE,  D3DFILL_SOLID );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );

	for(auto it = m_vecModel.begin();it != m_vecModel.end();++it)
	{
		F3dCModel* pModel = *it;
		pModel->Render();
	}
}

void CFeatherXModelManager::Process()
{
	for(auto it = m_vecModel.begin();it != m_vecModel.end();++it)
	{
		F3dCModel* pModel = *it;
		pModel->Process();
	}
}

void CFeatherXModelManager::Destroy()
{
	for(auto it = m_vecModel.begin();it != m_vecModel.end();++it)
	{
		F3dCModel* pModel = *it;
		pModel->Destroy();
	}

	S_DEL( m_pParser );
}

F3dCModel* CFeatherXModelManager::CreateModel( WCHAR* strFile)
{
	F3dCModel* pModel = new F3dCModel;	
	pModel->Create(strFile);
	m_vecModel.push_back(pModel);
	return pModel;
}
#include "FeatherXEnginePrecompile.h"

CFeatherXShaderManager::CFeatherXShaderManager()
{
}


CFeatherXShaderManager::~CFeatherXShaderManager()
{

}

void CFeatherXShaderManager::Init()
{
	CFeatherXShader *pShader = new CFeatherXShader;	
	D3DVERTEXELEMENT9 VertexDecl[] =
	{
	{ 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	{ 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
	{ 0, 48, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
	{ 0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
	D3DDECL_END()
	};	
	pShader->InitShader(F3dCDevice::GetSingleton()->GetD3DDevice(),_T("FX\\EffectParticleSystem.fx"),VertexDecl);	
	m_vShader.push_back(pShader);
	pShader = new CFeatherXShader;	

	D3DVERTEXELEMENT9 VertexDecl2[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	pShader->InitShader(F3dCDevice::GetSingleton()->GetD3DDevice(),_T("FX\\cook.fx"),VertexDecl2);
	m_vShader.push_back(pShader);
	
	pShader = new CFeatherXShader;
	D3DVERTEXELEMENT9 VertexDecl3[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	pShader->InitShader(F3dCDevice::GetSingleton()->GetD3DDevice(),_T("FX\\cross.fx"),VertexDecl3);
	m_vShader.push_back(pShader);

	CFeatherXShader *pShader2 = new CFeatherXShader;
}

void	CFeatherXShaderManager::Process()
{

}

void	CFeatherXShaderManager::Render()
{

}

void	CFeatherXShaderManager::Destroy()
{

}


CFeatherXShader* CFeatherXShaderManager::GetShader(CString strFileName)
{
	std::vector< CFeatherXShader * >::iterator it = m_vShader.begin();
	for(; it != m_vShader.end();)
	{
		CFeatherXShader* pShader = (*it);
		if( pShader->GetFileName() == strFileName ) {
			return *it;
		}
		++it;
	}
	return NULL;
}
#include "FeatherXEnginePrecompile.h"

CFeatherXShader::CFeatherXShader()
: m_pEffect(NULL)
, m_hTechnique(NULL)
, m_hMatProjection(NULL)
, m_hMatView(NULL)
, m_pDecl(NULL)		
{
	m_hTechnique = NULL;
	m_hMatProjection = NULL;
	m_hMatView = NULL;
	m_hTexture = NULL;
	m_hWVP = NULL;
	m_hLightDir = NULL;
	m_hEyePos = NULL;
	m_hColor = NULL;
}


CFeatherXShader::~CFeatherXShader()
{
	CFeatherXShader::DeleteShader();
}

// --------------------------------------------------------------------------------- //
// InitShader
// --------------------------------------------------------------------------------- //
bool CFeatherXShader::InitShader(LPDIRECT3DDEVICE9 Device, CString  file_name, D3DVERTEXELEMENT9 *VertexDecl)
{
	if ( m_pEffect ) return false;

	if(VertexDecl)
	{
		if (D3DERR_INVALIDCALL == Device->CreateVertexDeclaration( VertexDecl, &m_pDecl ) )
		{
			return false;
		}
	}
	LPD3DXBUFFER pErr = NULL;

#if _DEBUG
	if (FAILED(D3DXCreateEffectFromFile(Device,file_name,NULL,NULL,0,NULL,&m_pEffect,&pErr)))	
	{
		//if(pErr) pErr->Release();

		MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), L"ERROR", MB_OK);
		return false;
	}	
#else
	if (FAILED(D3DXCreateEffectFromFile(Device, file_name,NULL,NULL,D3DXSHADER_SKIPVALIDATION ,NULL,&m_pEffect,&pErr)))	
	{
		//std::string err  = (LPCTSTR)pErr->GetBufferPointer();
		//SAFE_RELEASE(pErr);
		return false;		
	}	
#endif

	if(pErr) pErr->Release();
	m_strFileName = file_name;
	m_hTechnique = m_pEffect->GetTechniqueByName("vs_t");
	m_hMatProjection = m_pEffect->GetParameterByName(NULL,"viewProj");
	m_hMatView = m_pEffect->GetParameterByName(NULL,"view");
	m_hTexture = m_pEffect->GetParameterByName(NULL,"DiffuseTexture");
	m_hWVP = m_pEffect->GetParameterByName(NULL,"mWVP");
	m_hEyePos = m_pEffect->GetParameterByName(NULL,"vEyePos");
	m_hLightDir = m_pEffect->GetParameterByName(NULL,"vLightDir");
	m_hColor = m_pEffect->GetParameterByName(NULL,"vColor");

	return true;
}

// --------------------------------------------------------------------------------- //
// DeleteShader
// --------------------------------------------------------------------------------- //
void	CFeatherXShader::DeleteShader()
{	
	if(m_pDecl) 
	{
		m_pDecl->Release();
		m_pDecl = NULL;
	}
	if(m_pEffect) 
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}
	return;
}
// --------------------------------------------------------------------------------- //
// SetShaderMatrix
// --------------------------------------------------------------------------------- //
bool CFeatherXShader::SetShaderMatrix(LPDIRECT3DDEVICE9 &Device,D3DXMATRIX & matView,D3DXMATRIX & matPro)
{
	if (!m_pEffect) 
		return false;

	if (D3DERR_INVALIDCALL == m_pEffect->SetTechnique(m_hTechnique) )
		return true;

	if ( D3DERR_INVALIDCALL  == m_pEffect->SetMatrix(m_hMatProjection, &matPro) ) 
		return true;
	
	if ( D3DERR_INVALIDCALL == m_pEffect->SetMatrix(m_hMatView, &matView) ) 
		return true;
	
	if ( D3DERR_INVALIDCALL  == Device->SetVertexDeclaration(m_pDecl) ) 
		return true;

	return true;
}
// --------------------------------------------------------------------------------- //
// LostDevice
// --------------------------------------------------------------------------------- //
void CFeatherXShader::LostDevice()
{	
	if ( !m_pEffect ) return;	
	m_pEffect->OnLostDevice();
}

// --------------------------------------------------------------------------------- //
// ResetDevice
// --------------------------------------------------------------------------------- //
bool CFeatherXShader::ResetDevice()
{
	if ( !m_pEffect ) return false;	
	m_pEffect->OnResetDevice();	
	return true;
}

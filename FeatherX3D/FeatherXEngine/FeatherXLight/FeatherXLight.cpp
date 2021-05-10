#include "FeatherXEnginePrecompile.h"

CFeatherXPointLight::CFeatherXPointLight()
{
}

CFeatherXPointLight::~CFeatherXPointLight()
{
}

CFeatherXLight::CFeatherXLight()
{
}
CFeatherXLight::~CFeatherXLight()
{
	vector<CFeatherXPointLight*>::iterator it = m_vPointLights.begin();
	for(;it != m_vPointLights.end();++it)
	{
		SAFE_DELETE(*it);
	}
	vector<CFeatherXSpotLight*>::iterator it2 = m_vSpotLights.begin();
	for(;it2 != m_vSpotLights.end();++it2)
	{
		SAFE_DELETE(*it2);
	}
}

void	CFeatherXLight::Init()
{

}

void	CFeatherXLight::Process()
{

}

void	CFeatherXLight::Render()
{

}

void    CFeatherXLight::Destroy()
{

}

bool CFeatherXLight::SetupLights()
{	
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 0.3f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 0.3f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.3f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 0.3f;

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetMaterial(&mtrl);
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3( 100, 100, 0 );

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	light.Position.x = 5000.0f;
	light.Position.y = 5000.0f;
	light.Position.z = 1000.0f;

	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );	/// 광원의 방향을 단위벡터로 만든다.
	light.Range       = 7300.0f;									/// 광원이 다다를수 있는 최대거리

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetLight(0, &light);
	F3dCDevice::GetSingleton()->GetD3DDevice()->LightEnable(0,TRUE);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );			/// 광원설정을 켠다
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );		/// 환경광원(ambient light)의 값 설정
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_SPECULARENABLE, true);

	return true;
}
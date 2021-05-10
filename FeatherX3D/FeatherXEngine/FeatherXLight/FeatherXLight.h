#pragma once

using namespace std;
class CFeatherXSpotLight
{
public:
	short m_nLightNum;

	CFeatherXSpotLight() 
	{ 
	}
	~CFeatherXSpotLight()
	{
	}
};

class CFeatherXPointLight
{
public:
	short m_nLightNum;

	CFeatherXPointLight();
	~CFeatherXPointLight();
};

class CFeatherXLight : public FBSingleton< CFeatherXLight >
{
public:
	CFeatherXLight();
	~CFeatherXLight();

	vector<CFeatherXSpotLight*>		m_vSpotLights;
	vector<CFeatherXPointLight*>	m_vPointLights;

	void	Init();
	void	Process();
	void	Render();
	void    Destroy();

	bool SetupLights();
	inline void EnableLight() { F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_LIGHTING,TRUE ); }
	inline void DisableLight() { F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_LIGHTING,FALSE); }
private:
};
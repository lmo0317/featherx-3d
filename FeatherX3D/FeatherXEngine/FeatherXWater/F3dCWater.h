#pragma once

struct Vertex
{
	float x, y, z;
	DWORD color;
	float tu, tv;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
	};

	void Set(float _x, float _y, float _z, DWORD _color, float _tu, float _tv)
	{
		x = _x;		y = _y;		z = _z;
		color = _color;
		tu = _tu;	tv = _tv;
	}
};

struct Option_Shader_Water
{
	Option_Shader_Water():
	AlphaLayer(true),
	_using(false),
	textureScale(25,26),
	waveFreq(0.028f),
	waveAmp(1.8f),
	BumpScale(0.2f),
	bumpSpeed(2,2),
	deepColor(0,0.3,0.9,0.8),
	shallowColor(0,1,1,0.5),
	waterAmount(0.3f),
	reflectionColor(0.95,1,1,0.5),
	reflectionAmount(1.0f),
	reflectionBlur(0.0f),	
	fresnelPower(5.f),
	fresnelBias(0.328f),
	hdrMultiplier(0.471f)
	{

	}
	bool							AlphaLayer;
	bool							_using;
	D3DXVECTOR2					textureScale;
	float							waveFreq;
	float							waveAmp;
	float							BumpScale;
	D3DXVECTOR2					bumpSpeed;
	D3DXVECTOR4					deepColor;
	D3DXVECTOR4					shallowColor;
	float							waterAmount;
	D3DXVECTOR4					reflectionColor;
	float							reflectionAmount;
	float							reflectionBlur;				
	float							fresnelPower;
	float							fresnelBias;
	float							hdrMultiplier;

};

class F3dCWater 
{
public:
	F3dCWater();
	~F3dCWater();

	void Init();
	void Process();
	void Render();
	void Destroy();
	void setTechniqueVariables();
	void InitEffect();

private:

	LPDIRECT3DDEVICE9				m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;
	LPDIRECT3DTEXTURE9			m_pWaterBump;			// π∞
	LPDIRECT3DCUBETEXTURE9		m_pReflectionMap;		// «œ¥√¿« π›ªÁ ∏ 
	LPD3DXEFFECT					m_Effect_Water;
	Vertex							m_quadVertices[4];
	float								m_timer;
	Option_Shader_Water m_Option;
};
#include "FeatherXEnginePrecompile.h"

F3dCWater::F3dCWater() :
m_timer(0)
{

}

F3dCWater::~F3dCWater()
{

}

void F3dCWater::Init()
{
	m_pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();

	float width = 1000;
	float height = 1000;
	float high = 3;

	m_quadVertices[0].Set(-width,	high,	-height, 0xffffff00, 0.0f, 0.0f);
	m_quadVertices[1].Set(width, high, -height, 0xff00ff00, 1.0f, 0.0f);
	m_quadVertices[2].Set(-width,	high,	height, 0xffff0000, 0.0f, 1.0f);
	m_quadVertices[3].Set(width, high, height, 0xff0000ff, 1.0f,	1.0f);

	m_pD3DDevice->CreateVertexBuffer( 4*sizeof(Vertex), D3DUSAGE_WRITEONLY, 
		Vertex::FVF_Flags, D3DPOOL_DEFAULT, 
		&m_pVertexBuffer, NULL );

	void *pVertices = NULL;
	m_pVertexBuffer->Lock( 0, sizeof(m_quadVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, m_quadVertices, sizeof(m_quadVertices) );
	m_pVertexBuffer->Unlock();

	InitEffect();
}



void F3dCWater::Destroy()
{

}

void F3dCWater::Process()
{

}


void F3dCWater::InitEffect()
{
	D3DXCreateTextureFromFile( m_pD3DDevice, L"Effect/Texture/wave0.dds", &m_pWaterBump );
	D3DXCreateCubeTextureFromFile( m_pD3DDevice, L"Effect/Texture/cubemap.dds", &m_pReflectionMap );

	HRESULT hr;
	LPD3DXBUFFER pBufferErrors = NULL;

	hr = D3DXCreateEffectFromFile( m_pD3DDevice, 
		L"fx/water.fx",
		NULL, 
		NULL, 
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, 
		NULL, 
		&m_Effect_Water, 
		&pBufferErrors );

	if( FAILED(hr) )
	{
	}
}

void F3dCWater::setTechniqueVariables()
{
	m_timer += 0.001f;
	D3DXMATRIX				mat_WorldViewProj;
	D3DXMATRIX				mat_World;
	D3DXMATRIX				mat_View;
	D3DXMATRIX				mat_Proj;

	m_pD3DDevice->GetTransform(D3DTS_WORLD, &mat_World);
	Identity(mat_World);
	m_pD3DDevice->GetTransform(D3DTS_VIEW,	&mat_View);
	m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mat_Proj);

	//Calculate the matrices
	mat_WorldViewProj = mat_World * mat_View * mat_Proj;

	//set the texturs
	m_Effect_Water->SetTexture("texture0", m_pWaterBump);
	m_Effect_Water->SetTexture("texture1", m_pReflectionMap);

	//set the matrices
	m_Effect_Water->SetMatrix("WorldViewProj", &mat_WorldViewProj);

	//set eye position
	D3DXVECTOR4 eye(*CFeatherXCamera::GetSingleton()->GetEyeVector(), 1.0f);
	m_Effect_Water->SetVector("eyePosition", &eye);

	//set the time
	m_Effect_Water->SetFloat("time",m_timer);

	// 파고 조절 컨트롤
	
	m_Effect_Water->SetValue("textureScale", m_Option.textureScale, sizeof(D3DXVECTOR2));
	m_Effect_Water->SetFloat("waveFreq", m_Option.waveFreq);
	m_Effect_Water->SetFloat("waveAmp",	m_Option.waveAmp);
	m_Effect_Water->SetFloat("BumpScale", m_Option.BumpScale);
	m_Effect_Water->SetValue("bumpSpeed", m_Option.bumpSpeed, sizeof(D3DXVECTOR2));
	m_Effect_Water->SetVector("deepColor", &m_Option.deepColor);
	m_Effect_Water->SetVector("shallowColor", &m_Option.shallowColor);
	m_Effect_Water->SetFloat("waterAmount", m_Option.waterAmount);
	m_Effect_Water->SetVector("reflectionColor",	&m_Option.reflectionColor);
	m_Effect_Water->SetFloat("reflectionAmount",	m_Option.reflectionAmount);
	m_Effect_Water->SetFloat("reflectionBlur", m_Option.reflectionBlur);
	m_Effect_Water->SetFloat("fresnelPower", m_Option.fresnelPower);
	m_Effect_Water->SetFloat("fresnelBias", m_Option.fresnelBias);
	m_Effect_Water->SetFloat("hdrMultiplier",	m_Option.hdrMultiplier);
	
}

void F3dCWater::Render()
{
	if(m_Option.AlphaLayer)
	{
		// 텍스쳐 사용 준비. 
		DWORD alpha = 175;
		m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, (0x00ffffff)+(alpha<<24));
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		// 알파 채널을 넣어서 반투명하게 하자.
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
		m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
		m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	m_Effect_Water->SetTechnique( "water" );
	setTechniqueVariables();
	
	UINT uPasses;
	m_Effect_Water->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
	{
		m_Effect_Water->BeginPass( uPass );
		m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );
		m_pD3DDevice->SetFVF(  Vertex::FVF_Flags );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		m_Effect_Water->EndPass();
	}
	m_Effect_Water->End();

	if(m_Option.AlphaLayer)  {
		DWORD alpha = 255;
		m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, (0x00ffffff)+(alpha<<24));
		m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}
}
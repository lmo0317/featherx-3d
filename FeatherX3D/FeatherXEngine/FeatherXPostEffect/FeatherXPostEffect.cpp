#include "FeatherXEnginePrecompile.h"

CFeatherXPostEffect::CFeatherXPostEffect():
m_pNormalMap(NULL),
m_pDecl(NULL)
{
}
CFeatherXPostEffect::~CFeatherXPostEffect()
{

}

void	CFeatherXPostEffect::Init()
{
	CFeatherXShader* pShader = CFeatherXShaderManager::GetSingleton()->GetShader(_T("FX\\cross.fx"));
	if(pShader == NULL)
		return;

	LPD3DXEFFECT pEffect = pShader->GetEffect();
	if(!pEffect)
		return;

	m_pEffect = pEffect;
	HRESULT hr;

	LPDIRECT3DSURFACE9 m_pBackBuffer = NULL;
	F3dCDevice::GetSingleton()->GetD3DDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	m_pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
	m_pBackBuffer->Release();

	LPD3DXBUFFER pErr=NULL;	
	m_hTechnique = m_pEffect->GetTechniqueByName( "TShader" );
	m_hmWVP      = m_pEffect->GetParameterByName( NULL, "mWVP" );
	m_hvLightDir = m_pEffect->GetParameterByName( NULL, "vLightDir" );
	m_hvColor    = m_pEffect->GetParameterByName( NULL, "vColor" );
	m_hvEyePos   = m_pEffect->GetParameterByName( NULL, "vEyePos" );
	m_htDecaleTex= m_pEffect->GetParameterByName( NULL, "DecaleTex" );
	m_htNormalMap= m_pEffect->GetParameterByName( NULL, "NormalMap" );
	SAFE_RELEASE(pErr);
	hr = F3dCDevice::GetSingleton()->GetD3DDevice()->CreateTexture(
									 m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, 
									 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, 
									 D3DPOOL_DEFAULT, &m_pTexScene, NULL);
	if( FAILED(hr) ) 
		return;
	
	hr = m_pTexScene->GetSurfaceLevel( 0, &m_pSurfScene );
	
	if( FAILED(hr) ) 
		return;
	
	m_dwCropWidth  = m_d3dsdBackBuffer.Width  - m_d3dsdBackBuffer.Width  % 4;
	m_dwCropHeight = m_d3dsdBackBuffer.Height - m_d3dsdBackBuffer.Height % 4;

	hr = F3dCDevice::GetSingleton()->GetD3DDevice()->CreateTexture(
									 m_dwCropWidth / 4, m_dwCropHeight / 4, 
									 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F,
									 D3DPOOL_DEFAULT, &m_pTexSceneScaled, NULL);
	if( FAILED(hr) ) 
		return;
	
	hr = m_pTexSceneScaled->GetSurfaceLevel( 0, &m_pSurfSceneScaled );
	if( FAILED(hr) ) 
		return;
	
	hr = F3dCDevice::GetSingleton()->GetD3DDevice()->CreateTexture(
									 m_dwCropWidth / 4 + 2, m_dwCropHeight / 4 + 2, 
									 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
									 D3DPOOL_DEFAULT, &m_pTexBrightPass, NULL);
	if( FAILED(hr) ) 
		return;

	hr = m_pTexBrightPass->GetSurfaceLevel( 0, &m_pSurfBrightPass );
	
	if( FAILED(hr) ) 
		return;

	F3dCDevice::GetSingleton()->GetD3DDevice()->ColorFill( m_pSurfBrightPass, NULL
							, D3DCOLOR_ARGB(0, 0, 0, 0) );
	hr = F3dCDevice::GetSingleton()->GetD3DDevice()->CreateTexture(
									 m_dwCropWidth / 4 + 2, m_dwCropHeight / 4 + 2, 
									 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
									 D3DPOOL_DEFAULT, &m_pTexStarSource, NULL);
	if( FAILED(hr) ) 
		return;
	hr = m_pTexStarSource->GetSurfaceLevel( 0, &m_pSurfStarSource );
	
	if( FAILED(hr) ) 
		return;
	
	F3dCDevice::GetSingleton()->GetD3DDevice()->ColorFill( m_pSurfStarSource, NULL
							, D3DCOLOR_ARGB(0, 0, 0, 0) );
	for(int i=0; i < NUM_STAR_TEXTURES; i++ ) {
		hr = F3dCDevice::GetSingleton()->GetD3DDevice()->CreateTexture(
			m_dwCropWidth /4, m_dwCropHeight / 4,
			1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT, &m_apTexStar[i], NULL );
		if( FAILED(hr) ) 
			return;
		
		hr = m_apTexStar[i]->GetSurfaceLevel( 0, &m_apSurfStar[i] );
		if( FAILED(hr) ) 
			return;
	}
	RS( D3DRS_ZENABLE,        TRUE );	
	RS( D3DRS_LIGHTING, FALSE );

	D3DXVECTOR3 vFromPt   = D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &m_mView, &vFromPt, &vLookatPt, &vUpVec );
	FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
	D3DXMatrixPerspectiveFovLH( &m_mProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );

}
void	CFeatherXPostEffect::Process()
{

}

void	CFeatherXPostEffect::Render()
{
	RenderScene();
	Scene_To_SceneScaled();	
	SceneScaled_To_BrightPass();
	BrightPass_To_StarSource();	
	RenderStar();               

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget(0,m_pBackBuffer);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	float w = (float)this->m_d3dsdBackBuffer.Width ;
	float h = (float)this->m_d3dsdBackBuffer.Height;
	typedef struct{float x,y,z,w,u,v;}TVERTEX;
	TVERTEX VertexFinal[4] = {
		{ 0, 0, 0, 1, 0+0.5f/w, 0+0.5f/h,},
		{ w, 0, 0, 1, 1+0.5f/w, 0+0.5f/h,},
		{ w, h, 0, 1, 1+0.5f/w, 1+0.5f/h,},
		{ 0, h, 0, 1, 0+0.5f/w, 1+0.5f/h,},
	};

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetFVF(D3DFVF_XYZRHW| D3DFVF_TEX1);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, m_pTexScene );
	F3dCDevice::GetSingleton()->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
		, 2, VertexFinal, sizeof( TVERTEX ) );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE) ;
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE) ;
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ;
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, m_apTexStar[0] );
	F3dCDevice::GetSingleton()->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
		, 2, VertexFinal, sizeof( TVERTEX ) );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) ;
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);	
	SAFE_RELEASE(m_pBackBuffer);
}

void CFeatherXPostEffect::Destroy()
{

}

HRESULT CFeatherXPostEffect::Scene_To_SceneScaled()
{
	CoordRect coords;
	RECT rectSrc;
	rectSrc.left   = (m_d3dsdBackBuffer.Width  - m_dwCropWidth ) / 2;
	rectSrc.top    = (m_d3dsdBackBuffer.Height - m_dwCropHeight) / 2;
	rectSrc.right  = rectSrc.left + m_dwCropWidth;
	rectSrc.bottom = rectSrc.top  + m_dwCropHeight;
	GetTextureCoords( m_pTexScene, &rectSrc, m_pTexSceneScaled, NULL, &coords );
	int index=0;
	D3DXVECTOR2 offsets[MAX_SAMPLES];

	for( int y=0; y < 4; y++ ) {
		for( int x=0; x < 4; x++ ) {
			offsets[ index ].x = (x - 1.5f) / m_d3dsdBackBuffer.Width;
			offsets[ index ].y = (y - 1.5f) / m_d3dsdBackBuffer.Height;
			index++;
		}
	}	
	m_pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));	
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget( 0, m_pSurfSceneScaled );
	m_pEffect->SetTechnique("DownScale4x4");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, m_pTexScene );
	DrawFullScreenQuad( coords.u0, coords.v0, coords.u1, coords.v1 );
	m_pEffect->EndPass();
	m_pEffect->End();
	
	return S_OK;
}

HRESULT CFeatherXPostEffect::SceneScaled_To_BrightPass()
{
	D3DSURFACE_DESC desc;
	m_pTexBrightPass->GetLevelDesc( 0, &desc );
	RECT rectDest = {0,0,desc.Width,desc.Height};
	InflateRect( &rectDest, -1, -1 );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetScissorRect( &rectDest );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget( 0, m_pSurfBrightPass );
	m_pEffect->SetTechnique("BrightPassFilter");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, m_pTexSceneScaled );
	DrawFullScreenQuad( 0.0f, 0.0f, 1.0f, 1.0f );
	m_pEffect->EndPass();
	m_pEffect->End();

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

	return S_OK;
}
HRESULT CFeatherXPostEffect::GetGaussBlur5x5(
	DWORD dwD3DTexWidth, DWORD dwD3DTexHeight,
	D3DXVECTOR2* avTexCoordOffset, D3DXVECTOR4* avSampleWeight)
{
	float tu = 1.0f / (float)dwD3DTexWidth ;
	float tv = 1.0f / (float)dwD3DTexHeight ;

	float totalWeight = 0.0f;
	int index=0;
	for( int x = -2; x <= 2; x++ ) {
		for( int y = -2; y <= 2; y++ ) {
			if( 2 < abs(x) + abs(y) ) continue;

			avTexCoordOffset[index] = D3DXVECTOR2( x * tu, y * tv );
			float fx = (FLOAT)x;
			float fy = (FLOAT)y;
			avSampleWeight[index].x = avSampleWeight[index].y =
				avSampleWeight[index].z = avSampleWeight[index].w
				= expf( -(fx*fx + fy*fy)/(2*1.0f) );
			totalWeight += avSampleWeight[index].x;

			index++;
		}
	}
	for( int i=0; i < index; i++ ) avSampleWeight[i] *= 1.0f/totalWeight;

	return S_OK;
}

HRESULT CFeatherXPostEffect::BrightPass_To_StarSource()
{
	D3DSURFACE_DESC desc;
	m_pTexStarSource->GetLevelDesc( 0, &desc );
	RECT rectDest = {0,0,desc.Width,desc.Height};
	InflateRect( &rectDest, -1, -1 );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetScissorRect( &rectDest );
	CoordRect coords;
	GetTextureCoords( m_pTexBrightPass, NULL, m_pTexStarSource,
		&rectDest, &coords );
	D3DXVECTOR2 offsets[MAX_SAMPLES];
	D3DXVECTOR4 weights[MAX_SAMPLES];
	m_pTexBrightPass->GetLevelDesc( 0, &desc );
	GetGaussBlur5x5( desc.Width, desc.Height, offsets, weights );
	m_pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));
	m_pEffect->SetValue("g_avSampleWeights", weights, sizeof(weights));
	m_pEffect->SetTechnique("GaussBlur5x5");
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget( 0, m_pSurfStarSource );
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, m_pTexBrightPass );
	DrawFullScreenQuad( coords.u0, coords.v0, coords.u1, coords.v1 );
	m_pEffect->EndPass();
	m_pEffect->End();

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

	return S_OK;
}
HRESULT CFeatherXPostEffect::RenderStar()
{
	static const int s_maxPasses = 3;
	static const int nSamples = 8;
	static const D3DXCOLOR s_colorWhite(0.63f, 0.63f, 0.63f, 0.0f) ;
	static const D3DXCOLOR s_ChromaticAberrationColor[8] = {
		D3DXCOLOR(0.5f, 0.5f, 0.5f,  0.0f),	
		D3DXCOLOR(0.8f, 0.3f, 0.3f,  0.0f), 
		D3DXCOLOR(1.0f, 0.2f, 0.2f,  0.0f),	
		D3DXCOLOR(0.5f, 0.2f, 0.6f,  0.0f), 
		D3DXCOLOR(0.2f, 0.2f, 1.0f,  0.0f),	
		D3DXCOLOR(0.2f, 0.3f, 0.7f,  0.0f), 
		D3DXCOLOR(0.2f, 0.6f, 0.2f,  0.0f),	
		D3DXCOLOR(0.3f, 0.5f, 0.3f,  0.0f), 
	} ;

	static D3DXVECTOR4 s_aaColor[s_maxPasses][nSamples];

	for (int p = 0 ; p < s_maxPasses ; p ++) {
		float ratio = (float)(p + 1) / (float)s_maxPasses ;
		for (int s = 0 ; s < nSamples ; s ++) {
			D3DXCOLOR chromaticAberrColor ;
			D3DXColorLerp(&chromaticAberrColor,
				&( s_ChromaticAberrationColor[s] ),
				&s_colorWhite, ratio) ;
			D3DXColorLerp( (D3DXCOLOR*)&( s_aaColor[p][s] ),
				&s_colorWhite, &chromaticAberrColor, 0.7f ) ;
		}
	}

	float radOffset = m_fWorldRotY/5;
	D3DSURFACE_DESC desc;
	m_pSurfStarSource->GetDesc( &desc );
	float srcW = (FLOAT) desc.Width;
	float srcH = (FLOAT) desc.Height;

	int nStarLines = 6;
	for (int d = 0 ; d < nStarLines ; d ++) {
		PDIRECT3DTEXTURE9 pTexSource = m_pTexStarSource;
		float rad = radOffset + 2*d*D3DX_PI/(FLOAT)nStarLines;
		float sn = sinf(rad);
		float cs = cosf(rad);
		D3DXVECTOR2 vtStepUV = D3DXVECTOR2(0.3f * sn / srcW,
			0.3f * cs / srcH);

		float attnPowScale = (atanf(D3DX_PI/4) + 0.1f) *
			(160.0f + 120.0f) / (srcW + srcH);

		int iWorkTexture = 0;
		for (int p = 0 ; p < s_maxPasses; p++) {
			PDIRECT3DSURFACE9 pSurfDest = NULL;
			if (p == s_maxPasses - 1) {
				pSurfDest = m_apSurfStar[d+2];
			} else {
				pSurfDest = m_apSurfStar[iWorkTexture];
			}
			F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget( 0, pSurfDest );
			D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];
			D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];
			for (int i = 0 ; i < nSamples ; i++) {
				float lum = powf( 0.95f, attnPowScale * i );
				avSampleWeights[i] = s_aaColor[s_maxPasses-1-p][i]
				* lum * (p+1.0f) * 0.5f ;
				avSampleOffsets[i].x = vtStepUV.x * i ;
				avSampleOffsets[i].y = vtStepUV.y * i ;
				if ( 0.9f <= fabs(avSampleOffsets[i].x) ||
					0.9f <= fabs(avSampleOffsets[i].y) ) {
						avSampleOffsets[i].x = 0.0f ;
						avSampleOffsets[i].y = 0.0f ;
						avSampleWeights[i] *= 0.0f ;
				}
			}
			m_pEffect->SetValue("g_avSampleOffsets", avSampleOffsets, sizeof(avSampleOffsets));
			m_pEffect->SetVectorArray("g_avSampleWeights", avSampleWeights, nSamples);
			m_pEffect->SetTechnique("Star");
			m_pEffect->Begin(NULL, 0);
			m_pEffect->BeginPass(0);
			F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, pTexSource );
			DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);
			m_pEffect->EndPass();
			m_pEffect->End();
			vtStepUV *= nSamples;
			attnPowScale *= nSamples;
			pTexSource = m_apTexStar[iWorkTexture];
			iWorkTexture ^= 1;
		}
	}

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget( 0, m_apSurfStar[0] );

	m_pEffect->SetTechnique("MergeTextures");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, m_apTexStar[0+2] );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 1, m_apTexStar[1+2] );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 2, m_apTexStar[2+2] );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 3, m_apTexStar[3+2] );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 4, m_apTexStar[4+2] );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 5, m_apTexStar[5+2] );

	DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);

	m_pEffect->End();

	return S_OK;
}

HRESULT CFeatherXPostEffect::GetTextureCoords( PDIRECT3DTEXTURE9 pTexSrc, RECT* pRectSrc, 
											PDIRECT3DTEXTURE9 pTexDest, RECT* pRectDest, CoordRect* pCoords )
{
	D3DSURFACE_DESC desc;

	if( pTexSrc == NULL || pTexDest == NULL || pCoords == NULL )
		return E_INVALIDARG;
	pCoords->u0 = 0.0f;
	pCoords->v0 = 0.0f;
	pCoords->u1 = 1.0f; 
	pCoords->v1 = 1.0f;

	if( pRectSrc != NULL ) {
		pTexSrc->GetLevelDesc( 0, &desc );
		pCoords->u0 += pRectSrc->left                   / desc.Width;
		pCoords->v0 += pRectSrc->top                    / desc.Height;
		pCoords->u1 -= (desc.Width  - pRectSrc->right)  / desc.Width;
		pCoords->v1 -= (desc.Height - pRectSrc->bottom) / desc.Height;
	}

	if( pRectDest != NULL ) {
		pTexDest->GetLevelDesc( 0, &desc );
		pCoords->u0 -= pRectDest->left                   / desc.Width;
		pCoords->v0 -= pRectDest->top                    / desc.Height;
		pCoords->u1 += (desc.Width  - pRectDest->right)  / desc.Width;
		pCoords->v1 += (desc.Height - pRectDest->bottom) / desc.Height;
	}

	return S_OK;
}

void CFeatherXPostEffect::RenderScene()
{
	D3DXMATRIX m, mT, mR, mView, mProj;
	D3DXMATRIX mWorld;
	D3DXVECTOR4 v, light_pos, eye_pos;
	D3DXMatrixScaling( &m,  3.0f, 3.0f, 3.0f );
	D3DXMatrixRotationY( &mR,  D3DX_PI );
	D3DXMatrixTranslation( &mT, 0.0f,-2.0f ,0.0f );
	mWorld = m * mR * mT;
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_WORLD,  &mWorld);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_VIEW,  &m_mView );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_PROJECTION,  &m_mProj );

	TSS( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	TSS( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SAMP( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	SAMP( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	
	if( m_pEffect != NULL ) 
	{
		D3DXMatrixRotationY( &mWorld,  m_fTime );
		m = mWorld * m_mView * m_mProj;
		m_pEffect->SetMatrix( m_hmWVP, &m );

		light_pos = D3DXVECTOR4( -100.577f, -0.577f, -0.577f,1);
		D3DXMatrixInverse( &m, NULL, &mWorld);
		D3DXVec4Transform( &v, &light_pos, &m );
		D3DXVec3Normalize( (D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v );
		v.w = -0.5f;
		m_pEffect->SetVector( m_hvLightDir, &v );

		m = mWorld * m_mView;
		D3DXMatrixInverse( &m, NULL, &m);
		v = D3DXVECTOR4( 0, 0, 0, 1);
		D3DXVec4Transform( &v, &v, &m );
		m_pEffect->SetVector( m_hvEyePos, &v );
		m_pEffect->SetTexture( m_htNormalMap, m_pNormalMap );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetVertexDeclaration( m_pDecl );
	}
}


void CFeatherXPostEffect::DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV)
{
	D3DSURFACE_DESC desc;
	PDIRECT3DSURFACE9 pSurf;
	F3dCDevice::GetSingleton()->GetD3DDevice()->GetRenderTarget(0, &pSurf);
	pSurf->GetDesc(&desc);
	pSurf->Release();
	FLOAT w = (FLOAT)desc.Width;
	FLOAT h = (FLOAT)desc.Height;

	typedef struct{
		float p[4];
		float t[2];
	} ScreenVertex;
	ScreenVertex svQuad[4] = {
		{0-0.5f, 0-0.5f, 0.5f, 1.0f, fLeftU,  fTopV,},
		{w-0.5f, 0-0.5f, 0.5f, 1.0f, fRightU, fTopV,},
		{0-0.5f, h-0.5f, 0.5f, 1.0f, fLeftU,  fBottomV,},
		{w-0.5f, h-0.5f, 0.5f, 1.0f, fRightU, fBottomV,},
	};

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	F3dCDevice::GetSingleton()->GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(ScreenVertex));
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}


bool CFeatherXPostEffect::Begin()
{
	F3dCDevice::GetSingleton()->GetD3DDevice()->GetRenderTarget(0, &m_pBackBuffer);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderTarget(0, m_pSurfScene);
	return true;
}
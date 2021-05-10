#include "FeatherXEnginePrecompile.h"

F3dCRenderMgr::F3dCRenderMgr():
m_pRenderWorld(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}
F3dCRenderMgr::~F3dCRenderMgr()
{

}

void	F3dCRenderMgr::Init()
{
	m_pDebugRenderer = new DEBUG_RENDERER;

	HRESULT hr;
	LPD3DXBUFFER pErr=NULL;

	if( FAILED( hr = D3DXCreateEffectFromFile(
		F3dCDevice::GetSingleton()->GetD3DDevice(), L"fx\\phong.fx", NULL, NULL, 
		0 , NULL, &m_pEffect, &pErr ))){
			if(pErr) {
			MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer()
				, L"ERROR", MB_OK);
			}
	}else{
		m_hTechnique = m_pEffect->GetTechniqueByName( "TShader" );
		m_hmWVP      = m_pEffect->GetParameterByName( NULL, "mWVP" );
		m_hmW        = m_pEffect->GetParameterByName( NULL, "mW" );
		m_hvLightDir = m_pEffect->GetParameterByName( NULL, "vLightDir" );
		m_hvColor    = m_pEffect->GetParameterByName( NULL, "vColor" );
		m_hvEyePos   = m_pEffect->GetParameterByName( NULL, "vEyePos" );
	}
}

void	F3dCRenderMgr::Process()
{

}

void	F3dCRenderMgr::PhysXDebugRender()
{
	F3dMatrix matWVP,matWorld,matView,matProj;
	Identity(matWorld);
	matWorld = F3dCRenderMgr::GetSingleton()->GetWorldMatrix();
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform(D3DTS_WORLD,&matWorld);

	// 뷰 행렬
	matView = F3dCRenderMgr::GetSingleton()->GetViewMatrix();
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_VIEW, &matView );

	matProj = F3dCRenderMgr::GetSingleton()->GetProjMatrix();
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_PROJECTION, &matProj );

	NxScene* pScene = mpManager::GetSingleton()->GetScene();

	if(pScene == NULL)
		return;

	if(m_pDebugRenderer)
		m_pDebugRenderer->RenderData(F3dCDevice::GetSingleton()->GetD3DDevice(),pScene->getDebugRenderable());
}

void	F3dCRenderMgr::Render(double dTime,double dt)
{	
	D3DXMatrixLookAtLH(
		&m_matView,
		CFeatherXCamera::GetSingleton()->GetEyeVector(),
		CFeatherXCamera::GetSingleton()->GetLookatVector(),
		CFeatherXCamera::GetSingleton()->GetUpVector()
		);
	LPDIRECT3DDEVICE9 pDevie = F3dCDevice::GetSingleton()->GetD3DDevice();
	pDevie->SetTransform(D3DTS_VIEW,&m_matView);
	D3DXMatrixPerspectiveFovLH(&m_matProj,D3DX_PI/4,1.0f,1.0f,100000.0f);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform(D3DTS_PROJECTION,&m_matProj);

	CFeatherXPostEffect::GetSingleton()->Begin();

	if( SUCCEEDED( F3dCDevice::GetSingleton()->GetD3DDevice()->BeginScene()) == 0 )
		return;	

	if(F3dCTextureManager::GetSingleton()) {
		F3dCTextureManager::GetSingleton()->StartRender();
	}

	F3dCDevice::GetSingleton()->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x88888888, 1.0f, 0L);

	//라이팅 켜는 부분
	CFeatherXLight::GetSingleton()->EnableLight();

	//물 렌더 하는 위치
	//F3dCWaterManager::GetSingleton()->Render();

	//모델 렌더 하는 위치
	CFeatherXModelManager::GetSingleton()->Render();

	RenderPhongWorld();

	//파티클 이펙트 렌더 하는 부분
	F3dCFxManager::GetSingleton()->Render();

	//tail 렌더 하는 부분
	F3dCTailManager::GetSingleton()->Render(dTime,dt);

	//post effect 렌더링 하는 부분
	CFeatherXPostEffect::GetSingleton()->Render();

	//physx debug render
	F3dCRenderMgr::GetSingleton()->PhysXDebugRender();

	m_RenderUISpriteNormal.Render();
	m_RenderUISpriteNormal.RenderWire();

	m_RenderUISpriteTop.Render();
	m_RenderUISpriteTop.RenderWire();

	F3dCDevice::GetSingleton()->GetD3DDevice()->EndScene();
	F3dCDevice::GetSingleton()->GetD3DDevice()->Present( NULL, NULL, NULL, NULL );
}

void F3dCRenderMgr::RenderPhongWorld()
{
	D3DXMATRIX m, mT, mR, mView, mProj;
	D3DXMATRIX mWorld;
	D3DXVECTOR4 v, light_pos, eye_pos;
	DWORD i;

	// 행렬 설정
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_WORLD,  &m_matWorld);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_VIEW,  &m_matView );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_PROJECTION,  &m_matProj );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	if( m_pEffect == NULL ) 
		return;

	UINT uPasses;
	m_pEffect->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
	{

		m_pEffect->SetTechnique( m_hTechnique );
		m_pEffect->BeginPass( uPass);
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetFVF( D3DFVF_CUSTOMVERTEX );

		// 로컬-월드 변환행렬
		m_pEffect->SetMatrix( m_hmW, &m_matWorld );

		// 로컬-투영 변환행렬
		m = m_matWorld * m_matView * m_matProj;
		m_pEffect->SetMatrix( m_hmWVP, &m );

		// 광원방향
		light_pos = D3DXVECTOR4( -0.577f, -0.577f, -0.577f,0);
		D3DXMatrixInverse( &m, NULL, &mWorld);
		D3DXVec4Transform( &v, &light_pos, &m );
		D3DXVec3Normalize( (D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v );
		v.w = -0.3;
		m_pEffect->SetVector( m_hvLightDir, &v );

		// 시점
		m = m_matWorld * m_matView;
		D3DXMatrixInverse( &m, NULL, &m);
		v = D3DXVECTOR4( 0, 0, 0, 1);
		D3DXVec4Transform( &v, &v, &m );
		m_pEffect->SetVector( m_hvEyePos, &v );

		fcoWorld* pWorld = m_pRenderWorld->GetWorld();
		LPDIRECT3DTEXTURE9* pTexDiffuse = pWorld->GetTexDiffuse();
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, *pTexDiffuse);
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetStreamSource( 0, *pWorld->GetVB(), 0, sizeof(CUSTOMVERTEX) );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetIndices( *pWorld->GetIB() );
		F3dCDevice::GetSingleton()->GetD3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pWorld->GetXHeight() * pWorld->GetZHeight(), 0, (pWorld->GetXHeight() -1)*(pWorld->GetZHeight() -1)*2 );
	}

	m_pEffect->End();
}

void F3dCRenderMgr::RenderWorld()
{
	F3dMatrix matWorld;
	Identity(matWorld);

	if(!m_pRenderWorld)
		return;

	fcoWorld* pWorld = m_pRenderWorld->GetWorld();
	LPDIRECT3DTEXTURE9* pTexDiffuse = pWorld->GetTexDiffuse();
	F3dCDevice::GetSingleton()->GetD3DDevice()->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128,128,128), 1.0f, 0 );
	CFeatherXLight::GetSingleton()->SetupLights();

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTexture( 0, *pTexDiffuse);							/// 0번 텍스쳐 스테이지에 텍스쳐 고정(색깔맵)
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	/// 0번 텍스처 스테이지의 확대 필터
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		/// 0번 텍스처 : 0번 텍스처 인덱스 사용

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform( D3DTS_WORLD, &matWorld );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetStreamSource( 0, *pWorld->GetVB(), 0, sizeof(CUSTOMVERTEX) );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetFVF( D3DFVF_CUSTOMVERTEX );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetIndices( *pWorld->GetIB() );
	F3dCDevice::GetSingleton()->GetD3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pWorld->GetXHeight() * pWorld->GetZHeight(), 0, (pWorld->GetXHeight() -1)*(pWorld->GetZHeight() -1)*2 );
}

void	F3dCRenderMgr::Destroy()
{

}

bool F3dCRenderMgr::BeginRender()
{
	if( SUCCEEDED( F3dCDevice::GetSingleton()->GetD3DDevice()->BeginScene() ) )
		return true;
	return false;
}

void F3dCRenderMgr::EndRender()
{
	F3dCDevice::GetSingleton()->GetD3DDevice()->EndScene();
	F3dCDevice::GetSingleton()->GetD3DDevice()->Present( NULL, NULL, NULL, NULL );
}

void F3dCRenderMgr::SetRenderPipeline()
{
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform(D3DTS_WORLD,&m_matWorld);
	D3DXMatrixLookAtLH(&m_matView,CFeatherXCamera::GetSingleton()->GetEyeVector(),CFeatherXCamera::GetSingleton()->GetLookatVector(),CFeatherXCamera::GetSingleton()->GetUpVector());
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform(D3DTS_VIEW,&m_matView);
	D3DXMatrixPerspectiveFovLH(&m_matProj,D3DX_PI/4,1.0f,1.0f,500.0f);
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTransform(D3DTS_PROJECTION,&m_matProj);
}

void F3dCRenderMgr::RegUISprite( const F3dCUISprite* pUISprite )  
{
	if( !pUISprite->bIsTop )
		m_RenderUISpriteNormal.RegUISprite( pUISprite );
	else
		m_RenderUISpriteTop.RegUISprite( pUISprite );
}
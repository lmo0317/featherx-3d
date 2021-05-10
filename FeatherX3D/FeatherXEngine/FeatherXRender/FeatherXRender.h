#pragma once

class F3dCRenderMgr : public FBSingleton< F3dCRenderMgr >
{
public:
	F3dCRenderMgr();
	~F3dCRenderMgr();

	void Init();
	void Process();
	void Render(double dTime,double dt);
	void Destroy();

	void SetRenderPipeline();
	bool BeginRender();
	void EndRender();
	void RenderWorld();
	void RenderPhongWorld();

	D3DXMATRIXA16 GetViewMatrix() {return m_matView;}
	D3DXMATRIXA16 GetWorldMatrix() {return m_matWorld;}
	D3DXMATRIXA16 GetProjMatrix() {return m_matProj;}
	void PhysXDebugRender();
	void SetRenderWorld(F3dCWorld* pRenderWorld) {m_pRenderWorld = pRenderWorld;}

	void RegUISprite( const F3dCUISprite* pUISprite );

private:

	D3DXMATRIXA16 m_matWorld;
	D3DXMATRIXA16 m_matView;
	D3DXMATRIXA16 m_matProj;
	DEBUG_RENDERER* m_pDebugRenderer;
	F3dCWorld *m_pRenderWorld;

	LPD3DXEFFECT		   m_pEffect;		// 이펙트
	D3DXHANDLE				m_hTechnique;	// 테크닉
	D3DXHANDLE				m_hmWVP;		// 로컬-투영변환행렬
	D3DXHANDLE				m_hmW;			// 로컬-월드변환행렬
	D3DXHANDLE				m_hvLightDir;	// 광원방향
	D3DXHANDLE				m_hvColor;		// 정점색
	D3DXHANDLE				m_hvEyePos;		// 시점의 위치

	F3dCRenderUISprite				m_RenderUISpriteNormal;
	F3dCRenderUISprite				m_RenderUISpriteTop;
};
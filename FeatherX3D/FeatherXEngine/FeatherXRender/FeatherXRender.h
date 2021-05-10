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

	LPD3DXEFFECT		   m_pEffect;		// ����Ʈ
	D3DXHANDLE				m_hTechnique;	// ��ũ��
	D3DXHANDLE				m_hmWVP;		// ����-������ȯ���
	D3DXHANDLE				m_hmW;			// ����-���庯ȯ���
	D3DXHANDLE				m_hvLightDir;	// ��������
	D3DXHANDLE				m_hvColor;		// ������
	D3DXHANDLE				m_hvEyePos;		// ������ ��ġ

	F3dCRenderUISprite				m_RenderUISpriteNormal;
	F3dCRenderUISprite				m_RenderUISpriteTop;
};
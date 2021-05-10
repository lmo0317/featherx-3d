#pragma once

class F3dCModel : public F3dCPhysics
{
public:
	F3dCModel();
	~F3dCModel();

	void Create(WCHAR* strFile);
	
	void Init();
	void Process();
	void Render();
	void Destroy();
	void Animate();
	void SetMat();
	void SetPos(F3dVector vecPos);
	F3dVector GetPos();
	void SetDir(F3dVector vecDir);
	F3dVector GetDir();
	void CreateFx(WCHAR* strFile,std::wstring& strLinkedNode,F3dVector& vecOffset);
	D3DXMATRIXA16* F3dCModel::GetNodeMat(std::wstring strName);

	void Update();

private:
	ZNodeMgr* m_pNodeMgr;

	int m_nDrawMode;
	int m_nDrawOption;
	bool bAnimate;
	float m_fFrame;

	std::vector<F3dCParticle*> m_vecFx;
	F3dVector m_vecDirection;
	F3dVector m_vecPosition;
};
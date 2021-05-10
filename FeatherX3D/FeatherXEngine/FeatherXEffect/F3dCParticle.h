#pragma once

class F3dCModel;
class F3dCParticle 
{
public:
	F3dCParticle();
	~F3dCParticle();
	
	bool Init(WCHAR* strFile);
	bool Process( double dTime);
	
	bool AddParticle( double dTime );

	/************************************************************************/
	/* SET                                                                  */
	/************************************************************************/
	void SetPos(F3dParticle* pParticle);
	void SetVelocity(F3dParticle* pParticle);
	void SetRot(F3dParticle* pParticle);

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	bool Render();
	void CameraMatrixUpdate();
	void Reset();
	void DeleteIndexBuffer();
	void MakeIndexBuffer();
	void SetPlay();
	void SetWorldMat(D3DXMATRIX mat) {m_matWorld = mat;}
	void SetParentInfo(F3dParticleParentInfo& ParentInfo);

private:
	static WORD*								s_pIndexBuffer;

	bool										m_bPlay;
	double										m_dCurTime;
	double										m_dShootStartTime;
	double										m_dStartTick;
	int											m_nVertexNum;
	int											m_nParticleNum;
	

	F3dParticleVertex*							m_pParticleVertex;
	F3dCParticleTemplate*						m_pParticleTemplate;	

	F3dVector									m_vecPos;
	F3dMatrix									m_matWorld;
	F3dMatrix									m_mtxViewProj;
	F3dMatrix									m_matBillboard;
	F3dMatrix									m_matBillboardY;
	F3dMatrix									m_matD3DIdentity;

	LPDIRECT3DTEXTURE9							m_pTexture;
	std::vector< F3dParticle*>					m_vParticle;

	F3dParticleParentInfo						m_ParentInfo;
	
};

#pragma once

class ZBone : public ZNode
{
protected:
	D3DXMATRIXA16			m_matSkin;		/// Skin TM 행렬
public:
	ZBone( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZBone();

	D3DXMATRIXA16* GetMatrixSkin() { return &m_matSkin; }
	D3DXMATRIXA16* Animate( float frame, D3DXMATRIXA16* pParent );

	/// 메시를 그린다
	virtual int	Draw( D3DXMATRIX* pTM );
};
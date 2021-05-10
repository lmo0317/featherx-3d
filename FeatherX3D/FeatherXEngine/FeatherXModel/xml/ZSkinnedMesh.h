#pragma once

/**
 * 스키닝 메시 클래스의 BASE클래스
 *
 */
class ZSkinnedMesh : public ZMesh
{
protected:
	vector<int>				m_idxBones;	/// 메시에 영향을 미치는 뼈대의 인덱스
	vector<D3DXMATRIX>*		m_pMatPalette;

public:
	ZSkinnedMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZSkinnedMesh();
	
	// 최초에 한번 불린다.
	void SetMatrixPalette( vector<D3DXMATRIX>* pPal ) { m_pMatPalette = pPal; }

	vector<int>& GetBones() { return m_idxBones; }
	virtual int	Draw( D3DXMATRIX* pTM ) { return 1; }
};
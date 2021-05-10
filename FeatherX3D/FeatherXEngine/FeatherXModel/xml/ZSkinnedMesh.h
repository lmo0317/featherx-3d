#pragma once

/**
 * ��Ű�� �޽� Ŭ������ BASEŬ����
 *
 */
class ZSkinnedMesh : public ZMesh
{
protected:
	vector<int>				m_idxBones;	/// �޽ÿ� ������ ��ġ�� ������ �ε���
	vector<D3DXMATRIX>*		m_pMatPalette;

public:
	ZSkinnedMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZSkinnedMesh();
	
	// ���ʿ� �ѹ� �Ҹ���.
	void SetMatrixPalette( vector<D3DXMATRIX>* pPal ) { m_pMatPalette = pPal; }

	vector<int>& GetBones() { return m_idxBones; }
	virtual int	Draw( D3DXMATRIX* pTM ) { return 1; }
};
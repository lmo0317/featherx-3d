#pragma once

/**
 * ���������� SW ��Ű�� �޽� Ŭ����
 *
 */
class ZSWSkinnedMesh : public ZSkinnedMesh
{
protected:
	/// ����Ʈ���� ��Ű���� ���ؼ� �������ۿ� �Ȱ��� �����迭�� �� ī�� ������ �д�.
	ZSWSkinnedVertex*		m_pvtx;		/// skinning ���� �迭
	ZRigidVertex*			m_pvtxDraw;	/// rigid ���� �迭
	// ����Ʈ���� ��Ű���� ���ؼ� �ε������ۿ� �Ȱ��� �ε����迭�� �� ī�� ������ �д�.
	void*					m_pidx;		/// index�� 16��Ʈ, 32��Ʈ 2������ �����ؾ�
										/// �ϹǷ� void�� �����͸� ����ߴ�
	D3DFORMAT				m_fmtIdx;	/// D3DFMT_INDEX16 or D3DFMT_INDEX32
	
	int _CreateVIB( ZCMesh* pMesh );
	void _ApplyPalette( D3DXMATRIX* pTM );

public:
	ZSWSkinnedMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZSWSkinnedMesh();

	virtual int	Draw( D3DXMATRIX* pTM );
};
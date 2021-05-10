#pragma once

/**
 * 3D������Ʈ�� �޽üӼ��� ���� Ŭ����
 *
 */
class ZMesh : public ZNode
{
protected:
	BOOL					m_isSkinned;	/// ��Ű���� ����Ǵ� �޽��ΰ�?

	DWORD					m_nVerts;		/// ��ü ������ ����
	DWORD					m_nTriangles;	/// �׷��� �ﰢ���� ����
	DWORD					m_dwFVF;		/// ������ fvf��
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			/// ���� ����
	LPDIRECT3DINDEXBUFFER9	m_pIB;			/// �ε��� ����
public:
	ZMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZMesh();
	
	BOOL	IsSkinned() { return m_isSkinned; }
	/// �޽ø� �׸���
	virtual int	Draw( D3DXMATRIX* pTM ) { return 1; }
};
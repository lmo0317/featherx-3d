#pragma once

/**
 * ��Ű�� ���� �޽� Ŭ����
 *
 */
class ZRigidMesh : public ZMesh
{
protected:
	int _CreateVIB( ZCMesh* pMesh );
public:
	ZRigidMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZRigidMesh();

	virtual int	Draw( D3DXMATRIX* pTM );
};
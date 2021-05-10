#pragma once

/**
 * 스키닝 없는 메시 클래스
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
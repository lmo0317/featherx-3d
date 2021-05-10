#pragma once

/**
 * D3D�� Fixed Function ��Ű�� �޽� Ŭ����
 *
 */
class ZFFSkinnedMesh : public ZSkinnedMesh
{
protected:
	int		_CreateVIB( ZCMesh* pMesh );
	void	_ApplyPalette( D3DXMATRIX* pTM );
public:
	ZFFSkinnedMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh );
	virtual ~ZFFSkinnedMesh();

	virtual int	Draw( D3DXMATRIX* pTM );
};
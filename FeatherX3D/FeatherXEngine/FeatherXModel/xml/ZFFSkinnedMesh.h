#pragma once

/**
 * D3D의 Fixed Function 스키닝 메시 클래스
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
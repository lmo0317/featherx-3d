#include "FeatherXEnginePrecompile.h"

ZSkinnedMesh::ZSkinnedMesh( LPDIRECT3DDEVICE9 pDev, ZCMesh* pMesh ) : ZMesh( pDev, pMesh )
{
	m_pMatPalette = NULL;
}

ZSkinnedMesh::~ZSkinnedMesh()
{
	m_pMatPalette = NULL;
	stl_wipe_vector( m_idxBones );
}

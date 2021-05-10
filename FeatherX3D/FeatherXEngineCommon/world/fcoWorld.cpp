#include "FeatherXEngineCommonPrecompile.h"
#define SCALE 4
#define TEX_SCALE 25

fcoWorld::fcoWorld()
{

}

fcoWorld::~fcoWorld()
{

}

/************************************************************************/
/* INIT GEOMETRY                                                        */
/************************************************************************/

bool fcoWorld::InitGeometry()
{
	if( FAILED( InitTexture() ) ) return false;
	if( FAILED( InitVB() ) ) return false;
	if( FAILED( InitIB() ) ) return false;

	return true;
}

/************************************************************************/
/* INIT VERTEX BUFFER                                                   */
/************************************************************************/

bool fcoWorld::InitIB()
{
	LPDIRECT3DINDEXBUFFER9* pIB = GetIB();
	if( FAILED( F3dCDevice::GetSingleton()->GetD3DDevice()->CreateIndexBuffer( (GetXHeight()-1)*(GetZHeight()-1)*2 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, pIB, NULL ) ) )
	{
		return false;
	}

	MYINDEX		i;
	MYINDEX*	pI;

	int nSize = (GetXHeight()-1)*(GetZHeight()-1)*2;
	if( FAILED((*pIB)->Lock( 0, nSize * sizeof(MYINDEX), (void**)&pI, 0 ) ) )
		return false;

	NxU32* faces = new NxU32[nSize*3];
	SetFaces(faces);

	NxU32 k = 0;
	for( DWORD z = 0 ; z < GetZHeight()-1 ; z++ )
	{
		for( DWORD x = 0 ; x < GetXHeight()-1 ; x++ )
		{
			i._0 = (z*GetXHeight()+x);
			i._1 = (z*GetXHeight()+x+1);
			i._2 = ((z+1)*GetXHeight()+x);
			
			faces[k++] = i._0;
			faces[k++] = i._2;
			faces[k++] = i._1;

			*pI++ = i;

			i._0 = ((z+1)*GetXHeight()+x);
			i._1 = (z*GetXHeight()+x+1) ;
			i._2 = ((z+1)*GetXHeight()+x+1);

			faces[k++] = i._0;
			faces[k++] = i._2;
			faces[k++] = i._1;

			*pI++ = i;
		}
	}

	(*pIB)->Unlock();
	return true;
}

/************************************************************************/
/* INIT INDEX BUFFER                                                    */
/************************************************************************/

bool fcoWorld::InitVB()
{
	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;
	LPDIRECT3DTEXTURE9* pTexHeight = GetTexHeight();

	(*pTexHeight)->GetLevelDesc( 0, &ddsd );
	SetXHeight(ddsd.Width);
	SetZHeight(ddsd.Height);

	LPDIRECT3DVERTEXBUFFER9* pVB = GetVB();
	if( FAILED( F3dCDevice::GetSingleton()->GetD3DDevice()->CreateVertexBuffer( ddsd.Width*ddsd.Height*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, pVB, NULL ) ) )
	{
		return false;
	}

	(*pTexHeight)->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
	VOID* pVertices;
	if( FAILED( (*pVB)->Lock( 0, GetXHeight() * GetZHeight() *sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
		return false;

	m_pVerts = new NxVec3[GetXHeight() * GetZHeight()];
	CUSTOMVERTEX	v;
	CUSTOMVERTEX*	pV = (CUSTOMVERTEX*)pVertices;
	int nSize = GetXHeight();

	for( DWORD z = 0 ; z < GetZHeight() ; z++ )
	{
		for( DWORD x = 0 ; x < GetXHeight() ; x++ )
		{
			v.p.x = ((float)x - GetXHeight()/2.f) * SCALE;
			v.p.z = ((float)z - GetZHeight()/2.f) * SCALE;
			DWORD dY = *((LPDWORD)d3drc.pBits+x+z*(d3drc.Pitch/4));

			v.p.y = ((float)(dY & 0x000000ff)/10.0f ) * SCALE;	/// DWORDÀÌ¹Ç·Î pitch/4
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize( &v.n, &v.n );
			v.t.x = ((float)x / (GetXHeight()-1)) * SCALE * TEX_SCALE;
			v.t.y = ((float)z / (GetZHeight()-1)) * SCALE * TEX_SCALE;
			*pV++ = v;
			m_pVerts[x + z*nSize] = NxVec3(v.p.x , v.p.y , v.p.z);
		}
	}

	(*pVB)->Unlock();
	(*pTexHeight)->UnlockRect( 0 );

	return true;
}

/************************************************************************/
/* INIT TEXTURE                                                         */
/************************************************************************/
bool fcoWorld::InitTexture()  
{
	LPDIRECT3DTEXTURE9* pTexHeight = GetTexHeight();
	if( FAILED( D3DXCreateTextureFromFileEx( F3dCDevice::GetSingleton()->GetD3DDevice(), L"world\\map128_6.bmp", 
		D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_DEFAULT, 0, 
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, pTexHeight) ) )
		return false;

	LPDIRECT3DTEXTURE9* pTexDiffuse = GetTexDiffuse();
	if( FAILED( D3DXCreateTextureFromFile( F3dCDevice::GetSingleton()->GetD3DDevice(), L"world\\cross_garden_jadi.dds", pTexDiffuse) ) )
		return false;

	return true;
}
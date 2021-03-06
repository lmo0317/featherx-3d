#pragma once
struct ZObjectInfo
{
	int				nObjectCount;
	int				nMeshCount;
	float			fAnimationStart;
	float			fAnimationEnd;
	int				nKeyType;
	int				nSkinType;
	vector<wstring>	strObjectNameTable;
	vector<int>		BoneTable;
	vector<int>		MeshTable;
	~ZObjectInfo()
	{
		stl_wipe_vector( strObjectNameTable );
		stl_wipe_vector( BoneTable );
	}
};

struct ZMaterial
{
	D3DMATERIAL9			material;	/// 재질
	float					opacity;	/// 투명도
	LPDIRECT3DTEXTURE9		pTex[4];	/// 0 = diffuse, 1 = normal, 3 = specular, etc...
	ZMaterial()
	{
		for( int i = 0 ; i < 4 ; i++ ) pTex[i] = NULL;
	}
	~ZMaterial()
	{
		for( int i = 0 ; i < 4 ; i++ ) S_REL( pTex[i] );
	}
};

struct ZBaseKey
{
	int				frame;
};

struct ZKeyPos : public ZBaseKey
{
	D3DXVECTOR3		value;
};

struct ZKeyRot : public ZBaseKey
{
	D3DXQUATERNION	value;
};

struct ZKeyScl : public ZBaseKey
{
	D3DXVECTOR3		value;
};

struct ZKeys
{	
	vector<ZKeyPos>		pos;
	vector<ZKeyRot>		rot;
	vector<ZKeyScl>		scl;
	~ZKeys() { stl_wipe_vector( pos ); stl_wipe_vector( rot); stl_wipe_vector( scl ); }
};

struct ZRigidVertex
{
	enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;
};

struct ZSkinnedVertex
{
	enum { FVF = (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1) };
	D3DXVECTOR3	p;
	float		b[3];
	DWORD		i;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;
};

struct ZSWSkinnedVertex
{
	D3DXVECTOR3	p;
	float		b[4];
	DWORD		i[4];
};

struct BOXVERTEX
{
	enum { FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE };

	float x, y, z;	/// 정점의 변환된 좌표
	DWORD color;	/// 정점의 색깔
	BOXVERTEX() { x = 0; y = 0; z = 0; color = 0; }
	BOXVERTEX( float f1, float f2, float f3, DWORD c ) { x = f1; y = f2; z = f3; color = c; }
};
#pragma once

#pragma pack( push )
#pragma pack( 1 )

enum
{
	VF_NORMAL = 0,
	VF_BLEND1 = 1,
	VF_BLEND2 = 2,
	VF_BLEND3 = 3,
	VF_BLEND4 = 4,
};

enum
{
	MESH_TYPE_NORMAL,
	MESH_TYPE_BUMP,
	MESH_TYPE_BILLBOARD,
	MESH_TYPE_AXIAL_BILLBOARD,
	MESH_TYPE_PARTICLE,
	MESH_TYPE_ANIMATION,
};

enum
{
	BILLBOARD_ANIMATION_TYPE_ROT,
	BILLBOARD_ANIMATION_TYPE_POS,
	BILLBOARD_ANIMATION_TYPE_SCALE,
	BILLBOARD_ANIMATION_TYPE_COLOR,
};

// vertex format
struct F3dColorVertex
{
	F3dColorVertex() : vtxPoint(0,0,0), dwColor(0xFF000000) {}

    F3dVector vtxPoint;
    DWORD dwColor;
};

struct F3dTexVertex
{
	F3dTexVertex() : vtxPoint(0,0,0), fU(0), fV(0) {}

	F3dVector vtxPoint;
	float	  fU, fV;
	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_TEX1,
	};
};

struct F3dTexVertexLine
{
	F3dTexVertex vtxV[2];
};

struct F3dVertexLine 
{
	F3dVertexLine() : dTime(0.f) {}

	double dTime;
	F3dVertex vtxV[2];
};

struct F3dNormalVertexNoTex
{
	F3dNormalVertexNoTex() : vtxPoint(0,0,0), vtxNormal(0,0,0) {}
	F3dVector vtxPoint;
	F3dVector vtxNormal;
};

struct F3dNormalVertex
{
	F3dNormalVertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0) {}

    F3dVector vtxPoint;
    F3dVector vtxNormal;
    float     fU, fV;
};

struct F3dNonBlendVertex
{
	F3dNonBlendVertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	UINT	  unBoneIndex;

	void SetVertex( const F3dNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 0 )
			return;
		unBoneIndex = nBoneIdx;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 0 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dBlend2Vertex
{
	F3dBlend2Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), unBoneIndex(0), fBlend(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d (value,value, 0, 0)
	float	  fBlend;

	void SetVertex( const F3dNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
		fBlend		= 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 1 )
			return;
		unBoneIndex	|= nBoneIdx<<(nBlendIdx*8);

		if( nBlendIdx < 1 ) {
			fBlend = fBoneWeight;
		}
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 1 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dBlend3Vertex
{
	F3dBlend3Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d (value,value, value, 0)
	float	  afBlend[2];

	void SetVertex( const F3dNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
		afBlend[0]  = afBlend[1] = 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 2 )
			return;
		unBoneIndex	|= nBoneIdx<<(nBlendIdx*8);
		if( nBlendIdx < 2 )
			afBlend[nBlendIdx]	= fBoneWeight;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 2 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dBlend4Vertex
{
	F3dBlend4Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d, in byte
	float	  afBlend[3];

	void SetVertex( const F3dNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
		afBlend[0]  = afBlend[1] = afBlend[2] = 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 3 )
			return;
		unBoneIndex	|= nBoneIdx<<(nBlendIdx*8);
		if( nBlendIdx < 3 )
			afBlend[nBlendIdx]	= fBoneWeight;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 3 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

// BUMP MAPPING
struct F3dBumpNormalVertex
{
	F3dBumpNormalVertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), vtxTangent(0,0,0), fU(0), fV(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	F3dVector vtxTangent;
	float     fU, fV;
};

struct F3dBumpNonBlendVertex
{
	F3dBumpNonBlendVertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), vtxTangent(0,0,0), fU(0), fV(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	F3dVector vtxTangent;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d (value, 0, 0, 0)

	void SetVertex( const F3dBumpNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		vtxTangent	= Vertex.vtxTangent;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 0 )
			return;
		unBoneIndex = nBoneIdx;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 0 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dBumpBlend2Vertex
{
	F3dBumpBlend2Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), vtxTangent(0,0,0), fU(0), fV(0), unBoneIndex(0), fBlend(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	F3dVector vtxTangent;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d (value,value, 0, 0)
	float	  fBlend;

	void SetVertex( const F3dBumpNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		vtxTangent  = Vertex.vtxTangent;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
		fBlend		= 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 1 )
			return;
		unBoneIndex	|= nBoneIdx<<nBlendIdx<<8;

		if( nBlendIdx < 1 ) {
			fBlend = fBoneWeight;
		}
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 1 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dBumpBlend3Vertex
{
	F3dBumpBlend3Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), vtxTangent(0,0,0), fU(0), fV(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	F3dVector vtxTangent;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d (value,value, value, 0)
	float	  afBlend[2];

	void SetVertex( const F3dBumpNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		vtxTangent  = Vertex.vtxTangent;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
		afBlend[0]  = afBlend[1] = 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 2 )
			return;
		unBoneIndex	|= nBoneIdx<<nBlendIdx<<8;

		if( nBlendIdx < 2 )
			afBlend[nBlendIdx]	= fBoneWeight;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 2 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dBumpBlend4Vertex
{
	F3dBumpBlend4Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), vtxTangent(0,0,0), fU(0), fV(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	F3dVector vtxTangent;
	float	  fU, fV;
	UINT	  unBoneIndex; //4d, in byte
	float	  afBlend[3];

	void SetVertex( const F3dBumpNormalVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		vtxTangent  = Vertex.vtxTangent;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		unBoneIndex = 0;
		afBlend[0]  = afBlend[1] = afBlend[2] = 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 3 )
			return;
		unBoneIndex	|= nBoneIdx<<nBlendIdx<<8;

		if( nBlendIdx < 3 )
			afBlend[nBlendIdx]	= fBoneWeight;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 3 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

//////////////////////////////// lightmap /////////////////////////////////////
struct F3dLightmapVertex
{
	F3dLightmapVertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), fU2(0), fV2(0) {}

    F3dVector vtxPoint;
    F3dVector vtxNormal;
    float     fU, fV;
    float     fU2, fV2;
};

struct F3dLightmapNonBlendVertex
{
	F3dLightmapNonBlendVertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), fU2(0), fV2(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	float     fU2, fV2;
	UINT	  unBoneIndex;

	void SetVertex( const F3dLightmapVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		fU2			= Vertex.fU2;
		fV2			= Vertex.fV2;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 0 )
			return;
		unBoneIndex = nBoneIdx;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 0 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dLightmapBlend2Vertex
{
	F3dLightmapBlend2Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), fU2(0), fV2(0), unBoneIndex(0), fBlend(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	float     fU2, fV2;
	UINT	  unBoneIndex; //4d (value,value, 0, 0)
	float	  fBlend;

	void SetVertex( const F3dLightmapVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		fU2			= Vertex.fU2;
		fV2			= Vertex.fV2;
		unBoneIndex = 0;
		fBlend		= 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 1 )
			return;
		unBoneIndex	|= nBoneIdx<<(nBlendIdx*8);

		if( nBlendIdx < 1 ) {
			fBlend = fBoneWeight;
		}
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 1 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dLightmapBlend3Vertex
{
	F3dLightmapBlend3Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), fU2(0), fV2(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	float     fU2, fV2;
	UINT	  unBoneIndex; //4d (value,value, value, 0)
	float	  afBlend[2];

	void SetVertex( const F3dLightmapVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		fU2			= Vertex.fU2;
		fV2			= Vertex.fV2;
		unBoneIndex = 0;
		afBlend[0]  = afBlend[1] = 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 2 )
			return;
		unBoneIndex	|= nBoneIdx<<(nBlendIdx*8);
		if( nBlendIdx < 2 )
			afBlend[nBlendIdx]	= fBoneWeight;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 2 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dLightmapBlend4Vertex
{
	F3dLightmapBlend4Vertex() : vtxPoint(0,0,0), vtxNormal(0,0,0), fU(0), fV(0), fU2(0), fV2(0), unBoneIndex(0) {}

	F3dVector vtxPoint;
	F3dVector vtxNormal;
	float	  fU, fV;
	float     fU2, fV2;
	UINT	  unBoneIndex; //4d, in byte
	float	  afBlend[3];

	void SetVertex( const F3dLightmapVertex& Vertex )
	{
		vtxPoint	= Vertex.vtxPoint;
		vtxNormal	= Vertex.vtxNormal;
		fU			= Vertex.fU;
		fV			= Vertex.fV;
		fU2			= Vertex.fU2;
		fV2			= Vertex.fV2;
		unBoneIndex = 0;
		afBlend[0]  = afBlend[1] = afBlend[2] = 0.f;
	}
	void SetBoneIndex( int nBlendIdx, int nBoneIdx, float fBoneWeight )
	{
		if( nBlendIdx > 3 )
			return;
		unBoneIndex	|= nBoneIdx<<(nBlendIdx*8);
		if( nBlendIdx < 3 )
			afBlend[nBlendIdx]	= fBoneWeight;
	}
	int GetBoneIndex( int nBlendIdx ) const
	{
		if( nBlendIdx > 3 )
			return 0;
		return ( unBoneIndex>>(nBlendIdx*8)&0x000000FF );
	}
};

struct F3dEdgeVertex
{
	F3dEdgeVertex() : vtxPoint(0,0,0) {}

	F3dVector vtxPoint;
    BYTE	  abyBoneIndex[4];
    float	  afBlend[4];
};

struct F3dEdgeIndex
{
	F3dEdgeIndex() : wIndex(0), wVertex(0) {}

	WORD	wIndex;
	WORD	wVertex;
};

struct F3dIndex2 {
	F3dIndex2() : wA(0), wB(0) {}
	WORD wA, wB;
};

struct F3dName
{
	F3dName()
	: nNameKey( 0 )	{}
	~F3dName() { strName.clear(); }

	int nNameKey;
	std::string strName;
};

struct F3dNameW
{
	F3dNameW()
	: nNameKey( 0 )	{}
	~F3dNameW() { strName.clear(); }

	int nNameKey;
	std::wstring strName;
};
//Billboard Animation
struct F3dBillBoardAnimationBase
{
	F3dBillBoardAnimationBase()
		:fTick(0),nIndex(0)
	{

	}
	float fTick;
	int nIndex;
};

struct F3dBillBoardAnimationPos : public F3dBillBoardAnimationBase
{
	F3dBillBoardAnimationPos()		
		:vtxPoint(F3dVector(0,0,0)){}	
	F3dVector vtxPoint;
};

struct F3dBillBoardAnimationScale : public F3dBillBoardAnimationBase
{
	F3dBillBoardAnimationScale()
		:vtx2Scale(F3dVector2(0,0)){}
	F3dVector2 vtx2Scale;
};

struct F3dBillBoardAnimationColor : public F3dBillBoardAnimationBase
{
	F3dBillBoardAnimationColor()
		:vtx4Color(0,0,0,0){}	
	F3dVector4 vtx4Color;
};

struct F3dBillBoardAnimationRot : public F3dBillBoardAnimationBase
{
	F3dBillBoardAnimationRot()
		:vtxRot(0,0,0){}
	F3dVector vtxRot;
};

inline bool operator==( const F3dName& a, const F3dName& b )
{
	if( a.nNameKey == b.nNameKey && _stricmp( a.strName.c_str(), b.strName.c_str() ) == 0 ) {
		return true;
	}
	return false;
}

inline bool operator!=( const F3dName& a, const F3dName& b )
{
	if( a.nNameKey == b.nNameKey )
		return false;

	if( _stricmp( a.strName.c_str(), b.strName.c_str() ) == 0 ) {
		return false;
	}
	return true;
}

inline bool operator==( const F3dNameW& a, const F3dNameW& b )
{
	if( a.nNameKey == b.nNameKey && _wcsicmp( a.strName.c_str(), b.strName.c_str() ) == 0 ) {
		return true;
	}
	return false;
}

inline bool operator!=( const F3dNameW& a, const F3dNameW& b )
{
	if( a.nNameKey == b.nNameKey )
		return false;

	if( _wcsicmp( a.strName.c_str(), b.strName.c_str() ) == 0 ) {
		return false;
	}
	return true;
}

inline bool operator==( const F3dNameW& a, const WCHAR* b )
{
	if( _wcsicmp( a.strName.c_str(), b ) == 0 ) {
		return true;
	}
	return false;
}

inline bool operator!=( const F3dNameW& a, const WCHAR* b )
{
	if( _wcsicmp( a.strName.c_str(), b ) != 0 ) {
		return true;
	}
	return false;
}

#pragma pack( pop )
typedef boost::function< void (const WCHAR*, int, int) > BuildPVSCB_ptr;
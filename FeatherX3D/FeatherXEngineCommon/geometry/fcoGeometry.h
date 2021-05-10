#pragma once

const float  ALMOST_ZERO = 1.0e-6f;
#define DW_AS_FLT(DW) (*(FLOAT*)&(DW))
#define FLT_AS_DW(F) (*(DWORD*)&(F))
#define FLT_SIGN(F) ((FLT_AS_DW(F) & 0x80000000L))
#define IS_SPECIAL(F) ((FLT_AS_DW(F) & 0x7f800000L)==0x7f800000L)
#define ALMOST_ZERO(F) ((FLT_AS_DW(F) & 0x7f800000L)==0)

typedef D3DXPLANE			F3dPlane;
typedef D3DXVECTOR2			F3dVector2;
typedef D3DXVECTOR3			F3dVector;
typedef D3DXVECTOR3			F3dVertex;
typedef D3DXVECTOR4			F3dVector4;
typedef D3DXMATRIX			F3dMatrix;
typedef D3DXQUATERNION		F3dQuat;
typedef D3DCOLORVALUE		F3dColorValue;
typedef D3DCOLOR			F3dColor;
typedef D3DVIEWPORT9		F3dViewPort;
typedef IDirect3DSurface9	F3dSurface;
typedef IDirect3DQuery9		F3dQuery;

struct F3dIndex3
{
	WORD wA, wB, wC;

	F3dIndex3()
	{
		wA = wB = wC = 0;
	}

	F3dIndex3( WORD wA, WORD wB, WORD wC )
	{
		this->wA = wA; this->wB = wB; this->wC = wC;
	}
};

struct F3dPosKeyFrame {
	float		fTime;
	F3dVector	vtxP;
};

struct F3dRotKeyFrame {
	float		fTime;
	F3dQuat		quatR;
};

struct F3dScaleKeyFrame {
	float		fTime;
	F3dVector	vtxS;
};

struct F3dAlphaKeyFrame {
	float		fTime;
	float		fAlpha;
};

// index
struct F3dIndex32
{
	UINT uA, uB, uC;

	F3dIndex32()
	{
		uA = uB = uC = 0;
	}

	F3dIndex32( UINT uA, UINT uB, UINT uC )
	{
		this->uA = uA; this->uB = uB; this->uC = uC;
	}
};

const float F3D_PI = 3.14159265358979323846f;
const float F3D_2PI = 6.28318530717958647692f;
const float F3D_HALF_PI	= 1.57079632679489661923f;
const float F3D_ALMOSTZERO = 0.0001f;

/************************************************************************/
/* OPERATOR                                                             */
/************************************************************************/

inline const F3dVector& operator^( const F3dVector& vecA, const F3dVector& vecB )
{
	static F3dVector vecResult;
	vecResult.x = vecA.y*vecB.z - vecA.z*vecB.y;
	vecResult.y = vecA.z*vecB.x - vecA.x*vecB.z;
	vecResult.z = vecA.x*vecB.y - vecA.y*vecB.x;
	return vecResult;
}

inline float operator*( const F3dVector& vecA, const F3dVector& vecB )
{
	return D3DXVec3Dot( &vecA, &vecB );
}

inline const F3dVector& operator*( const F3dVector& vecA, const F3dMatrix& mtxM )
{
	static F3dVector vecResult;
	float fTx	= vecA.x*mtxM._11 + vecA.y*mtxM._21 + vecA.z*mtxM._31 + mtxM._41;
	float fTy	= vecA.x*mtxM._12 + vecA.y*mtxM._22 + vecA.z*mtxM._32 + mtxM._42;
	vecResult.z = vecA.x*mtxM._13 + vecA.y*mtxM._23 + vecA.z*mtxM._33 + mtxM._43;
	vecResult.x = fTx;
	vecResult.y = fTy;
	return vecResult;
}

inline const F3dPlane& operator*( const F3dPlane& P, const F3dMatrix& M )
{
	static F3dPlane Out;
	D3DXPlaneTransform( &Out, &P, &M );
	return Out;
}

/************************************************************************/
/* INLINE                                                               */
/************************************************************************/

inline bool IsMostZero( float fValue )
{
	return (fabsf( fValue ) < F3D_ALMOSTZERO);
}

inline float ToRadian( float fDegree )
{
	return (fDegree * F3D_PI / 180.0f);
}

inline F3dVector ToRadian(F3dVector V)
{
	return F3dVector(ToRadian(V.x),ToRadian(V.y),ToRadian(V.z));
}

inline float ToDegree( float fRadian )
{
	return (fRadian * 180.f / F3D_PI);
}
// color
inline F3dColorValue MakeColorValue( float r, float g, float b, float a )
{
	F3dColorValue colResult;
	colResult.r = r;
	colResult.g = g;
	colResult.b = b;
	colResult.a = a;

	return colResult;
}
inline D3DCOLOR GetColor( const F3dColorValue& ColorValue )
{
	D3DCOLOR Color;
	BYTE r, g, b, a;
	r = BYTE(ColorValue.r*255);
	g = BYTE(ColorValue.g*255);
	b = BYTE(ColorValue.b*255);
	a = BYTE(ColorValue.a*255);

	Color = (a<<24)+(r<<16)+(g<<8)+b;
	return Color;
}
inline D3DCOLOR GetColor( const F3dVector4& ColorValue )
{
	D3DCOLOR Color;
	BYTE r, g, b, a;
	r = BYTE(ColorValue.x*255);
	g = BYTE(ColorValue.y*255);
	b = BYTE(ColorValue.z*255);
	a = BYTE(ColorValue.w*255);

	Color = (a<<24)+(r<<16)+(g<<8)+b;
	return Color;
}
inline F3dColorValue GetColorValue( F3dColor color )
{
	return MakeColorValue( GetBValue(color)/255.f, GetGValue(color)/255.f, GetRValue(color)/255.f, 1.f );
}

inline F3dVector4 GetColorValueVector(F3dColor color)
{
	float a	= (float)(((color)&0xff000000)>>24);
	float r = (float)(((color)&0x00ff0000)>>16);
	float g	= (float)(((color)&0x0000ff00)>>8);
	float b = (float)((color)&0x000000ff);

	return F3dVector4(r/255.f,g/255.f,b/255.f,a/255.f);
}

// vector
inline void SetZero( F3dVector& vecResult )
{
	vecResult.x = vecResult.y = vecResult.z = 0.f;
}
inline F3dVector MakeVector( float x, float y, float z )
{
	F3dVector vtxResult( x, y, z );
	return vtxResult;
}
inline F3dVector4 MakeVector4( float x, float y, float z, float w )
{
	F3dVector4 vtxResult( x, y, z, w );
	return vtxResult;
}
inline F3dVector4 MakeVector4( const F3dVector& vtxV, float w )
{
	F3dVector4 vtxResult( vtxV.x, vtxV.y, vtxV.z, w );
	return vtxResult;
}
inline F3dQuat MakeQuat( float x, float y, float z, float w )
{
	F3dQuat quatResult( x, y, z, w );
	return quatResult;
}
inline void VecLerp( F3dVector &vtxResult, const F3dVector &V1, const F3dVector &V2, float fS )
{
	D3DXVec3Lerp( &vtxResult, &V1, &V2, fS );
}
inline void Vec2Lerp( F3dVector2 &vtxResult, const F3dVector2 &V1, const F3dVector2 &V2, float fS )
{
	D3DXVec2Lerp( &vtxResult, &V1, &V2, fS );
}
inline void Vec4Lerp( F3dVector4 &vtxResult, const F3dVector4 &V1, const F3dVector4 &V2, float fS )
{
	D3DXVec4Lerp( &vtxResult, &V1, &V2, fS );
}
inline void VecTransCoord( F3dVector &vtxResult, const F3dVector &vtxV, const F3dMatrix &mtxM )
{
	D3DXVec3TransformCoord( &vtxResult, &vtxV, &mtxM );
}
inline void VecTransNormal( F3dVector &vtxResult, const F3dVector &vtxV, const F3dMatrix &mtxM )
{
	D3DXVec3TransformNormal( &vtxResult, &vtxV, &mtxM );
}
inline void VecTransCoordArray(	F3dVector* vtxResult, UINT OutStride, const F3dVector* vtxV, UINT VStride, const F3dMatrix& mtxTM, UINT n )
{
	D3DXVec3TransformCoordArray( vtxResult, OutStride, vtxV, VStride, &mtxTM, n );
}
inline void VecTransNormalArray( F3dVector* vtxResult, UINT OutStride, const F3dVector* vtxV, UINT VStride, const F3dMatrix& mtxTM, UINT n )
{
	D3DXVec3TransformNormalArray( vtxResult, OutStride, vtxV, VStride, &mtxTM, n );
}

inline const F3dVector& VecNormalize( const F3dVector &vtxV )
{
	static F3dVector vtxResult;
	D3DXVec3Normalize( &vtxResult, &vtxV ); 
	return vtxResult;
}
inline const F3dVector& VecNormalize( F3dVector &vtxV )
{
	D3DXVec3Normalize( &vtxV, &vtxV ); 
	return vtxV;
}
inline const F3dVector2& Vec2Normalize( F3dVector2 &vtxV )
{
	D3DXVec2Normalize( &vtxV, &vtxV ); 
	return vtxV;
}

inline float VecDot( const F3dVector& vecA, const F3dVector& vecB )
{
	return D3DXVec3Dot( &vecA, &vecB );
}

inline float InvSqrt (float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - xhalf*x*x);
	return x;
}

inline float InvSqrt2 (float x)
{
	union{
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
	return 0.5f*convertor.floatPart*(3.0f - x*convertor.floatPart*convertor.floatPart);
}

inline float InvSqrt3 (float x)
{
	union{
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
	return convertor.floatPart*(1.5f - 0.5f*x*convertor.floatPart*convertor.floatPart);
}

inline float fast_sqrtf(float x) 
{
	float xhalf = 0.5F * x ;
	int i = 0x5F3759DF - ((*(int*)&x)>>1);
	float y=*(float*)&i;    
	y = y * (1.5F - y * y * xhalf);
	y = y * (1.5F - y * y * xhalf);
	return (x * y);
}

inline void VecAbs(F3dVector& vecA)
{
	vecA.x = fabsf(vecA.x);
	vecA.y = fabsf(vecA.y);
	vecA.z = fabsf(vecA.z);
}

inline float VecLength( const F3dVector& vecA )
{
	return fast_sqrtf((vecA.x*vecA.x)+(vecA.y*vecA.y)+(vecA.z*vecA.z));
}

inline float VecLength_XZ( const F3dVector& vecA )
{
	return fast_sqrtf((vecA.x*vecA.x)+(vecA.z*vecA.z));
}

inline float VecLengthSq( const F3dVector& vecA )
{
	return (vecA.x*vecA.x)+(vecA.y*vecA.y)+(vecA.z*vecA.z);
}

inline float VecLengthSq_XZ( const F3dVector& vecA )
{
	return (vecA.x*vecA.x)+(vecA.z*vecA.z);
}
inline float Vec2Length( const F3dVector2& vecA )
{
	return fast_sqrtf((vecA.x*vecA.x)+(vecA.y*vecA.y));
}

inline float Vec2LengthSq( const F3dVector2& vecA )
{
	return (vecA.x*vecA.x)+(vecA.y*vecA.y);
}

inline bool IsInRange( const F3dVector& vecV, float fRange )
{
	if( VecLengthSq( vecV ) < fRange*fRange ) {
		return true;
	}
	return false;
}

inline bool IsInRange_XZ( const F3dVector& vecV, float fRange )
{
	if( VecLengthSq_XZ( vecV ) < fRange*fRange ) {
		return true;
	}
	return false;
}

inline const F3dVector& F3dVecCross( const F3dVector& vecA, const F3dVector& vecB )
{
	static F3dVector vecResult;
	D3DXVec3Cross( &vecResult, &vecA, &vecB );
	return vecResult;
}

inline const F3dVector& VecCenter( const F3dVector& vtxMin, const F3dVector& vtxMax )
{
	static F3dVector vtxCenter;
	VecLerp( vtxCenter, vtxMin, vtxMax, 0.5f );

	return vtxCenter;
}

/// matrix
inline void MatrixLerp( F3dMatrix& res, const F3dMatrix &a, const F3dMatrix &b, const float f )
{
	res._11 = a._11 + f * (b._11 - a._11);
	res._21 = a._21 + f * (b._21 - a._21);
	res._31 = a._31 + f * (b._31 - a._31);
	res._41 = a._41 + f * (b._41 - a._41);

	res._12 = a._12 + f * (b._12 - a._12);
	res._22 = a._22 + f * (b._22 - a._22);
	res._32 = a._32 + f * (b._32 - a._32);
	res._42 = a._42 + f * (b._42 - a._42);

	res._13 = a._13 + f * (b._13 - a._13);
	res._23 = a._23 + f * (b._23 - a._23);
	res._33 = a._33 + f * (b._33 - a._33);
	res._43 = a._43 + f * (b._43 - a._43);

	res._14 = a._14 + f * (b._14 - a._14);
	res._24 = a._24 + f * (b._24 - a._24);
	res._34 = a._34 + f * (b._34 - a._34);
	res._44 = a._44 + f * (b._44 - a._44);
}

inline void SetZero( F3dMatrix &mtxResult )
{
	ZeroMemory( mtxResult, sizeof(mtxResult) );
}

inline void Identity( F3dMatrix &mtxResult )
{
	D3DXMatrixIdentity( &mtxResult );
}

inline void Transpose( F3dMatrix &mtxResult, const F3dMatrix &mtxM )
{
	D3DXMatrixTranspose( &mtxResult, &mtxM );
}

inline void Translation( F3dMatrix &mtxResult, float fX, float fY, float fZ )
{
	D3DXMatrixTranslation( &mtxResult, fX, fY, fZ );
}

inline void Translation( F3dMatrix &mtxResult, const F3dVertex &vtxPos )
{
	D3DXMatrixTranslation( &mtxResult, vtxPos.x, vtxPos.y, vtxPos.z );
}

inline void Inverse( F3dMatrix &mtxResult, const F3dMatrix &mtxSource )
{
	D3DXMatrixInverse( &mtxResult, NULL, &mtxSource );
}

inline void Scaling( F3dMatrix &mtxResult, float fX, float fY, float fZ )
{
	D3DXMatrixScaling( &mtxResult, fX, fY, fZ );
}

inline void Scaling( F3dMatrix &mtxResult, const F3dVertex &vtxScale )
{
	D3DXMatrixScaling( &mtxResult, vtxScale.x, vtxScale.y, vtxScale.z );
}

inline void Scale3x3( F3dMatrix &mtxResult, const F3dVector& scale )
{
	mtxResult.m[0][0] *= scale.x;
	mtxResult.m[0][1] *= scale.y;
	mtxResult.m[0][2] *= scale.z;

	mtxResult.m[1][0] *= scale.x;
	mtxResult.m[1][1] *= scale.y;
	mtxResult.m[1][2] *= scale.z;

	mtxResult.m[2][0] *= scale.x;
	mtxResult.m[2][1] *= scale.y;
	mtxResult.m[2][2] *= scale.z;
}

inline void Multiply( F3dMatrix &mtxResult, const F3dMatrix &mtxSource1, const F3dMatrix &mtxSource2 )
{
	D3DXMatrixMultiply( &mtxResult, &mtxSource1, &mtxSource2 ); 
}

inline void RotateX( F3dMatrix &mtxResult, float fRadian )
{
	D3DXMatrixRotationX( &mtxResult, fRadian );
}

inline void RotateY( F3dMatrix &mtxResult, float fRadian )
{
	D3DXMatrixRotationY( &mtxResult, fRadian );
}

inline void RotateZ( F3dMatrix &mtxResult, float fRadian )
{
	D3DXMatrixRotationZ( &mtxResult, fRadian );
}

inline void RotateEuler( F3dMatrix &mtxResult, const F3dVector& vecEuler )
{
	D3DXMatrixRotationYawPitchRoll( &mtxResult, vecEuler.x, vecEuler.y, vecEuler.z );
}

inline void RotationAxis( F3dMatrix &mtxResult, const F3dVector &vtxV, float fRadian )
{
	D3DXMatrixRotationAxis( &mtxResult, &vtxV, fRadian );
}

inline void GetMatrixToPos( F3dVector &vtxResult, const F3dMatrix &mtxM )
{
	vtxResult.x = mtxM._41;
	vtxResult.y = mtxM._42;
	vtxResult.z = mtxM._43;
}

inline void GetMatrixToScale( F3dVector &vtxResult, const F3dMatrix &mtxM )
{
	vtxResult.x = mtxM._11;
	vtxResult.y = mtxM._22;
	vtxResult.z = mtxM._33;
}

inline F3dVector GetColumn( const F3dMatrix &mtxM, int nColumn )
{
	return F3dVector( mtxM.m[0][nColumn], mtxM.m[1][nColumn], mtxM.m[2][nColumn] );
}

inline void MatrixToQuat( F3dQuat &quatResult, const F3dMatrix &mtxSource )
{
	D3DXQuaternionRotationMatrix( &quatResult, &mtxSource );
}


inline F3dVector MatrixToEuler( const F3dMatrix &mtxM )
{
	F3dVector v;

	if( mtxM.m[2][0] < 1.f ) {
		if( mtxM.m[2][0] > -1.0f ) {
			v.z = atan2(mtxM.m[1][0],mtxM.m[0][0]);
			v.y = asin(-mtxM.m[2][0]);
			v.x = atan2(mtxM.m[2][1],mtxM.m[2][2]);
		}
		else {
			// not unique:  zangle - xangle = -atan2(m44[0][1],m44[0][2])
			v.z = -atan2(mtxM.m[0][1],mtxM.m[0][2]);
			v.y = F3D_PI/2.f;
			v.x = 0;
		}
	}
	else {
		// not unique:  zangle + xangle = atan2(-m44[0][1],-m44[0][2])
		v.z = atan2(-mtxM.m[0][1],-mtxM.m[0][2]);
		v.y = -F3D_PI/2.f;
		v.x = 0;
	}

	v = -v;
	// Get the angles in the range [-pi, pi]
	v.x = v.x + F3D_2PI * floorf((-v.x) / F3D_2PI + 0.5f);
	v.y = v.y + F3D_2PI * floorf((-v.y) / F3D_2PI + 0.5f);
	v.z = v.z + F3D_2PI * floorf((-v.z) / F3D_2PI + 0.5f);
	return v;
}

inline void SetRotationMatrixX( F3dMatrix& mtxResult, const float fAngle )
{
	const float s = sinf( fAngle );
	const float c = cosf( fAngle );

	mtxResult.m[0][0] = 1; mtxResult.m[0][1] = 0; mtxResult.m[0][2] = 0; mtxResult.m[0][3] = 0;
	mtxResult.m[1][0] = 0; mtxResult.m[1][1] = c; mtxResult.m[1][2] = s; mtxResult.m[1][3] = 0;
	mtxResult.m[2][0] = 0; mtxResult.m[2][1] =-s; mtxResult.m[2][2] = c; mtxResult.m[2][3] = 0;
	mtxResult.m[3][0] = 0; mtxResult.m[3][1] = 0; mtxResult.m[3][2] = 0; mtxResult.m[3][3] = 1;
}

inline void SetRotationMatrixY( F3dMatrix& mtxResult, const float fAngle )
{
	const float s = sinf( fAngle );
	const float c = cosf( fAngle );

	mtxResult.m[0][0] = c; mtxResult.m[0][1] = 0; mtxResult.m[0][2] =-s; mtxResult.m[0][3] = 0;
	mtxResult.m[1][0] = 0; mtxResult.m[1][1] = 1; mtxResult.m[1][2] = 0; mtxResult.m[1][3] = 0;
	mtxResult.m[2][0] = s; mtxResult.m[2][1] = 0; mtxResult.m[2][2] = c; mtxResult.m[2][3] = 0;
	mtxResult.m[3][0] = 0; mtxResult.m[3][1] = 0; mtxResult.m[3][2] = 0; mtxResult.m[3][3] = 1;
}

inline void SetRotationMatrixZ( F3dMatrix& mtxResult, const float fAngle )
{
	const float s = sinf( fAngle );
	const float c = cosf( fAngle );

	mtxResult.m[0][0] = c; mtxResult.m[0][1] = s; mtxResult.m[0][2] = 0; mtxResult.m[0][3] = 0;
	mtxResult.m[1][0] =-s; mtxResult.m[1][1] = c; mtxResult.m[1][2] = 0; mtxResult.m[1][3] = 0;
	mtxResult.m[2][0] = 0; mtxResult.m[2][1] = 0; mtxResult.m[2][2] = 1; mtxResult.m[2][3] = 0;
	mtxResult.m[3][0] = 0; mtxResult.m[3][1] = 0; mtxResult.m[3][2] = 0; mtxResult.m[3][3] = 1;
}

inline void MultMatrix4x3( F3dMatrix& mtxResult, const F3dMatrix& right)
{
	float v[3];
	for( int i=0; i < 4; ++i )
	{
		v[0] = mtxResult.m[i][0];
		v[1] = mtxResult.m[i][1];
		v[2] = mtxResult.m[i][2];
		mtxResult.m[i][0] = v[0]*right.m[0][0] + v[1]*right.m[1][0] + v[2]*right.m[2][0];
		mtxResult.m[i][1] = v[0]*right.m[0][1] + v[1]*right.m[1][1] + v[2]*right.m[2][1];
		mtxResult.m[i][2] = v[0]*right.m[0][2] + v[1]*right.m[1][2] + v[2]*right.m[2][2];
	}

	mtxResult.m[3][0] += right.m[3][0];
	mtxResult.m[3][1] += right.m[3][1];
	mtxResult.m[3][2] += right.m[3][2];
}

inline void VecMul3x3( F3dVector& vtxResult, const F3dVector &v, const F3dMatrix& mtxM )
{
	vtxResult.x = v.x*mtxM.m[0][0] + v.y*mtxM.m[1][0] + v.z*mtxM.m[2][0];
	vtxResult.y = v.x*mtxM.m[0][1] + v.y*mtxM.m[1][1] + v.z*mtxM.m[2][1];
	vtxResult.z = v.x*mtxM.m[0][2] + v.y*mtxM.m[1][2] + v.z*mtxM.m[2][2];
}

inline void EulerZYX2Matrix( F3dMatrix& mtxResult, const F3dVector& v )
{
	F3dMatrix mtxY, mtxX;
	SetRotationMatrixX( mtxResult, v.z );
	SetRotationMatrixY( mtxY, v.y );
	SetRotationMatrixZ( mtxX, v.x );
	MultMatrix4x3( mtxResult, mtxY );
	MultMatrix4x3( mtxResult, mtxX );
}

inline void EulerXYZ2Matrix( F3dMatrix& mtxResult, const F3dVector& v)
{
	F3dMatrix mtxY, mtxZ;
	SetRotationMatrixX( mtxResult, v.x );
	SetRotationMatrixY( mtxY, v.y );
	SetRotationMatrixZ( mtxZ, v.z );
	MultMatrix4x3( mtxResult, mtxY );
	MultMatrix4x3( mtxResult, mtxZ );
}

inline void SetU( F3dMatrix& mtxResult, const F3dVector& vecU )
{
	mtxResult._11 = vecU.x; mtxResult._12 = vecU.y; mtxResult._13 = vecU.z;
}
inline void SetV( F3dMatrix& mtxResult, const F3dVector& vecV )
{
	mtxResult._21 = vecV.x; mtxResult._22 = vecV.y; mtxResult._23 = vecV.z;
}
inline void SetN( F3dMatrix& mtxResult, const F3dVector& vecN )
{
	mtxResult._31 = vecN.x; mtxResult._32 = vecN.y; mtxResult._33 = vecN.z;
}
inline void SetPos( F3dMatrix& mtxResult, const F3dVector& vecPos )
{
	mtxResult._41 = vecPos.x; mtxResult._42 = vecPos.y; mtxResult._43 = vecPos.z;
}

inline const F3dVector& GetU( const F3dMatrix& mtxM )
{
	static F3dVector vecResult;
	vecResult.x = mtxM._11;	vecResult.y = mtxM._12;	vecResult.z = mtxM._13;
	return vecResult;
}
inline const F3dVector& GetV( const F3dMatrix& mtxM )
{
	static F3dVector vecResult;
	vecResult.x = mtxM._21;	vecResult.y = mtxM._22;	vecResult.z = mtxM._23;
	return vecResult;
}
inline const F3dVector& GetN( const F3dMatrix& mtxM )
{
	static F3dVector vecResult;
	vecResult.x = mtxM._31;	vecResult.y = mtxM._32;	vecResult.z = mtxM._33;
	return vecResult;
}
inline const F3dVector& GetPos( const F3dMatrix& mtxM )
{
	static F3dVector vecResult;
	vecResult.x = mtxM._41;	vecResult.y = mtxM._42;	vecResult.z = mtxM._43;
	return vecResult;
}

inline void CopyPos( F3dMatrix& mtxDest, const F3dMatrix& mtxM )
{
	mtxDest._41 = mtxM._41;	mtxDest._42 = mtxM._42;	mtxDest._43 = mtxM._43;
}

inline void Normalize( F3dMatrix &mtxResult )
{
	F3dVector right		= ::GetU( mtxResult );
	F3dVector up		= ::GetV( mtxResult );
	F3dVector forward	= ::GetN( mtxResult );

	// normalize them
	VecNormalize( right );
	VecNormalize( up );
	VecNormalize( forward );

	// update them again with the normalized versions
	::SetU( mtxResult, right );
	::SetV( mtxResult, up );
	::SetN( mtxResult, forward );
}

inline const F3dVector& GetDirection( const F3dMatrix& mtxM )
{
	static F3dVector vecResult;
	vecResult.x = mtxM._31;	vecResult.y = mtxM._32;	vecResult.z = mtxM._33;
	return vecResult;
}

inline F3dMatrix GetViewMatrix( const F3dMatrix &m )
{
	F3dMatrix mtxResult;
	float tx = -m._41;
	float ty = -m._42;
	float tz = -m._43;

	mtxResult._11 = m._11;	mtxResult._12 = m._21;
	mtxResult._21 = m._12;	mtxResult._22 = m._22;
	mtxResult._31 = m._13;	mtxResult._32 = m._23;

	mtxResult._14 = mtxResult._24 = mtxResult._34 = 0.0f;

	mtxResult._13 = m._31;
	mtxResult._23 = m._32;
	mtxResult._33 = m._33;

	mtxResult._41 = mtxResult._11*tx + mtxResult._21*ty + mtxResult._31*tz;
	mtxResult._42 = mtxResult._12*tx + mtxResult._22*ty + mtxResult._32*tz;
	mtxResult._43 = mtxResult._13*tx + mtxResult._23*ty + mtxResult._33*tz;
	mtxResult._44 = 1.0f;

	return mtxResult;
}

inline void MatrixDecompose( F3dVector& vtxScale, F3dQuat& quatRotation, F3dVector& vtxPos, const F3dMatrix& mtxM )
{
	D3DXMatrixDecompose( &vtxScale, &quatRotation, &vtxPos, &mtxM );
}

// quaternion
inline void QuatSlerp( F3dQuat &quatResult, const F3dQuat &quatQ1, const F3dQuat &quatQ2, float fT )
{ 
	D3DXQuaternionSlerp( &quatResult, &quatQ1, &quatQ2, fT );
}

inline void QuatToMatrix( F3dMatrix &mtxResult, const F3dQuat &quatSource )
{
	D3DXMatrixRotationQuaternion( &mtxResult, &quatSource );
}

inline void AngleAxisToQuat( F3dQuat &quatResult, const F3dVector &pV, float fAngle )
{
	D3DXQuaternionRotationAxis( &quatResult, &pV, fAngle );
}

inline void QuatIdentity( F3dQuat &quatResult )
{
	D3DXQuaternionIdentity( &quatResult );
}

inline void QuatInverse( F3dQuat &quatResult, const F3dQuat& quatR )
{
	D3DXQuaternionInverse( &quatResult, &quatR );
}

//vecAxisSrc를 vecAxisDst로 회전시킬 수 있는 회전 사원수 delta를 계산한다.
inline void QuatRotate( F3dQuat &quatResult, const F3dVector& vecAxisSrc, const F3dVector& vecAxisDst )
{
	QuatIdentity( quatResult );
	float		fCosTheta = vecAxisSrc * vecAxisDst;
	F3dVector	vecTemp	;

	//
	// check if parallel
	//
	if( fCosTheta > 0.99999f ) {
		QuatIdentity( quatResult );
		return;
	}
	else if( fCosTheta < -0.99999f ) {// check if opposite
		//
		// check if we can use cross product of source vector and [1, 0, 0]
		//
		vecTemp.x = 0.0f;
		vecTemp.y = vecAxisSrc.x;
		vecTemp.z = -vecAxisSrc.y;
		if( VecLength( vecTemp ) < ALMOST_ZERO ) {
			//
			// nope! we need cross product of source vector and [0, 1, 0]
			//
			vecTemp = F3dVector( -vecAxisSrc.z, 0.0f, vecAxisSrc.x ) ;
		}

		//
		// normalize
		//
		VecNormalize( vecTemp );

		//
		//	set
		//
		quatResult.x = vecTemp.x;
		quatResult.y = vecTemp.y;
		quatResult.z = vecTemp.z;
		quatResult.w = 0.0f;
		return;
	}

	//
	// ... else we can just cross multiply the axis vectors
	//
	vecTemp = vecAxisSrc ^ vecAxisDst;
	VecNormalize( vecTemp );

	//
	// we have to use half-angle formulae (sin^2 t = ( 1 - cos (2t) ) /2)
	//
	vecTemp *= ::sqrtf( 0.5f * ( 1.0f - fCosTheta ) );

	//
	// scale the axis to get the normalized quaternion
	// w part is cosine of half the rotation angle
	// cos^2 t = ( 1 + cos (2t) ) / 2
	//
	quatResult.x = vecTemp.x;
	quatResult.y = vecTemp.y;
	quatResult.z = vecTemp.z;
	quatResult.w = ::sqrtf( 0.5f * ( 1.0f + fCosTheta) );
}

inline void QuatMultiply( F3dQuat &quatResult, const F3dQuat &Q1, const F3dQuat &Q2 )
{
	D3DXQuaternionMultiply( &quatResult, &Q1, &Q2 );
}

inline void QuatNormalize( F3dQuat &quatResult )
{
	D3DXQuaternionNormalize( &quatResult, &quatResult );
}

inline void QuatToEuler_Degree( F3dVector& vecEulerOut, const F3dQuat& quatIn )
{
	float x,y,z,w;
	x = quatIn.x;
	y = quatIn.y;
	z = quatIn.z;
	w = quatIn.w;

	float sqx = x*x;
	float sqy = y*y;
	float sqz = z*z;
	float sqw = w*w;

	F3dVector vecEulerRad;
	float fSinfY = -2.f*(x*z-y*w);

	// If both parameters of atan2 are 0, the function returns 0
	vecEulerRad.x = atan2f( 2.f*(y*z+x*w), (-sqx-sqy+sqz+sqw) );

	//If x is less than –1 or greater than 1, asin returns an indefinite by default.
	if(fSinfY >= -1 &&  fSinfY <= 1) {
		vecEulerRad.y = asinf( fSinfY);
	}
	else {
		vecEulerRad.y = 0.f;
	}

	// If both parameters of atan2 are 0, the function returns 0
	vecEulerRad.z = atan2f( 2.f*(x*y+z*w),(sqx-sqy-sqz+sqw));

	vecEulerOut.x = ToDegree( vecEulerRad.x );
	vecEulerOut.y = ToDegree( vecEulerRad.y );
	vecEulerOut.z = ToDegree( vecEulerRad.z );
}

inline void EulerToQuat_Degree( F3dQuat& quatQut, const F3dVector& vecEuler )
{
	float fYaw = ToRadian( vecEuler.x );
	float fPitch = ToRadian( vecEuler.y );
	float fRoll = ToRadian( vecEuler.z );
	D3DXQuaternionRotationYawPitchRoll( &quatQut, fYaw, fPitch, fRoll );
	float fTemp = quatQut.y;
	quatQut.y = quatQut.x;
	quatQut.x = fTemp;
}

inline void QuatToEuler( F3dVector& vecEulerOut, const F3dQuat& quatIn )
{
	float x,y,z,w;
	x = quatIn.x;
	y = quatIn.y;
	z = quatIn.z;
	w = quatIn.w;

	float sqx = x*x;
	float sqy = y*y;
	float sqz = z*z;
	float sqw = w*w;

	vecEulerOut.x = atan2f( 2.f*(y*z+x*w), (-sqx-sqy+sqz+sqw) );
	vecEulerOut.y = asinf( -2.f*(x*z-y*w));
	vecEulerOut.z = atan2f( 2.f*(x*y+z*w),(sqx-sqy-sqz+sqw));
}

inline void EulerToQuat( F3dQuat& quatQut, const F3dVector& vecEuler )
{
	D3DXQuaternionRotationYawPitchRoll( &quatQut, vecEuler.x, vecEuler.y, vecEuler.z );
	float fTemp = quatQut.y;
	quatQut.y = quatQut.x;
	quatQut.x = fTemp;
}

// PLANE
inline void PlaneFromPoints( F3dPlane& ResultPlane, const F3dVector &V1, const F3dVector &V2, const F3dVector &V3 )
{
	D3DXPlaneFromPoints( &ResultPlane, &V1, &V2, &V3 );
}

inline void PlaneFromPointNormal( F3dPlane& ResultPlane, const F3dVertex &vtxPoint, const F3dVector &vecNormal )
{
	D3DXPlaneFromPointNormal( &ResultPlane,	&vtxPoint, &vecNormal );
}

inline float PlaneDotCoord( const F3dPlane& Plane, const F3dVector& vtxPoint )
{
	return D3DXPlaneDotCoord( &Plane, &vtxPoint );
}

inline void PlaneNormalize( F3dPlane& ResultPlane )
{
	D3DXPlaneNormalize( &ResultPlane, &ResultPlane );
}

inline float PlaneDotNormal( const F3dPlane& P, const F3dVector& V )
{
	return D3DXPlaneDotNormal( &P, &V );
}

inline float GetAngleXZ( const F3dVector& vecA, const F3dVector& vecB )
{
	float fRadian = atan2(vecB.x*vecA.z - vecB.z*vecA.x, vecB.x*vecA.x + vecB.z*vecA.z );
	return ToDegree(fRadian);
}

inline float VecAngle( const F3dVector& vecA, const F3dVector& vecB )
{
	float fRadian = acosf( vecA * vecB );
	return ToDegree(fRadian);
}

inline float GetAngleXY( const F3dVector& vecA, const F3dVector& vecB )
{
	float fRadian = atan2(vecB.x*vecA.y - vecB.y*vecA.x, vecB.x*vecA.x + vecB.y*vecA.y );
	return ToDegree(fRadian);
}

inline void PlaneTransform( F3dPlane& Out, const F3dPlane& P, const F3dMatrix& M )
{
	D3DXPlaneTransform( &Out, &P, &M );
}

inline bool GetLineNormal( F3dPlane& Plane, const F3dVertex& vecQ1, const F3dVertex& vecQ2, const F3dVertex& vecP )
{
	F3dVertex vecQa = vecQ2 - vecQ1;
	F3dVertex vecPa = vecP - vecQ1;

	float fQ = VecLength( vecQa );
	float fL = vecPa*vecQa/fQ;
	F3dVector vecProjqP = ((vecPa*vecQa)/(fQ*fQ))*vecQa;

	F3dVector vecNormal = vecP - vecProjqP;
	if( VecLength( vecNormal ) < ALMOST_ZERO ) {
		return false;
	}

	VecNormalize( vecNormal );

	PlaneFromPointNormal( Plane, vecQ1, vecNormal );
	return true;
}

inline bool GetLineNormalXZ( F3dPlane& Plane, const F3dVertex& vecQ1, const F3dVertex& vecQ2, const F3dVertex& vecP )
{
	F3dVertex vecQ1a = vecQ1;
	F3dVertex vecQa = vecQ2 - vecQ1;
	F3dVertex vecPa = vecP - vecQ1;
	F3dVertex vecP2 = vecPa;

	vecQ1a.y = 0;
	vecQa.y = 0;
	vecPa.y = 0;
	vecP2.y = 0;

	float fQ = VecLength( vecQa );
	float fL = vecPa*vecQa/fQ;
	F3dVector vecProjqP = ((vecPa*vecQa)/(fQ*fQ))*vecQa;

	F3dVector vecNormal = vecP2 - vecProjqP;
	if( VecLength( vecNormal ) < 2.f ) {
		return false;
	}

	VecNormalize( vecNormal );

	PlaneFromPointNormal( Plane, vecQ1a, vecNormal );
	return true;
}

//:	SolveForX
//----------------------------------------------------------------------------------------
//
//	Given Z and Y, Solve for X on the plane 
//
//-------------------------------------------------------------------------------------://
inline float PlaneSolveForX( const F3dPlane& Plane, float Y, float Z )
{
	//Ax + By + Cz + D = 0
	// Ax = -(By + Cz + D)
	// x = -(By + Cz + D)/A
	if( Plane.a ) {
		return ( -(Plane.b*Y + Plane.c*Z + Plane.d) / Plane.a );
	}

	return (0.0f);
}

//:	SolveForY
//----------------------------------------------------------------------------------------
//
//	Given X and Z, Solve for Y on the plane 
//
//-------------------------------------------------------------------------------------://
inline float PlaneSolveForY( const F3dPlane& Plane, float X, float Z )
{
	//Ax + By + Cz + D = 0
	// By = -(Ax + Cz + D)
	// y = -(Ax + Cz + D)/B

	if( Plane.b ) {
		return ( -(Plane.a*X + Plane.c*Z + Plane.d) / Plane.b );
	}

	return (0.0f);
}

//:	SolveForZ
//----------------------------------------------------------------------------------------
//
//	Given X and Y, Solve for Z on the plane 
//
//-------------------------------------------------------------------------------------://
inline float PlaneSolveForZ( const F3dPlane& Plane, float X, float Y )
{
	//Ax + By + Cz + D = 0
	// Cz = -(Ax + By + D)
	// z = -(Ax + By + D)/C

	if( Plane.c ) {
		return ( -(Plane.a*X + Plane.b*Y + Plane.d) / Plane.c );
	}

	return (0.0f);
}

inline float GetLerp( const float& f1, const float& f2, float s )
{
	return (f1 + s*(f2-f1));
}

inline void AffineTransformation( F3dMatrix& mtxResult, const F3dQuat& quatRot, const F3dVector& vtxScale, const F3dVector& vtxPos )
{
	F3dMatrix tempMat, scaleMat;
	Identity( tempMat );
	QuatToMatrix( tempMat, quatRot );
	Scaling( scaleMat, vtxScale );
	tempMat = scaleMat * tempMat;
	tempMat._41 = vtxPos.x;
	tempMat._42 = vtxPos.y;
	tempMat._43 = vtxPos.z;
	mtxResult = tempMat;
}

inline void VecCatmullRom( F3dVector& vecOut, const F3dVector& vecV0, const F3dVector& vecV1,	const F3dVector& vecV2, const F3dVector& vecV3, float s )
{
	D3DXVec3CatmullRom( &vecOut, &vecV0, &vecV1, &vecV2, &vecV3, s );
}

inline void VecHermite( F3dVector& vecOut, const F3dVector& vecV1, const F3dVector& vecT1,	const F3dVector& vecV2, const F3dVector& vecT2, float s )
{
	D3DXVec3Hermite( &vecOut, &vecV1, &vecT1, &vecV2, &vecT2, s );
}

//Normal
inline void GetNormal(F3dVector &vecOut ,F3dVector &vecV1,F3dVector &vecV2,F3dVector &vecV3 )
{
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 vEdge1 = vecV1 - vecV3;
	D3DXVECTOR3 vEdge2 = vecV2 - vecV3;
	D3DXVec3Cross(&vNormal, &vEdge1, &vEdge2);
	D3DXVec3Normalize(&vNormal,&vNormal);
	vecOut = vNormal;
}

// calculate the cube root
inline float CubeRoot(const float x)
{
	if( x > 0.0f ) {
		return powf(x, 0.333333f);
	}
	else {
		if( x < 0.0f ) {
			return -powf(-x, 0.333333f);
		}
	}

	return 0.0f;
}
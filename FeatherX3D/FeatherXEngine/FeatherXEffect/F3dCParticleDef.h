#pragma once

#define D3DFVF_PARTICLE_PSIZE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_PSIZE)
#define	D3DFVF_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class F3dCModel;
enum EMITTER_TYPE_AXIS
{
	EMITTER_TYPE_AXIS_X,
	EMITTER_TYPE_AXIS_Y,
	EMITTER_TYPE_AXIS_NONE,
	EMITTER_TYPE_AXIS_ALL,
};

enum ALPHA_TYPE
{
	ALPHA_TYPE_BLENDING,
	ALPHA_TYPE_ADDITIVE,
	ALPHA_TYPE_SUBTRACTIVE,
	ALPHA_TYPE_SRCALPHA_ONE,
};

enum EMITTER_TYPE
{
	EMITTER_TYPE_BOX,
	EMITTER_TYPE_CYLINDER,
	EMITTER_TYPE_SPHERE,

};

enum EFFECT_SHOOT_TYPE
{
	EFFECT_SHOOT_TYPE_RADIAL_SHAPE,
	EFFECT_SHOOT_TYPE_USER_DEFINE,
	EFFECT_SHOOT_TYPE_BOX_THE_COMPASS,
};

enum {
	MAX_PARTICLE = 20000,
};

/************************************************************************/
/* INDEX BUFFER의 정보                                                  */
/************************************************************************/
static float LineListData[6][2] = 
{
	{ 0.000f,  0.000f},
	{ 0.000f,  1.000f},    
	{ 1.000f, 1.000f},

	{ 1.000f,  1.000f},
	{ 1.000f,  0.000f},    
	{ 0.000f,  0.000f},
};

/************************************************************************/
/* SHADER INPUT 으로 사용되는 자료형                                     */
/************************************************************************/
struct F3dParticleVertex
{
	D3DXVECTOR4	vtx4Point;
	D3DXVECTOR2	vtx2Vertex;
	D3DXVECTOR2 vtx2Factor;
	D3DXVECTOR4 vtx4Color;
	float		fRot;
	D3DXVECTOR2 vtx2AxisRot;	
};

/************************************************************************/
/* PARTICLE 입자 하나의 자료형                                           */
/************************************************************************/

struct F3dParticleParentInfo
{
	F3dCModel* pParentModel;
	F3dVector vecOffset;
	std::wstring strLinkedNode;
};

struct F3dParticle
{
	F3dParticle()
	{
		vtxPos.x = 0;
		vtxPos.y = 0;
		vtxPos.z = 0;

		vtxStartPos.x = 0;
		vtxStartPos.y = 0;
		vtxStartPos.z = 0;

		vecStartVelocity.x = 0;
		vecStartVelocity.y = 0;
		vecStartVelocity.z = 0;

		dStartTime = 0;
		dEndTime = 0;
		fLengthSq = 0;

		fRot = 0;
		fStartRot = 0;
	}

	inline static bool Greater( const F3dParticle* p1, const F3dParticle* p2 )
	{
		return (p1->fLengthSq > p2->fLengthSq);
	}

	D3DXVECTOR3		vtxPos;
	D3DXVECTOR3		vtxStartPos;
	D3DXVECTOR3		vecStartVelocity;
	double			dStartTime;
	double			dEndTime;
	float			fLengthSq;
	float			fRot;
	float			fStartRot;
};
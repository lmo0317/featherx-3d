#include "FeatherXEngineCommonPrecompile.h"

float Rand()
{
	return (rand()/(float)RAND_MAX);
}

float GetRandom(float min,float max)
{
	float fRange = max - min;
	float fValue = Rand();

	return (min + (fValue * fRange));
}

const F3dVector& GetRotateAxisAngle( const F3dVector& vecP, const F3dVector& vecAxis, float fAngle )
{
	static F3dVector vtxResult;
	
	vtxResult = cosf(fAngle)*vecP+(vecAxis^vecP)*sinf(fAngle)+vecAxis*(vecAxis*vecP)*(1.f-cosf(fAngle));
	return vtxResult;
}
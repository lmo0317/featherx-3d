#pragma once

class F3dCParticleTemplate
{
public:
	F3dCParticleTemplate();
	~F3dCParticleTemplate();

	void Init(WCHAR* strFile);

	bool		bRandom;
	int			nEmitterType;
	int			nShootType;
	int			nAlphaType;
	int			nEmitterAxisType;
	int			nCurrentFrame;
	int			nEmitterNumberOfShoot;		
	int			nTotalFrameNum;
	int			nWidthFrameNum;
	int			nHeightFrameNum;
	int			nFrameNum;;

	float		fFrameProcessTime;
	float		fNumberOfRoutine;
	float		fEmitterLifeTime;			
	float		fEmitterShootTimeInterval;		
	float		fMinRot;
	float		fMaxRot;
	float		fConeAngle;
	float		fMinSpeed;
	float		fMaxSpeed;
	float		fEmitterXLength;
	float		fEmitterYLength;
	float		fEmitterZLength;
	float		fBillBoardLifeTime;

	F3dVector vecDirection;
	F3dVector4	vec4StartColor;	
	F3dVector4	vec4EndColor;	
	F3dVector4	vec4MiddleColor;	
	F3dVector	vecGravity;
	F3dVector2	vecStartSize;		
	F3dVector2	vecEndSize;

	std::wstring strTextureFileName;
};
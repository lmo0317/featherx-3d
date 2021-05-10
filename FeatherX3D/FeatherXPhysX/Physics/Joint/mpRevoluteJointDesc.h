#pragma once

class mpRevoluteJointDesc : public mpJointDesc
{
public:
	mpRevoluteJointDesc();	
	void SetToDefault();
	bool IsValid() const;

	mpJointDesc* Copy() const;
	void Copy( const mpJointDesc* pJointDesc );

	void GetNxRevoluteJointDesc( NxRevoluteJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const;

	NxJointLimitPairDesc Limit;
	NxMotorDesc			 Motor; 
	NxSpringDesc		 Spring;
	float fProjectionDistance;	
	float fProjectionAngle;
	UINT uFlags;
	NxJointProjectionMode ProjectionMode;
};

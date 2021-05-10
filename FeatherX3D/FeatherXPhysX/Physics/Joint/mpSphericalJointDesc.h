#pragma once

class mpSphericalJointDesc : public mpJointDesc
{
public:
	mpSphericalJointDesc();
	void SetToDefault();
	bool IsValid() const;

	mpJointDesc* Copy() const;
	void Copy( const mpJointDesc* pJointDesc );

	void GetNxSphericalJointDesc( NxSphericalJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const;

	F3dVector vecSwingAxis;
	float fProjectionDistance;	
	NxJointLimitPairDesc TwistLimit;
	NxJointLimitDesc SwingLimit;
	NxSpringDesc	 TwistSpring;
	NxSpringDesc	 SwingSpring;
	NxSpringDesc	 JointSpring;
	UINT uFlags;
	NxJointProjectionMode ProjectionMode;
};

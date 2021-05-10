#pragma once

class mpD6JointDesc : public mpJointDesc
{
public:
	NxD6JointMotion	XMotion, YMotion, ZMotion;
	NxD6JointMotion	Swing1Motion, Swing2Motion, TwistMotion;
	NxJointLimitSoftDesc LinearLimit;
	NxJointLimitSoftDesc Swing1Limit;
	NxJointLimitSoftDesc Swing2Limit;
	NxJointLimitSoftPairDesc TwistLimit;
	NxJointDriveDesc XDrive, YDrive, ZDrive;
	NxJointDriveDesc SwingDrive, TwistDrive;
	NxJointDriveDesc		SlerpDrive;
	F3dVector				vecDrivePosition;
	F3dQuat					quatDriveOrientation;
	F3dVector				vecDriveLinearVelocity;
	F3dVector				vecDriveAngularVelocity;
	NxJointProjectionMode	ProjectionMode;
	float					fProjectionDistance;
	float					fProjectionAngle;
	float					fGearRatio;
	UINT					uFlags;

	mpD6JointDesc();
	void	SetToDefault();
	bool	IsValid() const;

	mpJointDesc* Copy() const;
	void Copy( const mpJointDesc* pJointDesc );

	void GetNxD6JointDesc( NxD6JointDesc& JointDesc, const F3dMatrix& mtxWorld ) const;
};

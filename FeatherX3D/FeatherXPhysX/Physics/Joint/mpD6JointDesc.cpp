#include "MagiPhysXDef.h"

mpD6JointDesc::mpD6JointDesc() : mpJointDesc(NX_JOINT_D6)
{
	SetToDefault();
}

void mpD6JointDesc::SetToDefault() 
{
	mpJointDesc::SetToDefault();

	XMotion = NX_D6JOINT_MOTION_FREE;
	YMotion = NX_D6JOINT_MOTION_FREE;
	ZMotion = NX_D6JOINT_MOTION_FREE;
	TwistMotion = NX_D6JOINT_MOTION_FREE;
	Swing1Motion = NX_D6JOINT_MOTION_FREE;
	Swing2Motion = NX_D6JOINT_MOTION_FREE;

	vecDrivePosition = F3dVector( 0, 0, 0 );
	QuatIdentity( quatDriveOrientation );

	vecDriveLinearVelocity = F3dVector( 0, 0, 0 );
	vecDriveAngularVelocity = F3dVector( 0, 0, 0 );

	ProjectionMode = NX_JPM_NONE;	// choose NX_JPM_POINT_MINDIST to turn projection on
	fProjectionDistance = 0.1f;
	fProjectionAngle = 0.0872f;	//about 5 degrees in radians.

	uFlags = 0;
	fGearRatio = 1.0f;
}

bool mpD6JointDesc::IsValid() const
{
	if (uFlags & NX_D6JOINT_SLERP_DRIVE) {	// only possible with all angular DOF available
		if (Swing1Motion == NX_D6JOINT_MOTION_LOCKED ||
			Swing2Motion == NX_D6JOINT_MOTION_LOCKED ||
			TwistMotion == NX_D6JOINT_MOTION_LOCKED) return false;
	}

	// swing limits are symmetric, thus their range is 0..180 degrees
	if( Swing1Motion == NX_D6JOINT_MOTION_LIMITED ) {
		if( Swing1Limit.value < 0.0f ) return false;
		if( Swing1Limit.value > NxPi ) return false;
	}
	if( Swing2Motion == NX_D6JOINT_MOTION_LIMITED ) {
		if( Swing2Limit.value < 0.0f ) return false;
		if( Swing2Limit.value > NxPi ) return false;
	}

	// twist limits are asymmetric with -180 <= low < high <= 180 degrees
	if( TwistMotion == NX_D6JOINT_MOTION_LIMITED ) {
		if( TwistLimit.low.value < -NxPi ) return false;
		if( TwistLimit.high.value > NxPi ) return false;
		if( TwistLimit.low.value > TwistLimit.high.value ) return false;
	}

	// in angular limited-free mode, only -90..90 swings are possible
	if( Swing1Motion == NX_D6JOINT_MOTION_LIMITED && Swing2Motion == NX_D6JOINT_MOTION_FREE )
		if (Swing1Limit.value > NxHalfPi) return false;
	if( Swing2Motion == NX_D6JOINT_MOTION_LIMITED && Swing1Motion == NX_D6JOINT_MOTION_FREE )
		if (Swing2Limit.value > NxHalfPi) return false;

	if( uFlags & NX_D6JOINT_GEAR_ENABLED )		// gear only with twist motion enabled
		if (TwistMotion == NX_D6JOINT_MOTION_LOCKED) return false;

	return mpJointDesc::IsValid();
}

mpJointDesc* mpD6JointDesc::Copy() const
{
	mpD6JointDesc* pD6JointDesc = new mpD6JointDesc;

	// Joint
	pD6JointDesc->mpJointDesc::Copy( this );

	pD6JointDesc->XMotion					= this->XMotion;
	pD6JointDesc->YMotion					= this->YMotion;
	pD6JointDesc->ZMotion					= this->ZMotion;
	pD6JointDesc->Swing1Motion				= this->Swing1Motion;
	pD6JointDesc->Swing2Motion				= this->Swing2Motion;
	pD6JointDesc->TwistMotion				= this->TwistMotion;
	pD6JointDesc->LinearLimit				= this->LinearLimit;
	pD6JointDesc->Swing1Limit				= this->Swing1Limit;
	pD6JointDesc->Swing2Limit				= this->Swing2Limit;
	pD6JointDesc->TwistLimit				= this->TwistLimit;
	pD6JointDesc->XDrive					= this->XDrive;
	pD6JointDesc->YDrive					= this->YDrive;
	pD6JointDesc->ZDrive					= this->ZDrive;
	pD6JointDesc->SwingDrive				= this->SwingDrive;
	pD6JointDesc->TwistDrive				= this->TwistDrive;
	pD6JointDesc->SlerpDrive				= this->SlerpDrive;
	pD6JointDesc->vecDrivePosition			= this->vecDrivePosition;
	pD6JointDesc->quatDriveOrientation		= this->quatDriveOrientation;
	pD6JointDesc->vecDriveLinearVelocity	= this->vecDriveLinearVelocity;
	pD6JointDesc->vecDriveAngularVelocity	= this->vecDriveAngularVelocity;
	pD6JointDesc->ProjectionMode			= this->ProjectionMode;
	pD6JointDesc->fProjectionDistance		= this->fProjectionDistance;
	pD6JointDesc->fProjectionAngle			= this->fProjectionAngle;
	pD6JointDesc->fGearRatio				= this->fGearRatio;
	pD6JointDesc->uFlags					= this->uFlags;
	return pD6JointDesc;
}

void mpD6JointDesc::Copy( const mpJointDesc* pJointDesc )
{
	if( pJointDesc->GetType() != NX_JOINT_SPHERICAL )
		return;

	const mpD6JointDesc* pD6JointDesc = static_cast< const mpD6JointDesc* >( pJointDesc );

	// Joint
	mpJointDesc::Copy( pD6JointDesc );

	this->XMotion					= pD6JointDesc->XMotion;
	this->YMotion					= pD6JointDesc->YMotion;
	this->ZMotion					= pD6JointDesc->ZMotion;
	this->Swing1Motion				= pD6JointDesc->Swing1Motion;
	this->Swing2Motion				= pD6JointDesc->Swing2Motion;
	this->TwistMotion				= pD6JointDesc->TwistMotion;
	this->LinearLimit				= pD6JointDesc->LinearLimit;
	this->Swing1Limit				= pD6JointDesc->Swing1Limit;
	this->Swing2Limit				= pD6JointDesc->Swing2Limit;
	this->TwistLimit				= pD6JointDesc->TwistLimit;
	this->XDrive					= pD6JointDesc->XDrive;
	this->YDrive					= pD6JointDesc->YDrive;
	this->ZDrive					= pD6JointDesc->ZDrive;
	this->SwingDrive				= pD6JointDesc->SwingDrive;
	this->TwistDrive				= pD6JointDesc->TwistDrive;
	this->SlerpDrive				= pD6JointDesc->SlerpDrive;
	this->vecDrivePosition			= pD6JointDesc->vecDrivePosition;
	this->quatDriveOrientation		= pD6JointDesc->quatDriveOrientation;
	this->vecDriveLinearVelocity	= pD6JointDesc->vecDriveLinearVelocity;
	this->vecDriveAngularVelocity	= pD6JointDesc->vecDriveAngularVelocity;
	this->ProjectionMode			= pD6JointDesc->ProjectionMode;
	this->fProjectionDistance		= pD6JointDesc->fProjectionDistance;
	this->fProjectionAngle			= pD6JointDesc->fProjectionAngle;
	this->fGearRatio				= pD6JointDesc->fGearRatio;
	this->uFlags					= pD6JointDesc->uFlags;
}

void mpD6JointDesc::GetNxD6JointDesc( NxD6JointDesc& JointDesc, const F3dMatrix& mtxWorld ) const
{
	mpJointDesc::GetNxJointDesc( JointDesc, mtxWorld );

	JointDesc.xMotion				= this->XMotion;
	JointDesc.xMotion				= this->YMotion;
	JointDesc.xMotion				= this->ZMotion;
	JointDesc.swing1Motion			= this->Swing1Motion;
	JointDesc.swing2Motion			= this->Swing2Motion;
	JointDesc.twistMotion			= this->TwistMotion;
	JointDesc.linearLimit			= this->LinearLimit;
	JointDesc.swing1Limit			= this->Swing1Limit;
	JointDesc.swing2Limit			= this->Swing2Limit;
	JointDesc.twistLimit			= this->TwistLimit;
	JointDesc.xDrive				= this->XDrive;
	JointDesc.yDrive				= this->YDrive;
	JointDesc.zDrive				= this->ZDrive;
	JointDesc.swingDrive			= this->SwingDrive;
	JointDesc.twistDrive			= this->TwistDrive;
	JointDesc.slerpDrive			= this->SlerpDrive;
	JointDesc.drivePosition			= ToNxVector( this->vecDrivePosition );
	JointDesc.driveOrientation		= ToNxQuat( this->quatDriveOrientation );
	JointDesc.driveLinearVelocity	= ToNxVector( this->vecDriveLinearVelocity );
	JointDesc.driveAngularVelocity	= ToNxVector( this->vecDriveAngularVelocity );
	JointDesc.projectionMode		= this->ProjectionMode;
	JointDesc.projectionDistance	= this->fProjectionDistance;
	JointDesc.projectionAngle		= this->fProjectionAngle;
	JointDesc.gearRatio				= this->fGearRatio;
	JointDesc.flags					= this->uFlags;
}

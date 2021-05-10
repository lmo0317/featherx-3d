#include "MagiPhysXDef.h"

mpRevoluteJointDesc::mpRevoluteJointDesc() : mpJointDesc( NX_JOINT_REVOLUTE )
{
	SetToDefault();
}

void mpRevoluteJointDesc::SetToDefault()
{
	mpJointDesc::SetToDefault();
	fProjectionDistance = 1.0f;
	fProjectionAngle = 0.0872f;	//about 5 degrees in radians.

	Limit.setToDefault();
	Motor.setToDefault();
	Spring.setToDefault();

	uFlags = 0;
	ProjectionMode = NX_JPM_NONE;
}

bool mpRevoluteJointDesc::IsValid() const
{
	if( fProjectionDistance < 0.0f ) return false;
	if( fProjectionAngle < 0.02f ) return false;

	if( Limit.isValid() == false ) return false;
	if( Motor.isValid() == false ) return false;
	if( Spring.isValid() == false ) return false;

	return mpJointDesc::IsValid();
}

mpJointDesc* mpRevoluteJointDesc::Copy() const
{
	mpRevoluteJointDesc* pRevoluteJointDesc = new mpRevoluteJointDesc;

// Joint
	pRevoluteJointDesc->mpJointDesc::Copy( this );

// RevoluteJoint
	pRevoluteJointDesc->Limit				= this->Limit;
	pRevoluteJointDesc->Motor				= this->Motor;
	pRevoluteJointDesc->Spring				= this->Spring;
	pRevoluteJointDesc->fProjectionDistance = this->fProjectionDistance;
	pRevoluteJointDesc->fProjectionAngle	= this->fProjectionAngle;
	pRevoluteJointDesc->uFlags				= this->uFlags;
	pRevoluteJointDesc->ProjectionMode		= this->ProjectionMode;

	return pRevoluteJointDesc;
}

void mpRevoluteJointDesc::Copy( const mpJointDesc* pJointDesc )
{
	if( pJointDesc->GetType() != NX_JOINT_REVOLUTE )
		return;

	const mpRevoluteJointDesc* pRevoluteJointDesc = static_cast< const mpRevoluteJointDesc* >( pJointDesc );

	// Joint
	mpJointDesc::Copy( pRevoluteJointDesc );

	// RevoluteJoint
	this->Limit						= pRevoluteJointDesc->Limit;
	this->Motor						= pRevoluteJointDesc->Motor;
	this->Spring					= pRevoluteJointDesc->Spring;
	this->fProjectionDistance		= pRevoluteJointDesc->fProjectionDistance;
	this->fProjectionAngle			= pRevoluteJointDesc->fProjectionAngle;
	this->uFlags					= pRevoluteJointDesc->uFlags;
	this->ProjectionMode			= pRevoluteJointDesc->ProjectionMode;
}

void mpRevoluteJointDesc::GetNxRevoluteJointDesc( NxRevoluteJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const
{
	mpJointDesc::GetNxJointDesc( JointDesc, mtxWorld );

	JointDesc.limit = this->Limit;
	JointDesc.motor = this->Motor; 
	JointDesc.spring= this->Spring;
	JointDesc.projectionDistance = this->fProjectionDistance;	
	JointDesc.projectionAngle = this->fProjectionAngle;
	JointDesc.flags = this->uFlags;
	JointDesc.projectionMode = this->ProjectionMode;
}

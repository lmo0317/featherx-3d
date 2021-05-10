#include "MagiPhysXDef.h"

mpSphericalJointDesc::mpSphericalJointDesc() :
	mpJointDesc( NX_JOINT_SPHERICAL )
{
	SetToDefault();
}

void mpSphericalJointDesc::SetToDefault()
{
	mpJointDesc::SetToDefault();

	vecSwingAxis = F3dVector( 0, 0, 1.f );

	TwistLimit.setToDefault();
	SwingLimit.setToDefault();
	TwistSpring.setToDefault();
	SwingSpring.setToDefault();
	JointSpring.setToDefault();
	fProjectionDistance = 0.001f;
	uFlags = 0;
	ProjectionMode = NX_JPM_NONE;
}

bool mpSphericalJointDesc::IsValid() const
{
	//check unit vectors
	if( VecLengthSq( vecSwingAxis ) < 0.9f ) return false;
	if( fProjectionDistance < 0.0f ) return false;

	if( TwistLimit.isValid() == false ) return false;
	if( SwingLimit.isValid() == false ) return false;
	if( SwingSpring.isValid() == false ) return false;
	if( TwistSpring.isValid() == false ) return false;
	if( JointSpring.isValid() == false ) return false;

	return mpJointDesc::IsValid();
}

mpJointDesc* mpSphericalJointDesc::Copy() const
{
	mpSphericalJointDesc* pSphericalJointDesc = new mpSphericalJointDesc;

	// Joint
	pSphericalJointDesc->mpJointDesc::Copy( this );

	// SphericalJoint
	pSphericalJointDesc->vecSwingAxis		= this->vecSwingAxis;
	pSphericalJointDesc->fProjectionDistance= this->fProjectionDistance;
	pSphericalJointDesc->TwistLimit			= this->TwistLimit;
	pSphericalJointDesc->SwingLimit			= this->SwingLimit;
	pSphericalJointDesc->TwistSpring		= this->TwistSpring;
	pSphericalJointDesc->SwingSpring		= this->SwingSpring;
	pSphericalJointDesc->JointSpring		= this->JointSpring;
	pSphericalJointDesc->uFlags				= this->uFlags;
	pSphericalJointDesc->ProjectionMode		= this->ProjectionMode;

	return pSphericalJointDesc;
}

void mpSphericalJointDesc::Copy( const mpJointDesc* pJointDesc )
{
	if( pJointDesc->GetType() != NX_JOINT_SPHERICAL )
		return;

	const mpSphericalJointDesc* pSphericalJointDesc = static_cast< const mpSphericalJointDesc* >( pJointDesc );

	// Joint
	mpJointDesc::Copy( pSphericalJointDesc );

	// SphericalJoint
	this->vecSwingAxis				= pSphericalJointDesc->vecSwingAxis;
	this->fProjectionDistance		= pSphericalJointDesc->fProjectionDistance;
	this->TwistLimit				= pSphericalJointDesc->TwistLimit;
	this->SwingLimit				= pSphericalJointDesc->SwingLimit;
	this->TwistSpring				= pSphericalJointDesc->TwistSpring;
	this->SwingSpring				= pSphericalJointDesc->SwingSpring;
	this->JointSpring				= pSphericalJointDesc->JointSpring;
	this->uFlags					= pSphericalJointDesc->uFlags;
	this->ProjectionMode			= pSphericalJointDesc->ProjectionMode;
}

void mpSphericalJointDesc::GetNxSphericalJointDesc( NxSphericalJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const
{
	mpJointDesc::GetNxJointDesc( JointDesc, mtxWorld );

	JointDesc.swingAxis = ToNxVector( this->vecSwingAxis );
	JointDesc.projectionDistance = this->fProjectionDistance;	
	JointDesc.twistLimit	= this->TwistLimit;
	JointDesc.swingLimit	= this->SwingLimit;
	JointDesc.twistSpring	= this->TwistSpring;
	JointDesc.swingSpring	= this->SwingSpring;
	JointDesc.jointSpring	= this->JointSpring;
	JointDesc.flags			= this->uFlags;
	JointDesc.projectionMode= this->ProjectionMode;
}

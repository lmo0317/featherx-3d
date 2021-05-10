#include "MagiPhysXDef.h"

mpJointDesc::mpJointDesc( NxJointType t ) :
	Type(t)
{
	SetToDefault();
}

mpJointDesc::~mpJointDesc()
{
}

void mpJointDesc::SetToDefault()
{
	for( int i = 0; i < 2; ++i )
	{
		ActorID[i] = 0;
	}

	fMaxForce	= FLT_MAX;
	fMaxTorque	= FLT_MAX;
	fSolverExtrapolationFactor = 1.f;
	uUseAccelerationSpring = 0;
	uJointFlags	= NX_JF_VISUALIZATION;
}

bool mpJointDesc::IsValid() const
{
	if( ActorID[0] == ActorID[1] )
		return false;
	if( !(ActorID[0] || ActorID[1]) )
		return false;
	//non-null pointers must be dynamic:

	NxActor* pNxActor1 = mpManager::GetSingleton()->FindActor( ActorID[0] );
	NxActor* pNxActor2 = mpManager::GetSingleton()->FindActor( ActorID[1] );
	if( pNxActor1 == NULL )
		return false;
	if( pNxActor2 == NULL )
		return false;

	if( pNxActor1->isDynamic() == false )
		return false;
	if( pNxActor2->isDynamic() == false )
		return false;

	if( Type >= NX_JOINT_COUNT )
		return false;

	if( fMaxForce <= 0 )
		return false;
	if( fMaxTorque <= 0 )
		return false;
	if( (fSolverExtrapolationFactor < 0.5f) || (fSolverExtrapolationFactor > 2.0f) )
		return false;
	if( uUseAccelerationSpring > 1 )
		return false;

	return true;
}

void mpJointDesc::Copy( const mpJointDesc* pJointDesc )
{
	this->vtxGlobalAnchor			= pJointDesc->vtxGlobalAnchor;
	this->vecGlobalAxis				= pJointDesc->vecGlobalAxis;

	this->ActorID[0]				= pJointDesc->ActorID[0];
	this->ActorID[1]				= pJointDesc->ActorID[1];
	this->fMaxForce					= pJointDesc->fMaxForce;
	this->fMaxTorque				= pJointDesc->fMaxTorque;
	this->fSolverExtrapolationFactor= pJointDesc->fSolverExtrapolationFactor;
	this->uUseAccelerationSpring	= pJointDesc->uUseAccelerationSpring;
	this->uJointFlags				= pJointDesc->uJointFlags;
}

void mpJointDesc::GetNxJointDesc( NxJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const
{
	NxActor* pActor1 = mpManager::GetSingleton()->FindActor( this->ActorID[0] );
	NxActor* pActor2 = mpManager::GetSingleton()->FindActor( this->ActorID[1] );

	if( pActor1 == NULL || pActor2 == NULL )
		return;

	JointDesc.actor[0] = pActor1;
	JointDesc.actor[1] = pActor2;

	float fScale = mpManager::GetSingleton()->GetUnitScale();
	F3dVector vtxTempGlobalAnchor = this->vtxGlobalAnchor * mtxWorld;
	NxVec3 vtxGlobalAnchor = ToNxVector( vtxTempGlobalAnchor );
	vtxGlobalAnchor.x /= fScale;
	vtxGlobalAnchor.y /= fScale;
	vtxGlobalAnchor.z /= fScale;
	JointDesc.setGlobalAnchor( vtxGlobalAnchor );
	JointDesc.setGlobalAxis( ToNxVector( this->vecGlobalAxis ) );

	JointDesc.maxForce = this->fMaxForce;
	JointDesc.maxTorque = this->fMaxTorque;
	JointDesc.solverExtrapolationFactor = this->fSolverExtrapolationFactor;
	JointDesc.useAccelerationSpring = this->uUseAccelerationSpring;
	JointDesc.jointFlags = this->uJointFlags;
}

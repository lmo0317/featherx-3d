#include "MagiPhysXDef.h"

mpFixedJointDesc::mpFixedJointDesc() : mpJointDesc( NX_JOINT_FIXED )
{
	SetToDefault();
}

void mpFixedJointDesc::SetToDefault()
{
	mpJointDesc::SetToDefault();
}

bool mpFixedJointDesc::IsValid() const
{
	return mpJointDesc::IsValid();
}

mpJointDesc* mpFixedJointDesc::Copy() const
{
	mpFixedJointDesc* pFixedJointDesc = new mpFixedJointDesc;

	// Joint
	pFixedJointDesc->mpJointDesc::Copy( this );
	return pFixedJointDesc;
}

void mpFixedJointDesc::Copy( const mpJointDesc* pJointDesc )
{
	if( pJointDesc->GetType() != NX_JOINT_REVOLUTE )
		return;

	const mpFixedJointDesc* pFixedJointDesc = static_cast< const mpFixedJointDesc* >( pJointDesc );

	// Joint
	mpJointDesc::Copy( pFixedJointDesc );
}

void mpFixedJointDesc::GetNxFixedJointDesc( NxFixedJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const
{
	mpJointDesc::GetNxJointDesc( JointDesc, mtxWorld );
}

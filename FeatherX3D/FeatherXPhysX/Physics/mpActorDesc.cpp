#include "MagiPhysXDef.h"

float g_fUnitScale = 1.f;

mpBodyDesc::mpBodyDesc()
{
	SetToDefault();
}

void mpBodyDesc::SetNx( const NxBodyDesc& BodyDesc )
{
	ToF3dMatrix( mtxMassLocalTM, BodyDesc.massLocalPose );

	mtxMassLocalTM._41 *= g_fUnitScale;
	mtxMassLocalTM._42 *= g_fUnitScale;
	mtxMassLocalTM._43 *= g_fUnitScale;

	vecMassSpaceInertia = ToF3dVector( BodyDesc.massSpaceInertia );
	fMass				=  BodyDesc.mass;
	vecLinearVelocity	= ToF3dVector( BodyDesc.linearVelocity );
	vecAngularVelocity	= ToF3dVector( BodyDesc.angularVelocity );
	fWakeUpCounter		= BodyDesc.wakeUpCounter;
	fLinearDamping		= BodyDesc.linearDamping;
	fAngularDamping		= BodyDesc.angularDamping;
	fCCDMotionThreshold = BodyDesc.CCDMotionThreshold;
	uFlags				= BodyDesc.flags;
	fSleepLinearVelocity = BodyDesc.sleepLinearVelocity;
	fSleepAngularVelocity = BodyDesc.sleepAngularVelocity;
	uSolverIterationCount = BodyDesc.solverIterationCount;
	fSleepEnergyThreshold = BodyDesc.sleepEnergyThreshold;
	fSleepDamping		= BodyDesc.sleepDamping;
	fContactReportThreshold = BodyDesc.contactReportThreshold;
}

void mpBodyDesc::GetNx( NxBodyDesc& BodyDesc ) const
{
	ToNxMat34( BodyDesc.massLocalPose, mtxMassLocalTM );

	BodyDesc.massLocalPose.t.x /= g_fUnitScale;
	BodyDesc.massLocalPose.t.y /= g_fUnitScale;
	BodyDesc.massLocalPose.t.z /= g_fUnitScale;

	BodyDesc.massSpaceInertia		= ToNxVector( vecMassSpaceInertia );
	BodyDesc.mass					= fMass;
	BodyDesc.linearVelocity			= ToNxVector( vecLinearVelocity );
	BodyDesc.angularVelocity		= ToNxVector( vecAngularVelocity );
	BodyDesc.wakeUpCounter			= fWakeUpCounter;
	BodyDesc.linearDamping			= fLinearDamping;
	BodyDesc.angularDamping			= fAngularDamping;
	BodyDesc.CCDMotionThreshold		= fCCDMotionThreshold;
	BodyDesc.flags					= uFlags;
	BodyDesc.sleepLinearVelocity	= fSleepLinearVelocity;
	BodyDesc.sleepAngularVelocity	= fSleepAngularVelocity;
	BodyDesc.solverIterationCount	= uSolverIterationCount;
	BodyDesc.sleepEnergyThreshold	= fSleepEnergyThreshold;
	BodyDesc.sleepDamping			= fSleepDamping;
	BodyDesc.contactReportThreshold = fContactReportThreshold;
}

void mpBodyDesc::SetToDefault()
{
	NxBodyDesc BodyDesc;
	BodyDesc.setToDefault();

	SetNx( BodyDesc );
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////// mpActorDesc //////////////////////////////
////////////////////////////////////////////////////////////////////////////
mpActorDesc::mpActorDesc() :
	vtxPos( 0, 0, 0 ),
	qRotate( 0, 0, 0, 1.f )
{
	bBody = false;
}

void mpActorDesc::SetNx( const NxActorDesc& ActorDesc )
{
	ToF3dMatrix( mtxWorld, ActorDesc.globalPose );
	mtxWorld._41 *= g_fUnitScale;
	mtxWorld._42 *= g_fUnitScale;
	mtxWorld._43 *= g_fUnitScale;

	if( ActorDesc.body ) {
		bBody = true;
		Body.SetNx( *ActorDesc.body );
	}
	else {
		bBody = false;
		Body.SetToDefault();
	}
	fDensity = ActorDesc.density;
	uFlags = ActorDesc.flags;
	Group = ActorDesc.group;
	DominanceGroup = ActorDesc.dominanceGroup;
	uContactReportFlags = ActorDesc.contactReportFlags;
	suForceFieldMaterial = ActorDesc.forceFieldMaterial;
}

void mpActorDesc::GetNx( NxActorDesc& ActorDesc ) const
{
	ToNxMat34( ActorDesc.globalPose, mtxWorld );
	ActorDesc.globalPose.t.x /= g_fUnitScale;
	ActorDesc.globalPose.t.y /= g_fUnitScale;
	ActorDesc.globalPose.t.z /= g_fUnitScale;

	ActorDesc.density = fDensity;
	ActorDesc.flags = uFlags;
	ActorDesc.group = Group;
	ActorDesc.dominanceGroup = DominanceGroup;
	ActorDesc.contactReportFlags = uContactReportFlags;
	ActorDesc.forceFieldMaterial = suForceFieldMaterial;
}

void mpActorDesc::SetToDefault()
{
	NxActorDesc ActorDesc;
	ActorDesc.setToDefault();

	ActorDesc.density = 0.1f;
	SetNx( ActorDesc );
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////// mpShapeDesc //////////////////////////////
////////////////////////////////////////////////////////////////////////////
mpShapeDesc::mpShapeDesc() :
	vtxLocalPos( 0, 0, 0 ),
	qLocalRotate( 0, 0, 0, 1.f )
{
}

void mpShapeDesc::SetNx( const NxShapeDesc&ShapeDesc )
{
	ToF3dMatrix( mtxLocalTM, ShapeDesc.localPose );
	mtxLocalTM._41 *= g_fUnitScale;
	mtxLocalTM._42 *= g_fUnitScale;
	mtxLocalTM._43 *= g_fUnitScale;

	uShapeFlags		= ShapeDesc.shapeFlags;
	Group			= ShapeDesc.group;
	MaterialIndex	= ShapeDesc.materialIndex;
	fDensity		= ShapeDesc.density;
	fMass			= ShapeDesc.mass;
	fSkinWidth		= ShapeDesc.skinWidth;
	GroupsMask		= ShapeDesc.groupsMask;
	uNonInteractingCompartmentTypes = ShapeDesc.nonInteractingCompartmentTypes;
}

void mpShapeDesc::GetNx( NxShapeDesc& ShapeDesc, const F3dVector& vtxLocalPos ) const
{
	ToNxMat34( ShapeDesc.localPose, mtxLocalTM );
	ShapeDesc.localPose.t.x = vtxLocalPos.x / g_fUnitScale;
	ShapeDesc.localPose.t.y = vtxLocalPos.y / g_fUnitScale;
	ShapeDesc.localPose.t.z = vtxLocalPos.z / g_fUnitScale;

	ShapeDesc.shapeFlags= uShapeFlags;
	ShapeDesc.group		= Group;
	ShapeDesc.materialIndex = MaterialIndex;
	ShapeDesc.density	= fDensity;
	ShapeDesc.mass		= fMass;
	ShapeDesc.skinWidth	= fSkinWidth;
	ShapeDesc.groupsMask= GroupsMask;
	ShapeDesc.nonInteractingCompartmentTypes = uNonInteractingCompartmentTypes;
}

void mpShapeDesc::SetToDefault( NxShapeDesc& ShapeDesc )
{
	ShapeDesc.setToDefault();
	SetNx( ShapeDesc );
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////// mpSphereShapeDesc //////////////////////////////
////////////////////////////////////////////////////////////////////////////
mpSphereShapeDesc::mpSphereShapeDesc()
{
	SetToDefault();
}

void mpSphereShapeDesc::SetNx( const NxSphereShapeDesc& SphereShapeDesc )
{
	mpShapeDesc::SetNx( SphereShapeDesc );
	fRadius = SphereShapeDesc.radius * g_fUnitScale;
}

void mpSphereShapeDesc::GetNx( NxSphereShapeDesc& SphereShapeDesc, const F3dVector& vtxLocalPos ) const
{
	mpShapeDesc::GetNx( SphereShapeDesc, vtxLocalPos );
	SphereShapeDesc.radius = fRadius / g_fUnitScale;
}

void mpSphereShapeDesc::SetToDefault()
{
	NxSphereShapeDesc SphereShapeDesc;
	mpShapeDesc::SetToDefault( SphereShapeDesc );
	fRadius = 20.f;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////// mpBoxShapeDesc//////////////////////////////////
////////////////////////////////////////////////////////////////////////////
mpBoxShapeDesc::mpBoxShapeDesc()
{
	SetToDefault();
}

void mpBoxShapeDesc::SetNx( const NxBoxShapeDesc& BoxShapeDesc )
{
	mpShapeDesc::SetNx( BoxShapeDesc );
	vecExtent = ToF3dVector( BoxShapeDesc.dimensions );
	vecExtent *= g_fUnitScale;
}

void mpBoxShapeDesc::GetNx( NxBoxShapeDesc& BoxShapeDesc, const F3dVector& vtxLocalPos ) const
{
	mpShapeDesc::GetNx( BoxShapeDesc, vtxLocalPos );
	BoxShapeDesc.dimensions = ToNxVector( vecExtent );
	BoxShapeDesc.dimensions /= g_fUnitScale;
}

void mpBoxShapeDesc::SetToDefault()
{
	NxBoxShapeDesc BoxShapeDesc;
	mpShapeDesc::SetToDefault( BoxShapeDesc );
	vecExtent = F3dVector( 10.f, 10.f, 10.f );
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////// mpCapsuleShapeDesc /////////////////////////////
////////////////////////////////////////////////////////////////////////////
mpCapsuleShapeDesc::mpCapsuleShapeDesc()
{
	SetToDefault();
}

void mpCapsuleShapeDesc::SetNx( const NxCapsuleShapeDesc& CapsuleShapeDesc )
{
	mpShapeDesc::SetNx( CapsuleShapeDesc );
	fHeight = CapsuleShapeDesc.height * g_fUnitScale;
	fRadius = CapsuleShapeDesc.radius * g_fUnitScale;
}

void mpCapsuleShapeDesc::GetNx( NxCapsuleShapeDesc& CapsuleShapeDesc, const F3dVector& vtxLocalPos ) const
{
	mpShapeDesc::GetNx( CapsuleShapeDesc, vtxLocalPos );
	CapsuleShapeDesc.height = fHeight / g_fUnitScale;
	CapsuleShapeDesc.radius = fRadius / g_fUnitScale;
}

void mpCapsuleShapeDesc::SetToDefault()
{
	NxCapsuleShapeDesc CapsuleShapeDesc;
	mpShapeDesc::SetToDefault( CapsuleShapeDesc );
	fHeight = 10.f;
	fRadius = 10.f;
}

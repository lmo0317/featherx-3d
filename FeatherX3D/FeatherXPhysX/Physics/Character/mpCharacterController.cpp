#include "MagiPhysXDef.h"

mpCharacterController::mpCharacterController( const F3dVector& vtxCenter, float radius, float height, float slopeLimitDegree, float skinWidth, float stepOffset )
{
	float fUnitScale = mpManager::GetSingleton()->GetUnitScale();

	NxCapsuleControllerDesc desc;
	desc.position.x = vtxCenter.x / fUnitScale;
	desc.position.y = vtxCenter.y / fUnitScale;
	desc.position.z = vtxCenter.z / fUnitScale;
	desc.height = height / fUnitScale;
	desc.radius = radius / fUnitScale;
	desc.skinWidth = skinWidth / fUnitScale;
	desc.slopeLimit = cosf(slopeLimitDegree / fUnitScale);
	desc.stepOffset = stepOffset / fUnitScale;
	desc.upDirection = NX_Y;
	desc.climbingMode = CLIMB_EASY;
//	desc.callback = &mReport;

	NxScene* pScene = mpManager::GetSingleton()->GetScene();
	m_pNxController = mpManager::GetSingleton()->GetControllerManager()->createController( pScene, desc );
}

mpCharacterController::~mpCharacterController()
{
	if(m_pNxController) {
		mpManager::GetSingleton()->GetControllerManager()->releaseController( *m_pNxController );
	}
}

mpCharacterController::CollisionFlags mpCharacterController::Move( const F3dVector& motion )
{
	if( m_pNxController == NULL )
		return None;

	float fUnitScale = mpManager::GetSingleton()->GetUnitScale();
	NxVec3 m = ToNxVector( motion );
	m *= fUnitScale;

	UINT cf = 0;
	m_pNxController->move( m, 31, 0.000001f, cf );

	if( cf & NXCC_COLLISION_SIDES )
		return CollidedSides;

	if( cf & NXCC_COLLISION_UP )
		return CollidedAbove;

	if( cf & NXCC_COLLISION_DOWN )
		return CollidedBelow;

	return None;
}

const NxActor* mpCharacterController::GetNxActor()
{
	return m_pNxController->getActor();
}

void mpCharacterController::SetWorldTransform( const F3dMatrix& mtxTrans )
{
	if( m_pNxController->getActor() != NULL ) {
		float fUnitScale = mpManager::GetSingleton()->GetUnitScale();
		F3dMatrix mtxWorld = mtxTrans;
		mtxWorld._41 /= fUnitScale;
		mtxWorld._42 /= fUnitScale;
		mtxWorld._43 /= fUnitScale;
		SetGlobalTM( m_pNxController->getActor(), mtxWorld );
	}
}

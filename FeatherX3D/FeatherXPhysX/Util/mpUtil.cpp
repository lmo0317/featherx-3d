#include "MagiPhysXDef.h"

void MoveGlobalTM( NxActor* pActor, const F3dMatrix& mtxM )
{
	NxMat33 tm;
	tm.setColumnMajorStride4( &mtxM._11 );

	pActor->moveGlobalOrientation( tm );
	pActor->moveGlobalPosition( NxVec3( mtxM._41, mtxM._42, mtxM._43 ) );
}

void SetGlobalTM( NxActor* pActor, const F3dMatrix& mtxM )
{
	NxMat33 tm;
	tm.setColumnMajorStride4( &mtxM._11 );

	pActor->setGlobalOrientation( tm );
	pActor->setGlobalPosition( NxVec3( mtxM._41, mtxM._42, mtxM._43 ) );
}

void GetGlobalTM( F3dMatrix& mtxResult, const NxShape* pShape )
{
	if(pShape == NULL)
		return;

	NxMat34 pose = pShape->getGlobalPose();
	pose.M.getColumnMajorStride4( &mtxResult._11 );
	mtxResult._14 = mtxResult._24 = mtxResult._34 = 0;

	mtxResult._41 = pose.t.x;
	mtxResult._42 = pose.t.y;
	mtxResult._43 = pose.t.z;
	mtxResult._44 = 1.f;
}

void GetGlobalTM( F3dMatrix& mtxResult, const NxActor* pActor )
{
	if(pActor == NULL)
		return;

	NxMat34 pose = pActor->getGlobalPose();
	pose.M.getColumnMajorStride4( &mtxResult._11 );
	mtxResult._14 = mtxResult._24 = mtxResult._34 = 0;

	mtxResult._41 = pose.t.x;
	mtxResult._42 = pose.t.y;
	mtxResult._43 = pose.t.z;
	mtxResult._44 = 1.f;
}

void ToNxMat34( NxMat34& Result, const F3dMatrix& mtxM )
{
	Result.M.setColumnMajorStride4( &mtxM._11 );
	Result.t = NxVec3( mtxM._41, mtxM._42, mtxM._43 );
}

void ToF3dMatrix( F3dMatrix& Result, const NxMat34& mtxM )
{
	mtxM.M.getColumnMajorStride4( &Result._11 );
	Result._14 = Result._24 = Result._34 = 0;

	Result._41 = mtxM.t.x;
	Result._42 = mtxM.t.y;
	Result._43 = mtxM.t.z;
	Result._44 = 1.f;
}
#pragma once

// Kinematic Actors
void MoveGlobalTM( NxActor* pActor, const F3dMatrix& mtxM );

void SetGlobalTM( NxActor* pActor, const F3dMatrix& mtxM );
void GetGlobalTM( F3dMatrix& mtxResult, const NxActor* pActor );
void GetGlobalTM( F3dMatrix& mtxResult, const NxShape* pShape );

void ToNxMat34( NxMat34& Result, const F3dMatrix& mtxM );
void ToF3dMatrix( F3dMatrix& Result, const NxMat34& mtxM );
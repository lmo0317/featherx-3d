// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#pragma comment( lib, "NxCharacter.lib" )
#pragma comment( lib, "NxCooking.lib" )
#pragma comment( lib, "NxExtensions.lib" )
#pragma comment( lib, "PhysXLoader.lib" )

#include <map>
#include "NxCooking.h"
#include "NxPhysics.h"
#include "NxCharacter.h"
#include "NxPhysicsSDK.h"
#include "NxPMap.h"
#include "PhysXLoader.h"
#include "ControllerManager.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"

#include <boost/unordered/unordered_map.hpp>
#include <wtypes.h>

#include "FeatherXEngineCommonPrecompile.h"

#include "UserAllocator.h"
#include "ErrorStream.h"

#include "Util/mpUtil.h"
#include "Stream/Stream.h"

#include "mpDef.h"
#include "Physics/mpActorDesc.h"
#include "Physics/Joint/mpJointDesc.h"
#include "Physics/Joint/mpFixedJointDesc.h"
#include "Physics/Joint/mpD6JointDesc.h"
#include "Physics/Joint/mpRevoluteJointDesc.h"
#include "Physics/Joint/mpSphericalJointDesc.h"
#include "Physics/mpManager.h"
#include "Physics/Character/mpCharacterController.h"

#include "Physics/Vehicle/NxLinearInterpolationValues.h"
#include "Physics/Vehicle/NxWheelDesc.h"
#include "Physics/Vehicle/NxWheel.h"
#include "Physics/Vehicle/NxVehicleGearDesc.h"
#include "Physics/Vehicle/NxVehicleGears.h"
#include "Physics/Vehicle/NxVehicleMotorDesc.h"
#include "Physics/Vehicle/NxVehicleMotor.h"
#include "Physics/Vehicle/NxVehicleDesc.h"
#include "Physics/Vehicle/NxVehicle.h"


inline F3dVector ToF3dVector( const NxVec3& V )
{
	return F3dVector( V.x, V.y, V.z );
}

inline NxVec3 ToNxVector( const F3dVector& V )
{
	return NxVec3( V.x, V.y, V.z );
}

inline NxQuat ToNxQuat( const F3dQuat& Q )
{
	NxQuat Nq;
	Nq.setXYZW( Q.x, Q.y, Q.z, Q.w );
	return Nq;
}

#include "NxRemoteDebugger.h"

//Set this to 0 to stop the samples from trying to connect to the VRD
#define SAMPLES_USE_VRD 1

//Change this setting to the IP number or DNS name of the computer that is running the VRD
#define SAMPLES_VRD_HOST "localhost"
//Change this setting to the port on which the VRD is listening, or keep the default: NX_DBG_DEFAULT_PORT
#define SAMPLES_VRD_PORT NX_DBG_DEFAULT_PORT
//Change this setting to decide what type of information is sent to the VRD. Default: NX_DBG_EVENTMASK_EVERYTHING
#define SAMPLES_VRD_EVENTMASK NX_DBG_EVENTMASK_EVERYTHING

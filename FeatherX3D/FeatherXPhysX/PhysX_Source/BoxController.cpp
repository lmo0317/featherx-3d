/*----------------------------------------------------------------------------*\
|
|							NVIDIA PhysX Technology
|
|							     www.nvidia.com
|
\*----------------------------------------------------------------------------*/

#include "NxController.h"
#include "Controller.h"
#include "BoxController.h"
#include "NxPhysics.h"
#include "NxBoxShapeDesc.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BoxController::BoxController(const NxControllerDesc& desc, NxScene* s) : Controller(desc, s)
	{
	type = NX_CONTROLLER_BOX;

	const NxBoxControllerDesc& bc = (const NxBoxControllerDesc&)desc;

	extents	= bc.extents;

	// Create kinematic actor under the hood
	NxBodyDesc bodyDesc;
	bodyDesc.flags			|= NX_BF_KINEMATIC;

	NxBoxShapeDesc boxDesc;
	boxDesc.userData		= (void*)'CCTS';
	boxDesc.dimensions		= extents*0.8f;

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10000.0f;	// ### expose this ?
	actorDesc.density		= 10.0f;	// ### expose this ?

	// LOSS OF ACCURACY
	actorDesc.globalPose.t.x  = (float)position.x;
	actorDesc.globalPose.t.y  = (float)position.y;
	actorDesc.globalPose.t.z  = (float)position.z;
	kineActor = scene->createActor(actorDesc);
}

BoxController::~BoxController()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BoxController::reportSceneChanged()
{
	cctModule.VoidTestCache();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BoxController::getWorldBox(NxExtendedBounds3& box) const
	{
	box.setCenterExtents(position, extents);
	return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const NxVec3& BoxController::getExtents() const
	{
	return extents;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BoxController::setExtents(const NxVec3& e)
	{
	extents	= e;
	if(kineActor)
		{
		NxShape* S = *kineActor->getShapes();
		NX_ASSERT(S && static_cast<NxBoxShape*>(S));
		NxBoxShape* BS = static_cast<NxBoxShape*>(S);
		BS->setDimensions(e * 0.8f);
		}
	return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BoxController::setStepOffset(const float offset)
	{
    stepOffset = offset;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


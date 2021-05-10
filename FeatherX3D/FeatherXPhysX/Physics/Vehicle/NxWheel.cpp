#include "MagiPhysXDef.h"

NxWheel* NxWheel::createWheel(NxActor* actor, NxWheelDesc* wheelDesc) 
{
	if(wheelDesc->wheelFlags & NX_WF_USE_WHEELSHAPE)
	{
		return new NxWheel(actor, wheelDesc);
	}
}

NxWheel::NxWheel(NxActor* a, NxWheelDesc* wheelDesc) : actor(a)
{
	NxScene * scene = &actor->getScene();
	//create a shared car wheel material to be used by all wheels
	static NxMaterial * wsm = 0;
	if(!wsm)
	{
		NxMaterialDesc m;
		m.flags |= NX_MF_DISABLE_FRICTION;
		wsm = scene->createMaterial(m);
	}

	NxWheelShapeDesc wheelShapeDesc;

	wheelShapeDesc.localPose.t = wheelDesc->position;
	NxQuat q;
	q.fromAngleAxis(90.0f, NxVec3(0,1,0));
	wheelShapeDesc.localPose.M.fromQuat(q);
	wheelShapeDesc.materialIndex = wsm->getMaterialIndex();
	wheelFlags = wheelDesc->wheelFlags;
	
	NxReal heightModifier = (wheelDesc->wheelSuspension + wheelDesc->wheelRadius) / wheelDesc->wheelSuspension;

	wheelShapeDesc.suspension.spring = wheelDesc->springRestitution*heightModifier;
	wheelShapeDesc.suspension.damper = 0;//wheelDesc->springDamping*heightModifier;
	wheelShapeDesc.suspension.targetValue = wheelDesc->springBias*heightModifier;

	wheelShapeDesc.radius = wheelDesc->wheelRadius;
	wheelShapeDesc.suspensionTravel = wheelDesc->wheelSuspension; 
	wheelShapeDesc.inverseWheelMass = 0.1f;	//not given!? TODO

	wheelShapeDesc.lateralTireForceFunction.stiffnessFactor *= wheelDesc->frictionToSide;	
	wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor *= wheelDesc->frictionToFront;	

	wheelShape = static_cast<NxWheelShape *>(actor->createShape(wheelShapeDesc));
}

NxWheel::~NxWheel()
{
}

void NxWheel::tick(bool handBrake, NxReal motorTorque, NxReal brakeTorque, NxReal dt)
{
	//motorTorque *= 0.1f;
	brakeTorque *= 500.0f;
	if(handBrake && getWheelFlag(NX_WF_AFFECTED_BY_HANDBRAKE))
		brakeTorque = 3000.0f;

	if(getWheelFlag(NX_WF_ACCELERATED)) 
		wheelShape->setMotorTorque(motorTorque);

	wheelShape->setBrakeTorque(brakeTorque);
}

NxActor* NxWheel::getTouchedActor() const
{
	NxWheelContactData wcd;
	NxShape * s = wheelShape->getContact(wcd);	
	return s ? &s->getActor() : 0;
}

NxVec3 NxWheel::getWheelPos() const
{
	return wheelShape->getLocalPosition();
}

void NxWheel::setAngle(NxReal angle)
{
	wheelShape->setSteerAngle(-angle);
}


void NxWheel::drawWheel(NxReal approx) const
{/*
	//nothing, taken care of by built in visualization.
	NxWheelContactData wcd;
	NxShape* s = wheelShape->getContact(wcd);	
	if(!s) return;

	//printf(" f = %f %f %f\n",wcd.contactForce, wcd.longitudalSlip, wcd.longitudalImpulse);
	*/
/*
	glPushMatrix();
	glLoadIdentity();

	NxVec3 pVecBuffer[4];
	pVecBuffer[0] = wcd.contactPoint;
	pVecBuffer[1] = wcd.contactPoint + wcd.longitudalDirection;
	pVecBuffer[2] = wcd.contactPoint;
	pVecBuffer[3] = wcd.contactPoint + wcd.lateralDirection;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pVecBuffer);
	glDrawArrays(GL_LINES, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
*/
}

NxReal NxWheel::getRpm() const
{
	return NxMath::abs(wheelShape->getAxleSpeed())/NxTwoPi * 60.0f;
}

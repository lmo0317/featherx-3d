#pragma once

class NxVehicle;

struct ContactInfo 
	{
	ContactInfo() { reset(); }
	void reset() { otherActor = NULL; relativeVelocity = 0; }
	bool isTouching() const { return otherActor != NULL; }
	NxActor*				otherActor;
	NxVec3					contactPosition;
	NxVec3					contactPositionLocal;
	NxVec3					contactNormal;
	NxReal					relativeVelocity;
	NxReal					relativeVelocitySide;
};


class NxWheel 
{
public:
	NxWheel(NxActor* actor, NxWheelDesc* wheelDesc);
	virtual					~NxWheel();
	static NxWheel* createWheel(NxActor* actor, NxWheelDesc* wheelDesc);

	virtual void			tick(bool handbrake, NxReal motorTorque, NxReal brakeTorque, NxReal dt);
	virtual NxActor *		getTouchedActor() const;
	virtual NxVec3			getWheelPos() const;
	virtual void			setAngle(NxReal angle);
	virtual void			drawWheel(NxReal approx) const;
	virtual NxReal			getRpm() const;
	virtual NxVec3			getGroundContactPos() const { return getWheelPos()+NxVec3(0, -wheelShape->getRadius(), 0); }
	virtual float			getRadius() const { return wheelShape->getRadius(); }
	NxWheelShape *          getWheelShape() {return wheelShape;}    


	NxWheelShape*			getWheelShape() const {return wheelShape;}



	NX_INLINE bool			hasGroundContact() const { return getTouchedActor() != NULL; }
	NX_INLINE bool			getWheelFlag(NxWheelFlags flag) const { return (wheelFlags & flag) != 0; }

	void*					userData;
	protected:
	NxU32					wheelFlags;

private:
	NxActor* actor;
	NxWheelShape * wheelShape;

};
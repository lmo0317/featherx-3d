#pragma once

class mpJointDesc
{
public:
	std::wstring strActor1, strActor2;
	F3dVector vtxGlobalAnchor;
	F3dVector vecGlobalAxis;

	UINT64 ActorID[2];
	float fMaxForce;
	float fMaxTorque;
	float fSolverExtrapolationFactor;
	UINT uUseAccelerationSpring;
	UINT uJointFlags;

	virtual	~mpJointDesc();

	virtual void SetToDefault();
	virtual bool IsValid() const;
	NxJointType	GetType()	const	{ return Type; }

	virtual mpJointDesc* Copy() const = 0;
	virtual void Copy( const mpJointDesc* pJointDesc );

	void GetNxJointDesc( NxJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const;

	virtual void GetNxRevoluteJointDesc( NxRevoluteJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const {}
	virtual void GetNxSphericalJointDesc( NxSphericalJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const {}
	virtual void GetNxD6JointDesc( NxD6JointDesc& JointDesc, const F3dMatrix& mtxWorld ) const {}
	virtual void GetNxFixedJointDesc( NxFixedJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const {}

protected:
	mpJointDesc( NxJointType t );
	NxJointType Type;
};

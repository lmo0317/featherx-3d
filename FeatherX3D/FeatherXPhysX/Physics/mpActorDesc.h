#pragma once

struct mpBodyDesc
{
	mpBodyDesc();

	void		SetNx( const NxBodyDesc& BodyDesc );
	void		GetNx( NxBodyDesc& BodyDesc ) const;
	void		SetToDefault();

	F3dMatrix	mtxMassLocalTM;		//무게중심
	F3dVector	vecMassSpaceInertia;
	float		fMass;
	F3dVector	vecLinearVelocity;
	F3dVector	vecAngularVelocity;
	float		fWakeUpCounter;
	float		fLinearDamping;
	float		fAngularDamping;
	float		fCCDMotionThreshold;
	UINT		uFlags;
	float		fSleepLinearVelocity;
	float		fSleepAngularVelocity;
	UINT		uSolverIterationCount;
	float		fSleepEnergyThreshold;
	float		fSleepDamping;
	float		fContactReportThreshold;
};

struct mpActorDesc
{
	mpActorDesc();
	void		SetNx( const NxActorDesc& ActorDesc );
	void		GetNx( NxActorDesc& ActorDesc ) const;
	void		SetToDefault();

	std::wstring strName;
	F3dVector	vtxPos;
	F3dQuat		qRotate;
	F3dMatrix	mtxWorld;
	bool		bBody;
	mpBodyDesc	Body;
	float		fDensity;
	UINT		uFlags;
	NxActorGroup  Group;
	NxDominanceGroup  DominanceGroup;
	UINT	uContactReportFlags;
	UINT16  suForceFieldMaterial;
	CONTROL_TYPE cType;
};

struct mpShapeDesc
{
	mpShapeDesc();
	virtual SHAPE_TYPE	GetType() const = 0;
	virtual void	SetNx( const NxShapeDesc& ShapeDesc );
	void	GetNx( NxShapeDesc& pShapeDesc, const F3dVector& vtxLocalPos ) const;
	virtual void	SetToDefault( NxShapeDesc& ShapeDesc );

	F3dVector			vtxLocalPos;
	F3dQuat				qLocalRotate;
	F3dMatrix			mtxLocalTM;
	UINT				uShapeFlags;
	NxCollisionGroup	Group;
	NxMaterialIndex		MaterialIndex;
	float				fDensity;
	float				fMass;
	float				fSkinWidth;
	NxGroupsMask		GroupsMask;
	UINT				uNonInteractingCompartmentTypes;
};

struct mpSphereShapeDesc : public mpShapeDesc
{
	mpSphereShapeDesc();
	SHAPE_TYPE	GetType() const { return SHAPE_SPHERE; }
	void	SetNx( const NxSphereShapeDesc& SphereShapeDesc );
	void	GetNx( NxSphereShapeDesc& SphereShapeDesc, const F3dVector& vtxLocalPos ) const;
	void	SetToDefault();

	float fRadius;
};

struct mpBoxShapeDesc : public mpShapeDesc
{
	mpBoxShapeDesc();
	SHAPE_TYPE	GetType() const { return SHAPE_BOX; }
	void	SetNx( const NxBoxShapeDesc& BoxShapeDesc );
	void	GetNx( NxBoxShapeDesc& BoxShapeDesc, const F3dVector& vtxLocalPos ) const;
	void	SetToDefault();

	F3dVector vecExtent;
};

struct mpCapsuleShapeDesc : public mpShapeDesc
{
	mpCapsuleShapeDesc();
	SHAPE_TYPE	GetType() const { return SHAPE_CAPSULE; }
	void	SetNx( const NxCapsuleShapeDesc& CapsuleShapeDesc );
	void	GetNx( NxCapsuleShapeDesc& CapsuleShapeDesc, const F3dVector& vtxLocalPos ) const;
	void	SetToDefault();

	float fHeight;
	float fRadius;
};

#pragma once

struct F3dActor
{
	F3dActor() :  cCurType( CONTROL_DYNAMIC ), nLinkedNode(-1), ActorID(0), nLevel(0), bRoot(false), bNoRagdoll(false) {}
	std::wstring strName;
	std::wstring strLinkedNode;
	int			 nLinkedNode;
	CONTROL_TYPE cCurType;
	UINT64		 ActorID;
	int			 nLevel;
	bool		 bRoot;
	bool		 bNoRagdoll;
};
typedef boost::shared_ptr< F3dActor > F3dActorPtr;

struct F3dResultAni
{
	std::wstring strLinkedNode;
	F3dVector vecOffset;
};

class F3dCPhysics
{
public:
	enum
	{
		RAGDOLL_OFF,
		RAGDOLL_ON,
		RAGDOLL_RETURN,
	};

	F3dCPhysics();
	~F3dCPhysics();

	void Process();
	F3dActor* CreatePhysicsActor();
	virtual void SetWorldTransform( const F3dMatrix& mtxTrans );
	F3dMatrix& GetWorldTransform() {return m_mtxWorldTrans;}
	void addForce(F3dVector vecForce);
	void setLinearVelocity(F3dVector vecVelocity);
	F3dVector getLinearVelocity();
	void setAngularVelocity(F3dVector vecForce);
	void PhysicsContact(NxActor **actors);
	void SetCB_Contact(ContactCB_ptr pCB_Contact) {m_pCB_Contact = pCB_Contact;}
	bool IsMyActor(int nID);
	void SetKinematic( bool bEnabe );

protected:
	std::vector< F3dActor* > m_vPhysicsActor;
	F3dActor* m_pPhysicsActor;

	F3dMatrix m_mtxWorldTrans;
	float m_fScale;
	F3dResultAni m_physicResultAni;
	ContactCB_ptr m_pCB_Contact;

	std::vector< mpShapeDesc* > m_vShapeDesc;
	mpActorDesc m_ActorDesc;
};
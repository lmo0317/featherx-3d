#pragma once

#include "Character/mpCharacterController.h"
typedef boost::function< void (UINT64 *arr) >	SleepCB_ptr;
typedef boost::function< void (NxActor **actors) >	ContactCB_ptr;

class mpCallback;
class mpContact;
class mpManager : public FBSingleton< mpManager >
{
public:
	enum {
		PLAY_STOP,
		PLAY_PLAY,
	};

	struct mpCharacterControllerData
	{
		F3dVector vtxCenter;
		bool  bEnable;
		float fRadius;
		float fHeight;
		float fSlopeLimitDegree;
		float fSkinWidth;
		float fStepOffset;
		mpCharacterControllerData() : vtxCenter(0,0,0), bEnable( false ), fRadius(5.f), fHeight(10.f), fSlopeLimitDegree(45.f), fSkinWidth(0.01f), fStepOffset(0) {}
	};

	mpManager();
	~mpManager();

	bool		Init( std::wstring& strError );
	void		Finish();
	NxScene*	GetScene() { return m_pScene; }

	void		ReleaseAll();
	void		ReleaseTerrain();
	void		ReleaseActor( UINT64 uActorID );
	void		ReleaseJoint( UINT64 uJointID );
	void		ReleaseCharacterController( UINT64 uCharacterControllerID );

	void		CreateTerrain( fcoWorld* pWorld );
	UINT64		CreateActor( const mpActorDesc& ActorDesc, const std::vector< mpShapeDesc* >& vShapeDesc, const F3dMatrix& mtxWorld_NoScale, const float fWorldScale );
	UINT64		CreateActor(NxActor* pNewActor,std::wstring strName);
	
	//bool		AddCube( UINT64 uActorID, const mcoCOBB& OBB, const F3dMatrix& mtxLocal );
	bool		AddSphere( UINT64 uActorID, float fRadius, const F3dMatrix& mtxLocal );
	bool		AddCapsule( UINT64 uActorID, float fHeight, float fRadius, const F3dMatrix& mtxLocal );
	NxActor*	FindActor( UINT64 uActorID );
	const WCHAR* FindName( UINT64 uActorID );

	UINT64		CreateJoint( const mpJointDesc* pJointDesc, const F3dMatrix& mtxWorld );
	NxJoint*	FindJoint( UINT64 uJointID );

	ControllerManager* GetControllerManager();
	UINT64		CreateCharacterController( const mpCharacterControllerData& ccd );
	mpCharacterControllerPtr FindCharacterController( UINT64 uCharacterControllerID );

	void		SetGravity( const F3dVector& vecGravity );
	UINT64		GetLastID() const { return m_LastID; }
	void		Process( double dTime, double dt );

	void		Play() { m_nPlayState = PLAY_PLAY; }
	void		Stop() { m_nPlayState = PLAY_STOP; }

	float		GetUnitScale() const;

	void		CreateGroundPlane();
	void		ReleaseGroundPlane();

	//call back 함수
	void		SetCB_Sleep(UINT64 uActorID,SleepCB_ptr CB_Sleep);
	void		SetCB_Contact(UINT64 uActorID,ContactCB_ptr CB_Contact);
	void		SetDebugRender( NxParameter Visualize, bool bDebug );
	bool		IsDebugRender( NxParameter Visualize ) const;

	void		onSleep( NxActor **actors, NxU32 count );
	void		onContact(NxActor **actors,bool *bDeleted);
	void		Disable();

private:
	bool InitNx( std::wstring& strError );
	bool hasCookingLibrary();
	bool InitCooking(UserAllocator* allocator = NULL, ErrorStream* outputStream = NULL);
	void CloseCooking();
	bool CookTriangleMesh(const NxTriangleMeshDesc& desc, NxStream& stream);
	UINT64 GenerateActorID();

	// Physics SDK globals
	NxCookingInterface* m_pCooking;
	NxPhysicsSDK* m_pPhysicsSDK;
	NxScene* m_pScene;
	NxVec3 m_vecDefaultGravity;
	NxTriangleMesh* m_pTerrainMesh;
	NxActor*	m_pTerrain;
	ControllerManager* m_pControllerManager;
	MBHashMap< UINT64, NxActor* > m_hashActor;
	MBHashMap< UINT64, std::wstring > m_hashName;
	MBHashMap< UINT64, NxJoint* > m_hashJoint;
	MBHashMap< UINT64, mpCharacterControllerPtr > m_hashCharacterController;
	MBHashMap< UINT64,SleepCB_ptr> m_hashActorSleepCallback;
	MBHashMap< UINT64,ContactCB_ptr>m_hashActorContactCallback;
	UINT64 m_LastID;
	UINT64 m_LastCharacterControllerID;
	int m_nPlayState;
	mpCallback* m_pSleepCallback;
	mpContact* m_pContactCallback;
	bool m_bDebugRender;
	NxActor*	m_pGroundPlane;
	bool m_bDisable;
	ErrorStream	m_pMyErrorStream;
	UserAllocator*	m_pMyAllocator;
};

class mpCallback : public NxUserNotify
{
	virtual bool onJointBreak(NxReal breakingImpulse, NxJoint & brokenJoint){return true;}
	virtual void onWake(NxActor **actors, NxU32 count){}
	virtual void onSleep(NxActor **actors, NxU32 count)
	{
		// thread safe처리..
		mpManager::GetSingleton()->onSleep( actors, count );
	}
};

class mpContact : public NxUserContactReport
{
public:
	virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
	{
		static int nCount = 0;
		printf("CONTACT = [%d] \n",nCount++);
		mpManager::GetSingleton()->onContact(pair.actors,pair.isDeletedActor);
	}
};
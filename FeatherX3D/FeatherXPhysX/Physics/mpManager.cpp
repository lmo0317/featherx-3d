#include "MagiPhysXDef.h"

#define _PHYSICS_DEBUG
#if NX_SDK_VERSION_NUMBER >= 250
#define COOKING_INTERFACE 1
#endif


extern float g_fUnitScale;

mpManager::mpManager() :
	m_pCooking( NULL ),
	m_pPhysicsSDK( NULL ),
	m_pScene( NULL ),
	m_vecDefaultGravity(0,-9.8f,0),
	m_pTerrainMesh( NULL ),
	m_pTerrain( NULL ),
	m_LastID( 0 ),
	m_LastCharacterControllerID( 0 ),
	m_nPlayState( PLAY_STOP ),
	m_bDebugRender(false),
	m_pGroundPlane( NULL ),
	m_bDisable( false ),
	m_pSleepCallback(NULL),
	m_pContactCallback(NULL),
	m_pControllerManager(NULL)
{
}

mpManager::~mpManager()
{
}

bool mpManager::Init( std::wstring& strError )
{
	if( m_bDisable )
		return true;

	m_pSleepCallback = new mpCallback;
	m_pContactCallback = new mpContact;
	return InitNx( strError );
}

void mpManager::Finish()
{
	m_hashCharacterController.clear();
	if( m_pControllerManager ) {
		m_pControllerManager->purgeControllers();
	}
	FBDEL(m_pControllerManager);

	m_hashActor.clear();
	m_hashName.clear();
	m_hashJoint.clear();
	m_hashActorSleepCallback.clear();
	m_hashActorContactCallback.clear();
	ReleaseGroundPlane();
	if( m_pScene ) {
		m_pPhysicsSDK->releaseScene( *m_pScene );
		m_pScene = NULL;
	}
	if( m_pPhysicsSDK ) {
		//m_pPhysicsSDK->release();
		NxReleasePhysicsSDK( m_pPhysicsSDK );
	}
	FBDEL(m_pSleepCallback);
	FBDEL(m_pContactCallback);
}

void mpManager::ReleaseAll()
{
	if( m_pScene == NULL )
		return;

	ReleaseTerrain();

	MBHashMap< UINT64, NxActor* >::iterator it = m_hashActor.begin();
	for(; it != m_hashActor.end(); ++it )
	{
		m_pScene->releaseActor( *(it->second) );
	}

	MBHashMap< UINT64, NxJoint* >::iterator jit = m_hashJoint.begin();
	for(; jit != m_hashJoint.end(); ++jit )
	{
		m_pScene->releaseJoint( *jit->second );
	}

	m_hashActor.clear();
	m_hashName.clear();
	m_hashJoint.clear();
	m_hashCharacterController.clear();
	m_hashActorSleepCallback.clear();
	m_hashActorContactCallback.clear();
	m_LastID=0;
}

void mpManager::ReleaseTerrain()
{

	if( m_pScene == NULL )
		return;

	if( m_pTerrain == NULL )
		return;
	m_pScene->releaseActor( *m_pTerrain );
}

void mpManager::ReleaseActor( UINT64 ActorID )
{

	if( m_pScene == NULL )
		return;

	MBHashMap< UINT64, NxActor* >::iterator it = m_hashActor.find( ActorID );
	if( it == m_hashActor.end() )
		return;

	m_pScene->releaseActor( *it->second );
	m_hashActor.erase( ActorID );
	m_hashName.erase( ActorID );
	m_hashActorSleepCallback.erase( ActorID );
	m_hashActorContactCallback.erase( ActorID );
}

void mpManager::ReleaseJoint( UINT64 uJointID )
{

	if( m_pScene == NULL )
		return;

	MBHashMap< UINT64, NxJoint* >::iterator it = m_hashJoint.find( uJointID );
	if( it == m_hashJoint.end() )
		return;

	m_pScene->releaseJoint( *it->second );
	m_hashJoint.erase( uJointID );
	m_hashName.erase( uJointID );
}

void mpManager::ReleaseCharacterController( UINT64 uCharacterControllerID )
{
	if( m_pScene == NULL )
		return;

	auto it = m_hashCharacterController.find( uCharacterControllerID );
	if( it == m_hashCharacterController.end() )
		return;

	m_hashCharacterController.erase( uCharacterControllerID );
}

void mpManager::SetGravity( const F3dVector& vecGravity )
{
	NxVec3 vV = (NxVec3)vecGravity;
	m_pScene->setGravity( vV );
}

bool mpManager::InitNx( std::wstring& strError )
{
	if( m_bDisable )
		return false;

	m_pMyAllocator = new UserAllocator;
	
	const NxPhysicsSDKDesc& desc = NxPhysicsSDKDesc();
	NxSDKCreateError errorCode;

	m_pPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, m_pMyAllocator, &m_pMyErrorStream, desc, &errorCode );
	if( m_pPhysicsSDK == NULL ) 
	{
		switch( errorCode ) 
		{
		case NXCE_NO_ERROR           : strError = L"brief No errors occurred when creating the Physics SDK."; break;
		case NXCE_PHYSX_NOT_FOUND    : strError = L"brief Unable to find the PhysX libraries. The PhysX drivers are not installed correctly."; break;
		case NXCE_WRONG_VERSION      : strError = L"brief The application supplied a version number that does not match with the libraries."; break;
		case NXCE_DESCRIPTOR_INVALID : strError = L"brief The supplied SDK descriptor is invalid."; break;
		case NXCE_CONNECTION_ERROR   : strError = L"brief A PhysX card was found, but there are problems when communicating with the card."; break;
		case NXCE_RESET_ERROR        : strError = L"brief A PhysX card was found, but it did not reset (or initialize) properly."; break;
		case NXCE_IN_USE_ERROR       : strError = L"brief A PhysX card was found, but it is already in use by another application."; break;
		case NXCE_BUNDLE_ERROR       : strError = L"brief A PhysX card was found, but there are issues with loading the firmware."; break;
		default:
			strError = L"NxCreatePhysicsSDK fail.";
			break;
		}
		
		return false;
	}

#ifdef _PHYSICS_DEBUG
	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
#endif

	// Set the physics parameters
	m_pPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 0.2f );
	m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

	// Set the debug visualization parameters
#ifdef _DEBUG
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1);
#endif

	// scene를 생성한다.
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = m_vecDefaultGravity;
	sceneDesc.userNotify= m_pSleepCallback;
	sceneDesc.userContactReport = m_pContactCallback;
	m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
	if( m_pScene == NULL ) {
		sceneDesc.simType = NX_SIMULATION_SW;
		m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
		if( m_pScene == NULL ) {
			strError = L"m_pPhysicsSDK->createScene fail.";
			return false;
		}
	}

	// Create the default material
	NxMaterial* pDefaultMaterial = m_pScene->getMaterialFromIndex(0);
	pDefaultMaterial->setRestitution(0.5);		// 복원력
	pDefaultMaterial->setStaticFriction(100.0);	// 정지 마찰력
	pDefaultMaterial->setDynamicFriction(1.0);	// 운동 마찰력

	m_pControllerManager = new ControllerManager();
	Play();
	return true;
}

bool mpManager::hasCookingLibrary() // check to see if the cooking library is available or not!
{
	bool ret = true;

#ifdef COOKING_INTERFACE
	if ( m_pCooking == 0 )
	{
		m_pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
		if ( m_pCooking == 0 ) ret = false;
	}
#endif

	return ret;
}

bool mpManager::InitCooking(UserAllocator* allocator, ErrorStream* outputStream)
{
	if( m_bDisable )
		return false;

#ifdef COOKING_INTERFACE
	hasCookingLibrary();
	if ( !m_pCooking ) return false;
	return m_pCooking->NxInitCooking(allocator, outputStream);
#else
	return NxInitCooking(allocator, outputStream);
#endif
}

void mpManager::CloseCooking()
{ 
#ifdef COOKING_INTERFACE
	if ( !m_pCooking ) return;
	m_pCooking->NxCloseCooking();
#else
	return NxCloseCooking();
#endif
}

bool mpManager::CookTriangleMesh(const NxTriangleMeshDesc& desc, NxStream& stream)
{ 
#ifdef COOKING_INTERFACE
	hasCookingLibrary();
	if ( !m_pCooking ) return false;
	return m_pCooking->NxCookTriangleMesh(desc,stream);
#else
	return NxCookTriangleMesh(desc,stream);
#endif
}

UINT64 mpManager::GenerateActorID()
{ 
	return ++m_LastID;
}

void mpManager::CreateTerrain( fcoWorld* pWorld )
{ 
	if( m_bDisable )
		return;

	if( pWorld == NULL )
		return;

	// Build physical model
	NxTriangleMeshDesc terrainDesc;
	terrainDesc.numVertices					= pWorld->GetVBNum();
	terrainDesc.numTriangles				= pWorld->GetIBNum();
	terrainDesc.pointStrideBytes			= sizeof(NxVec3);
	terrainDesc.triangleStrideBytes			= 3*sizeof(NxU32);
	terrainDesc.points						= pWorld->GetVertex();
	terrainDesc.triangles					= pWorld->GetFaces();
	terrainDesc.flags						= 0;
	terrainDesc.heightFieldVerticalAxis		= NX_Y;
	terrainDesc.heightFieldVerticalExtent	= -1000.0f;

	bool status = InitCooking(m_pMyAllocator,&m_pMyErrorStream);
	if( status == false ) {
		return;
	}

	MemoryWriteBuffer buf;
	status = CookTriangleMesh(terrainDesc, buf);
	if( status == false ) {
		return;
	}
	MemoryReadBuffer readBuffer(buf.data);
	m_pTerrainMesh = m_pPhysicsSDK->createTriangleMesh(readBuffer);

	NxTriangleMeshShapeDesc terrainShapeDesc;
	terrainShapeDesc.meshData	= m_pTerrainMesh;
	terrainShapeDesc.shapeFlags	= NX_SF_FEATURE_INDICES;

	NxActorDesc ActorDesc;
	ActorDesc.shapes.pushBack(&terrainShapeDesc);
	m_pTerrain = m_pScene->createActor(ActorDesc);

	/*
	//****************************************************************************
	// 동적 충돌판정용 오브젝트 생성
	//****************************************************************************
	// 초기화
	NxActorDesc actorDesc;
	actorDesc.setToDefault();

	// 구체의 액터 생성
	NxSphereShapeDesc sphereDesc;
	// 초기화
	sphereDesc.setToDefault();
	// 공의 반경을 지정
	sphereDesc.radius = 2.0f;

	// 이 부근에서 동적으로 설정한다
	NxBodyDesc bodyDesc;
	// 초기화
	bodyDesc.setToDefault();
	// 회전에 의한 감쇠계수
	bodyDesc.angularDamping = 0.5f;

	// 구를 액터에 추가
	actorDesc.shapes.pushBack(&sphereDesc);
	// 여기서 동적정보를 지정
	actorDesc.body          = &bodyDesc;
	// 질량밀도
	actorDesc.density         = 0.01f;
	// 월드공간상의 위치
	actorDesc.globalPose.t  = NxVec3(30.0f, 20.0f, 30.0f);

	// 씬에 대해 액터 추가
	m_pScene->createActor(actorDesc);
	*/

	CloseCooking();
}

void mpManager::SetCB_Sleep(UINT64 uActorID,SleepCB_ptr CB_Sleep)
{
	m_hashActorSleepCallback.insert( std::pair<UINT64,SleepCB_ptr>(uActorID,CB_Sleep) );
}

void mpManager::SetCB_Contact(UINT64 uActorID,ContactCB_ptr CB_Contact)
{
 	m_hashActorContactCallback.insert( std::pair<UINT64,ContactCB_ptr>(uActorID,CB_Contact) );
}

/*
bool mpManager::AddCube( UINT64 uActorID, const mcoCOBB& OBB, const F3dMatrix& mtxLocal )
{
	if( m_bDisable )
		return false;


	MBHashMap< UINT64, NxActor* >::iterator it = m_hashActor.find( uActorID );
	if( it == m_hashActor.end() )
		return false;

	NxActor* pActor = it->second;
	NxBoxShapeDesc BoxDesc;
	BoxDesc.dimensions = ToNxVector( OBB.GetExtent() );
	ToNxMat34( BoxDesc.localPose, mtxLocal );
	NxShape* pNewShape = pActor->createShape( BoxDesc );
	pNewShape->setFlag( NX_SF_VISUALIZATION, false );

	return true;
}
*/

bool mpManager::AddSphere( UINT64 uActorID, float fRadius, const F3dMatrix& mtxLocal )
{
	if( m_bDisable )
		return false;

	MBHashMap< UINT64, NxActor* >::iterator it = m_hashActor.find( uActorID );
	if( it == m_hashActor.end() )
		return false;

	NxActor* pActor = it->second;
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = fRadius;
	ToNxMat34( sphereDesc.localPose, mtxLocal );
	NxShape* pNewShape = pActor->createShape( sphereDesc );
	pNewShape->setFlag( NX_SF_VISUALIZATION, false );

	return true;
}

bool mpManager::AddCapsule( UINT64 uActorID, float fHeight, float fRadius, const F3dMatrix& mtxLocal )
{ 
	if( m_bDisable )
		return false;

	MBHashMap< UINT64, NxActor* >::iterator it = m_hashActor.find( uActorID );
	if( it == m_hashActor.end() )
		return false;

	NxActor* pActor = it->second;
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.height = fHeight;
	capsuleDesc.radius = fRadius;
	ToNxMat34( capsuleDesc.localPose, mtxLocal );
	NxShape* pNewShape = pActor->createShape( capsuleDesc );
	pNewShape->setFlag( NX_SF_VISUALIZATION, false );

	return true;
}

NxActor* mpManager::FindActor( UINT64 uActorID )
{
	if( m_bDisable )
		return NULL;

	MBHashMap< UINT64, NxActor* >::iterator it = m_hashActor.find( uActorID );
	if( it == m_hashActor.end() )
		return NULL;

	return it->second;
}

const WCHAR* mpManager::FindName( UINT64 uActorID )
{
	if( m_bDisable )
		return NULL;

	MBHashMap< UINT64, std::wstring >::iterator it = m_hashName.find( uActorID );
	if( it == m_hashName.end() )
		return NULL;

	return it->second.c_str();
}

UINT64 mpManager::CreateActor(NxActor* pNewActor,std::wstring strName)
{
	if(pNewActor == NULL)
		return 0;

	UINT64 uActorID = GenerateActorID();
	pNewActor->userData = (void*)uActorID;
	m_hashActor.insert( std::pair< UINT64, NxActor* >( uActorID, pNewActor ) );
	m_hashName.insert( std::pair< UINT64, std::wstring >( uActorID, strName ) );	

	if(pNewActor && m_pTerrain)
	{
		m_pScene->setActorPairFlags(*pNewActor, *m_pTerrain, NX_NOTIFY_ON_START_TOUCH|NX_NOTIFY_ON_TOUCH|NX_NOTIFY_ON_END_TOUCH);
	}

	return uActorID;
}

UINT64 mpManager::CreateActor( const mpActorDesc& ActorDesc, const std::vector< mpShapeDesc* >& vShapeDesc, const F3dMatrix& mtxWorld_NoScale, const float fWorldScale )
{
	if( m_bDisable )
		return 0;

	bool bScale = fabsf( fWorldScale - 1.f ) > ALMOST_ZERO;
	NxActorDesc nxActorDesc;
	NxBodyDesc nxBodyDesc;
	ActorDesc.GetNx( nxActorDesc );
	if( ActorDesc.bBody ) { //dynamic actor일경우 bodydesc세팅
		ActorDesc.Body.GetNx( nxBodyDesc );
		nxActorDesc.body = &nxBodyDesc;
	}

	if( ActorDesc.cType == CONTROL_KINEMATIC ) { //kinematic일경우 flag세팅
		nxActorDesc.flags |= NX_BF_KINEMATIC;
	}
	nxActorDesc.flags &= ~NX_BF_VISUALIZATION;

	F3dMatrix mtxWorld;
	if( bScale ) {
		Scaling( mtxWorld, F3dVector( fWorldScale, fWorldScale, fWorldScale ) );
		mtxWorld *= mtxWorld_NoScale;
	}
	else {
		mtxWorld = mtxWorld_NoScale;
	}
	F3dMatrix mtxActorWorld, mtxInvActorWorld;
	if( bScale ) {
		Scaling( mtxActorWorld, F3dVector( fWorldScale, fWorldScale, fWorldScale ) );
		mtxActorWorld *= ActorDesc.mtxWorld;
		Inverse( mtxInvActorWorld, ActorDesc.mtxWorld );

		F3dMatrix mtxActorWorld_Scale = ActorDesc.mtxWorld * mtxWorld;
		ToNxMat34( nxActorDesc.globalPose, ActorDesc.mtxWorld * mtxWorld_NoScale );
		nxActorDesc.globalPose.t.x = mtxActorWorld_Scale._41 / g_fUnitScale;
		nxActorDesc.globalPose.t.y = mtxActorWorld_Scale._42 / g_fUnitScale;
		nxActorDesc.globalPose.t.z = mtxActorWorld_Scale._43 / g_fUnitScale;
	}
	else {
		F3dMatrix matWorld = ActorDesc.mtxWorld * mtxWorld_NoScale;
		ToNxMat34( nxActorDesc.globalPose,  matWorld);
		nxActorDesc.globalPose.t.x /= g_fUnitScale;
		nxActorDesc.globalPose.t.y /= g_fUnitScale;
		nxActorDesc.globalPose.t.z /= g_fUnitScale;
	}

	std::vector< NxShapeDesc* > vNxShapeDesc;
	std::vector< mpShapeDesc* >::const_iterator it = vShapeDesc.begin();
	for(; it != vShapeDesc.end(); ++it )
	{
		const mpShapeDesc* pShapeDest = *it;

		F3dVector vtxLocalPos = ::GetPos( pShapeDest->mtxLocalTM );
		if( bScale ) {
			F3dMatrix mtxShapeWorldTM_Scale, mtxLocalTM_Scale;
			mtxShapeWorldTM_Scale = pShapeDest->mtxLocalTM * mtxActorWorld;
			vtxLocalPos = ::GetPos( mtxShapeWorldTM_Scale * mtxInvActorWorld );
		}

		switch( pShapeDest->GetType() )
		{
		case SHAPE_SPHERE:
			{
				NxSphereShapeDesc* nxSphereShapeDesc = new NxSphereShapeDesc;
				((mpSphereShapeDesc*)pShapeDest)->GetNx( *nxSphereShapeDesc, vtxLocalPos );
				nxSphereShapeDesc->radius *= fWorldScale;
				nxActorDesc.shapes.pushBack( nxSphereShapeDesc );
				vNxShapeDesc.push_back( nxSphereShapeDesc );
			}
			break;
		case SHAPE_BOX:
			{
				NxBoxShapeDesc* nxBoxShapeDesc = new NxBoxShapeDesc;
				((mpBoxShapeDesc*)pShapeDest)->GetNx( *nxBoxShapeDesc, vtxLocalPos );
				nxBoxShapeDesc->dimensions.x *= fWorldScale;
				nxBoxShapeDesc->dimensions.y *= fWorldScale;
				nxBoxShapeDesc->dimensions.z *= fWorldScale;
				nxActorDesc.shapes.pushBack(nxBoxShapeDesc);
				vNxShapeDesc.push_back( nxBoxShapeDesc );
			}
			break;
		case SHAPE_CAPSULE:
			{
				NxCapsuleShapeDesc* nxCapsuleShapeDesc = new NxCapsuleShapeDesc;
				((mpCapsuleShapeDesc*)pShapeDest)->GetNx( *nxCapsuleShapeDesc, vtxLocalPos );
				nxCapsuleShapeDesc->height *= fWorldScale;
				nxCapsuleShapeDesc->radius *= fWorldScale;
				nxActorDesc.shapes.pushBack(nxCapsuleShapeDesc);
				vNxShapeDesc.push_back( nxCapsuleShapeDesc );
			}
			break;
		};
	}

	if( nxActorDesc.isValid() == false )
		return 0;

	if( m_pScene == NULL )
		return 0;

	NxActor* pNewActor = m_pScene->createActor(nxActorDesc);
	if(pNewActor == NULL)
		return 0;

	UINT64 uActorID = GenerateActorID();
	pNewActor->userData = (void*)uActorID;
	m_hashActor.insert( std::pair< UINT64, NxActor* >( uActorID, pNewActor ) );
	m_hashName.insert( std::pair< UINT64, std::wstring >( uActorID, ActorDesc.strName ) );	

	for( int i = 0; i < (int)vNxShapeDesc.size(); ++i )
	{
		delete vNxShapeDesc[i];
	}
	vNxShapeDesc.clear();

	if(pNewActor&&m_pTerrain)
	{
		m_pScene->setActorPairFlags(*pNewActor, *m_pTerrain, NX_NOTIFY_ON_START_TOUCH|NX_NOTIFY_ON_TOUCH|NX_NOTIFY_ON_END_TOUCH);
	}

	return uActorID;
}

UINT64 mpManager::CreateJoint( const mpJointDesc* pJointDesc, const F3dMatrix& mtxWorld )
{
	if( m_bDisable )
		return 0;

	NxJoint* pJoint = NULL;
	switch( pJointDesc->GetType() )
	{
	case NX_JOINT_REVOLUTE:
		{
			NxRevoluteJointDesc JointDesc;
			pJointDesc->GetNxRevoluteJointDesc( JointDesc, mtxWorld );
			if( JointDesc.isValid() == false )
				return 0;

			pJoint = m_pScene->createJoint( JointDesc );
		}
		break;
	case NX_JOINT_SPHERICAL:
		{
			NxSphericalJointDesc JointDesc;
			pJointDesc->GetNxSphericalJointDesc( JointDesc, mtxWorld );
			if( JointDesc.isValid() == false )
				return 0;

			pJoint = m_pScene->createJoint( JointDesc );
		}
		break;
	case NX_JOINT_D6:
		{
			NxD6JointDesc JointDesc;
			pJointDesc->GetNxD6JointDesc( JointDesc, mtxWorld );
			if( JointDesc.isValid() == false )
				return 0;

			pJoint = m_pScene->createJoint( JointDesc );
		}
		break;
	case NX_JOINT_FIXED:
		{
			NxFixedJointDesc JointDesc;
			pJointDesc->GetNxFixedJointDesc( JointDesc, mtxWorld );
			if( JointDesc.isValid() == false )
				return 0;

			pJoint = m_pScene->createJoint( JointDesc );
		}
		break;
	};

	if( pJoint == NULL )
		return 0;

	UINT64 uJointID = GenerateActorID();
	m_hashJoint.insert( std::pair< UINT64, NxJoint* >( uJointID, pJoint ) );
	return uJointID;
}

NxJoint* mpManager::FindJoint( UINT64 uJointID )
{
	if( m_bDisable )
		return nullptr;

	MBHashMap< UINT64, NxJoint* >::iterator it = m_hashJoint.find( uJointID );
	if( it == m_hashJoint.end() )
		return NULL;

	return it->second;
}

ControllerManager* mpManager::GetControllerManager()
{
	return m_pControllerManager;
}

UINT64 mpManager::CreateCharacterController( const mpCharacterControllerData& ccd )
{
	++m_LastCharacterControllerID;

	mpCharacterController* cc = new mpCharacterController( ccd.vtxCenter, ccd.fRadius, ccd.fHeight, ccd.fSlopeLimitDegree, ccd.fSkinWidth, ccd.fStepOffset );
	m_hashCharacterController.insert( std::make_pair( m_LastCharacterControllerID, cc ) );
	return m_LastCharacterControllerID;
}

mpCharacterControllerPtr mpManager::FindCharacterController( UINT64 uCharacterControllerID )
{

	mpCharacterControllerPtr null;
	auto it = m_hashCharacterController.find(uCharacterControllerID);
	if( it == m_hashCharacterController.end() )
		return null;
	return it->second;
}

void mpManager::Process( double dTime, double dt )
{
	if( m_bDisable )
		return;

	if( m_pScene == NULL )
		return;

	if( m_nPlayState != PLAY_PLAY )
		return;

	{
		m_pControllerManager->updateControllers();

		m_pScene->simulate( (NxReal)dt );
		m_pScene->flushStream();
		bool bTemp = m_pScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
		assert( bTemp );
	}
}

float mpManager::GetUnitScale() const
{
	return g_fUnitScale;
}

void mpManager::SetDebugRender( NxParameter Visualize, bool bDebug )
{
	m_bDebugRender = bDebug;
	m_pPhysicsSDK->setParameter( Visualize, bDebug );
	//	m_pPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, bDebug );
	//	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, bDebug );
}

bool mpManager::IsDebugRender( NxParameter Visualize ) const
{
	return m_pPhysicsSDK->getParameter( Visualize ) > ALMOST_ZERO;
}

void mpManager::CreateGroundPlane()
{
	if( m_bDisable )
		return;

	if( m_pGroundPlane )
		return;
	NxPlaneShapeDesc planeDesc;
	planeDesc.group = 31;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	m_pGroundPlane = m_pScene->createActor(actorDesc);
}

void mpManager::ReleaseGroundPlane()
{
	if( m_pGroundPlane == NULL )
		return;
	m_pScene->releaseActor( *m_pGroundPlane );
	m_pGroundPlane = NULL;
}

void mpManager::onContact(NxActor **actors,bool *bDeleted)
{
	for(int i=0;i<2;++i)
	{
		if(bDeleted[i] == false &&  actors[i] && actors[i]->userData)
		{
			MBHashMap< UINT64, ContactCB_ptr >::iterator it = m_hashActorContactCallback.find( (UINT64)actors[i]->userData );
			if( it != m_hashActorContactCallback.end() )
			{
				ContactCB_ptr CB_Contact = it->second;
				if(CB_Contact)
				{
					CB_Contact(actors);
				}
			}
		}
	}
}

void mpManager::onSleep( NxActor **actors, NxU32 count )
{
	NX_ASSERT(count > 0);
	while(count--)
	{
		NxActor *thisActor = *actors;
		MBHashMap< UINT64, SleepCB_ptr >::iterator it = m_hashActorSleepCallback.find( (UINT64)thisActor->userData );
		if( it == m_hashActorSleepCallback.end() )
			continue;

		SleepCB_ptr CB_Sleep = it->second;
		if(CB_Sleep)
		{
			CB_Sleep((UINT64*)thisActor->userData);
		}
		actors++;
	}
}

void mpManager::Disable()
{
	m_bDisable = true;
}
#include "FeatherXEnginePrecompile.h"

F3dCPhysics::F3dCPhysics():
m_pCB_Contact(NULL)
{
	Identity(m_mtxWorldTrans);
	m_fScale = 1.f;
	m_pPhysicsActor = NULL;
}

F3dCPhysics::~F3dCPhysics()
{

}

void F3dCPhysics::SetWorldTransform( const F3dMatrix& mtxTrans )
{
	Scaling( m_mtxWorldTrans, m_fScale, m_fScale, m_fScale );
	m_mtxWorldTrans *= mtxTrans;
}

void F3dCPhysics::Process()
{
	if(m_pPhysicsActor == NULL)
		return;

	NxActor* nxActor = mpManager::GetSingleton()->FindActor( m_pPhysicsActor->ActorID );
	float fUnitScale = mpManager::GetSingleton()->GetUnitScale();
	switch( m_pPhysicsActor->cCurType )
	{
	case CONTROL_DYNAMIC:
		{
			F3dMatrix mtxWorldTM;
			F3dMatrix mtxOffset;
			Identity(mtxOffset);
			::SetPos(mtxOffset,m_physicResultAni.vecOffset);
			GetGlobalTM( mtxWorldTM, nxActor );

			mtxWorldTM = mtxOffset * mtxWorldTM;
			SetWorldTransform( mtxWorldTM );
		}
		break;
	case CONTROL_STATIC:
		break;
	case CONTROL_KINEMATIC:
		{
			F3dMatrix mtxWorld = m_mtxWorldTrans;
			SetGlobalTM( nxActor, mtxWorld );
		}
		break;
	};
}

F3dActor* F3dCPhysics::CreatePhysicsActor()
{
	float fUnitScale = mpManager::GetSingleton()->GetUnitScale();
	UINT64 ActorID = mpManager::GetSingleton()->CreateActor( m_ActorDesc, m_vShapeDesc, m_mtxWorldTrans, m_fScale );
	if( ActorID == 0 )
		return NULL;

	m_pPhysicsActor = new F3dActor;
	m_pPhysicsActor->ActorID = ActorID;
	m_vPhysicsActor.push_back(m_pPhysicsActor);

	ContactCB_ptr ContactCB = std::bind1st( std::mem_fun(&F3dCPhysics::PhysicsContact), this );
	mpManager::GetSingleton()->SetCB_Contact(ActorID,ContactCB);

	return m_pPhysicsActor;
}

void F3dCPhysics::addForce(F3dVector vecForce)
{
	NxActor* nxActor = mpManager::GetSingleton()->FindActor( m_pPhysicsActor->ActorID );
	NxVec3 vecNxForce(vecForce.x,vecForce.y,vecForce.z);

	nxActor->addForce(vecNxForce);
}

void F3dCPhysics::setAngularVelocity(F3dVector vecForce)
{
	NxActor* nxActor = mpManager::GetSingleton()->FindActor( m_pPhysicsActor->ActorID );
	NxVec3 vecNxForce(vecForce.x,vecForce.y,vecForce.z);

	nxActor->setAngularVelocity(vecNxForce);
}

void F3dCPhysics::setLinearVelocity(F3dVector vecVelocity)
{
	NxActor* nxActor = mpManager::GetSingleton()->FindActor( m_pPhysicsActor->ActorID );
	NxVec3 vecNxVelocity(vecVelocity.x,vecVelocity.y,vecVelocity.z);

	nxActor->setLinearVelocity(vecNxVelocity);
}

F3dVector F3dCPhysics::getLinearVelocity()
{
	NxVec3 vecNxVelocity;

	NxActor* nxActor = mpManager::GetSingleton()->FindActor( m_pPhysicsActor->ActorID );
	vecNxVelocity = nxActor->getLinearVelocity();

	return F3dVector(vecNxVelocity.x,vecNxVelocity.y,vecNxVelocity.z);
}

void F3dCPhysics::PhysicsContact(NxActor **actors)
{
	if( m_pCB_Contact )	{
		m_pCB_Contact(actors);
	}
}


bool F3dCPhysics::IsMyActor(int nID)
{
	for(auto it = m_vPhysicsActor.begin();it != m_vPhysicsActor.end();++it)
	{
		if((*it)->ActorID == nID)
			return true;
	}

	if(m_pPhysicsActor && m_pPhysicsActor->ActorID == nID)
		return true;

	return false;
}


void F3dCPhysics::SetKinematic( bool bEnabe )
{
	if( m_vPhysicsActor.empty() && m_pPhysicsActor == NULL) {
		CreatePhysicsActor();
	}

	std::vector< F3dActor* >::iterator it = m_vPhysicsActor.begin();
	for(; it != m_vPhysicsActor.end(); ++it )
	{
		F3dActor* pPhysicsActor = (*it);
		if( pPhysicsActor == NULL )
			continue;

		NxActor* nxActor = mpManager::GetSingleton()->FindActor( pPhysicsActor->ActorID );
		if( nxActor == NULL )
			continue;
		if( bEnabe ) {
			pPhysicsActor->cCurType = CONTROL_KINEMATIC;
			nxActor->raiseBodyFlag( NX_BF_KINEMATIC );
		}
		else {
			pPhysicsActor->cCurType = CONTROL_DYNAMIC;
			nxActor->clearBodyFlag( NX_BF_KINEMATIC );
		}
	}
}

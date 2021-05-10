#include "FeatherXClientPrecompile.h"

FCCharacterPlayer::FCCharacterPlayer() :
m_pModel(NULL),
m_pTail(NULL),
m_nJump(20)
{

}

FCCharacterPlayer::~FCCharacterPlayer()
{

}

void FCCharacterPlayer::Init(const F3dVector& pos,const F3dVector& dir)
{
	m_pModel = CFeatherXModelManager::GetSingleton()->CreateModel(L"models\\robot.xml");
	
	m_pModel->SetPos(F3dVector(0,40,0));
	m_pModel->SetDir(F3dVector(1,0,0));
	
	m_pTail  = F3dCTailManager::GetSingleton()->CreateTail();

	ContactCB_ptr ContactCB = std::bind1st( std::mem_fun(&FCCharacterPlayer::CB_Contact),this);
	m_pModel->SetCB_Contact(ContactCB);

	m_pTail->Init(  0.3f );
	m_pTail->SetTexture(L"Effect/Texture/w_effect05.dds");
	m_pTail->SetTailLengthTime( 3.f );
}

void FCCharacterPlayer::Process(double dTime,double dt)
{
	F3dMatrix matWorld = m_pModel->GetWorldTransform();
	F3dVector vtxPos = ::GetPos(matWorld);
	m_pTail->AddTail( dTime, vtxPos );
	m_pTail->Process( dTime, dt );
}

void FCCharacterPlayer::Render(double dTime,double dt)
{	

}

void FCCharacterPlayer::Destroy()
{
}

void FCCharacterPlayer::CB_Contact(NxActor **actors)
{

}

void FCCharacterPlayer::ForWard()
{
	m_pModel->SetPos(m_pModel->GetPos() + m_pModel->GetDir() * 1);
}

void FCCharacterPlayer::BackWard()
{
	m_pModel->SetPos(m_pModel->GetPos() + m_pModel->GetDir() * -1);
}

void FCCharacterPlayer::Left()
{
	F3dVector vecDir = m_pModel->GetDir();
	float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir);
	float fRadian = ToRadian(fAngle - 1);
	vecDir.x = sinf( fRadian );
	vecDir.z = cosf( fRadian );
	m_pModel->SetDir(vecDir);
}

void FCCharacterPlayer::Right()
{
	F3dVector vecDir = m_pModel->GetDir();
	float fAngle = GetAngleXZ( F3dVector( 0, 0, 1.f ), vecDir);
	float fRadian = ToRadian(fAngle + 1);
	vecDir.x = sinf( fRadian );
	vecDir.z = cosf( fRadian );
	m_pModel->SetDir(vecDir);
}

void FCCharacterPlayer::Jump()
{
	m_pModel->addForce(F3dVector(0,m_nJump,0));
}

void FCCharacterPlayer::Stop()
{
	m_pModel->SetKinematic(true);
}

void FCCharacterPlayer::Break()
{
	//m_pModel->setAngularVelocity(F3dVector(0,0,0));
}
#include "FeatherXEnginePrecompile.h"
WORD* F3dCParticle::s_pIndexBuffer = NULL;

F3dCParticle::F3dCParticle():
m_pParticleVertex(NULL),
m_nVertexNum(0),
m_nParticleNum(0),
m_vecPos(0,0,0)
{

}

F3dCParticle::~F3dCParticle()
{

}

void F3dCParticle::MakeIndexBuffer()
{
	if( s_pIndexBuffer )
		return;

	s_pIndexBuffer = new WORD[ MAX_PARTICLE*6 ];
	int i = 0;
	for( int n = 0; n < MAX_PARTICLE; ++n, i += 6 )
	{
		int nIndex = n*4;
		s_pIndexBuffer[i] = nIndex;
		s_pIndexBuffer[i+2] = nIndex+1;
		s_pIndexBuffer[i+1] = nIndex+2;

		s_pIndexBuffer[i+3] = nIndex;
		s_pIndexBuffer[i+5] = nIndex+2;
		s_pIndexBuffer[i+4] = nIndex+3;
	}
}

void F3dCParticle::DeleteIndexBuffer()
{
	delete s_pIndexBuffer;
	s_pIndexBuffer = NULL;
}

void F3dCParticle::SetPlay()
{
	m_bPlay = true;
	m_dCurTime = 0;
	m_dStartTick = GetTickCount();
	m_dShootStartTime = 0;
	m_vParticle.clear();
}

void F3dCParticle::CameraMatrixUpdate()
{
}

void F3dCParticle::Reset()
{
	m_vParticle.clear();

	float fEmitterShootTimeInterval	= m_pParticleTemplate->fEmitterShootTimeInterval;
	m_dShootStartTime = -fEmitterShootTimeInterval;
}

bool F3dCParticle::Init(WCHAR* strFile)
{
	m_pParticleTemplate = new F3dCParticleTemplate;
	m_pParticleTemplate->Init(strFile);

	if(FAILED(D3DXCreateTextureFromFile(F3dCDevice::GetSingleton()->GetD3DDevice()
		,m_pParticleTemplate->strTextureFileName.c_str(),&m_pTexture  )))
		return false;

	m_dShootStartTime = 0;
	m_dCurTime = 0;
	m_dStartTick = GetTickCount();
	D3DXMatrixIdentity( &m_matWorld );
	MakeIndexBuffer();
	return true;
}

void F3dCParticle::SetParentInfo(F3dParticleParentInfo& ParentInfo)
{
	m_ParentInfo = ParentInfo;
}

/************************************************************************/
/* ADDPARTICLE                                                          */
/************************************************************************/

bool F3dCParticle::AddParticle( double dTime )
{		
	int nCount = 0;
	while( true )
	{
		F3dParticle* pParticle = new F3dParticle;
		SetPos(pParticle);
		SetVelocity(pParticle);
		SetRot(pParticle);

		pParticle->dStartTime = dTime;
		pParticle->dEndTime = ( dTime + m_pParticleTemplate->fBillBoardLifeTime );


		m_vParticle.push_back( pParticle );	
		++nCount;		
		int NumberOfShoot = m_pParticleTemplate->nEmitterNumberOfShoot;
		if(nCount >= NumberOfShoot)
			break;
	}

	return true;
}

/************************************************************************/
/* SETROT                                                               */
/************************************************************************/

void F3dCParticle::SetRot(F3dParticle* pParticle)
{
	float fMinRot = m_pParticleTemplate->fMinRot;
	float fMaxRot = m_pParticleTemplate->fMaxRot;

	pParticle->fRot = GetRandom(fMinRot,fMaxRot);
	if(m_pParticleTemplate->bRandom)
	{
		pParticle->fStartRot = (rand() % 360 ) * F3D_PI/180;
	}
}

/************************************************************************/
/* SETPOS                                                               */
/************************************************************************/

void F3dCParticle::SetPos(F3dParticle* pParticle)
{
	int	nEmitterType = m_pParticleTemplate->nEmitterType;
	if(nEmitterType == EMITTER_TYPE_BOX) 
	{
		pParticle->vtxStartPos.x = GetRandom(-m_pParticleTemplate->fEmitterXLength * 0.5f,m_pParticleTemplate->fEmitterXLength * 0.5f);
		pParticle->vtxStartPos.y = GetRandom(0.f,m_pParticleTemplate->fEmitterYLength);
		pParticle->vtxStartPos.z = GetRandom(-m_pParticleTemplate->fEmitterZLength * 0.5f,m_pParticleTemplate->fEmitterZLength * 0.5f);
	}
	else if (nEmitterType == EMITTER_TYPE_CYLINDER) 
	{
		F3dVector vecDir;
		vecDir.x = Rand()*2.f + -1.f;	vecDir.y = Rand()*2.f + -1.f;	vecDir.z = Rand()*2.f + -1.f;
		VecNormalize(vecDir);
		pParticle->vtxStartPos.x = vecDir.x * m_pParticleTemplate->fEmitterXLength;
		pParticle->vtxStartPos.y = GetRandom(0.f,m_pParticleTemplate->fEmitterYLength);
		pParticle->vtxStartPos.z = vecDir.z * m_pParticleTemplate->fEmitterXLength;
	}
	else if(nEmitterType == EMITTER_TYPE_SPHERE) 
	{
		F3dVector vecDir;
		vecDir.x = Rand()*2.f + -1.f;	vecDir.y = Rand()*2.f + -1.f;	vecDir.z = Rand()*2.f + -1.f;
		VecNormalize(vecDir);
		pParticle->vtxStartPos = vecDir * m_pParticleTemplate->fEmitterXLength;			
	}

	F3dVector vecParentPos;
	if(m_ParentInfo.pParentModel) {
		F3dCModel *pModel = m_ParentInfo.pParentModel;
		F3dMatrix matWorld = m_ParentInfo.pParentModel->GetWorldTransform();
		if(m_ParentInfo.strLinkedNode != L"") {
			D3DXMATRIXA16* mat16 = pModel->GetNodeMat(m_ParentInfo.strLinkedNode);
			if(mat16) {
				matWorld = (*mat16) * matWorld;
			}
		}
		vecParentPos = ::GetPos(matWorld);
		vecParentPos += m_ParentInfo.vecOffset;
	}

	pParticle->vtxStartPos += vecParentPos;
}

/************************************************************************/
/* VELOCITY                                                             */
/************************************************************************/

void F3dCParticle::SetVelocity(F3dParticle* pParticle)
{
	int	nShootType = m_pParticleTemplate->nShootType;
	float fMinSpeed = m_pParticleTemplate->fMinSpeed;
	float fMaxSpeed = m_pParticleTemplate->fMaxSpeed;

	D3DXVECTOR3 vecVelocity;
	D3DXVECTOR3 vecDirection = m_pParticleTemplate->vecDirection;
	float fSpeed = fMinSpeed + rand()*(fMaxSpeed-fMinSpeed);

	if(nShootType == EFFECT_SHOOT_TYPE_RADIAL_SHAPE)
	{
		vecVelocity.x = Rand()*2.f + -1.f;	vecVelocity.y = Rand()*2.f + -1.f;	vecVelocity.z = Rand()*2.f + -1.f;
		VecNormalize(vecVelocity);
	}
	else if(nShootType == EFFECT_SHOOT_TYPE_USER_DEFINE)
	{
		float fConeAnlge = m_pParticleTemplate->fConeAngle;
		int nCone100 = (int)(fConeAnlge * 100.f);
		if( nCone100 ) 
		{
			float fAngle = (float)(rand()%nCone100);
			fAngle /= 100.f;
			float fRadian = ToRadian( fAngle );
			D3DXVECTOR3 vecA;
			vecA.x = Rand()*200.f-100.f;	vecA.y = Rand()*200.f-100.f;	vecA.z = Rand()*200.f-100.f;

			VecNormalize( vecA );
			vecVelocity = GetRotateAxisAngle( vecDirection, vecA, fRadian );
			VecNormalize(vecVelocity);
		}
		else	
		{
			vecVelocity = vecDirection;
			VecNormalize( vecVelocity );
		}
	}
	else if(nShootType == EFFECT_SHOOT_TYPE_BOX_THE_COMPASS)
	{
		D3DXVECTOR3 vecAxis(vecDirection.x,vecDirection.y,vecDirection.z);
		vecVelocity = vecAxis - pParticle->vtxStartPos;
		VecNormalize( vecVelocity );
	}

	pParticle->vecStartVelocity = (vecVelocity * fSpeed);
}
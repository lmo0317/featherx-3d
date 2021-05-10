#include "FeatherXEnginePrecompile.h"

bool F3dCParticle::Process( double dTime )
{
	m_dCurTime = (GetTickCount() - m_dStartTick) / 1000.0f;
	if( m_bPlay == false && m_vParticle.empty() )
		return false;

	float fEmitterLifeTime = m_pParticleTemplate->fEmitterLifeTime;
	int	  nEmitterNumberOfShoot = m_pParticleTemplate->nEmitterNumberOfShoot;
	float fEmitterShootTimeInterval = m_pParticleTemplate->fEmitterShootTimeInterval;	
	float fBillBoardLifeTime = m_pParticleTemplate->fBillBoardLifeTime;

	if(fEmitterShootTimeInterval <= 0 || nEmitterNumberOfShoot <= 0 || 
		(fEmitterLifeTime < fEmitterShootTimeInterval && fEmitterLifeTime != 0) || 
		fBillBoardLifeTime <= 0)
	{
		Reset();
		m_bPlay = false;
		return false;
	}

	bool add_break = false;
	if(fEmitterLifeTime && m_dCurTime >= fEmitterLifeTime )
		add_break = true;

	if(m_dCurTime - m_dShootStartTime > fEmitterShootTimeInterval && add_break == false)
	{
		m_dShootStartTime = m_dCurTime;
		AddParticle( m_dCurTime );
	}

	const F3dVector4	vec4StartColor	= m_pParticleTemplate->vec4StartColor;
	const F3dVector4	vec4EndColor	= m_pParticleTemplate->vec4EndColor;
	const F3dVector4	vec4MiddleColor= m_pParticleTemplate->vec4MiddleColor;
	const F3dVector	vecGravity	 	= m_pParticleTemplate->vecGravity;
	const F3dVector2	vecStartSize	= m_pParticleTemplate->vecStartSize;
	const F3dVector2	vecEndSize		= m_pParticleTemplate->vecEndSize;

	int nParticleNum = 0;
	if(fEmitterLifeTime != 0 )
	{
		nParticleNum = (int)(((fEmitterLifeTime / fEmitterShootTimeInterval) + 1) * nEmitterNumberOfShoot);
	}
	else
	{
		nParticleNum = (int)(((fBillBoardLifeTime/fEmitterShootTimeInterval) + 1) * nEmitterNumberOfShoot);
	}

	m_nVertexNum = nParticleNum*4;	
	if(m_nParticleNum != nParticleNum)    
	{
		Reset();
		delete []m_pParticleVertex;
		m_pParticleVertex = new F3dParticleVertex[m_nVertexNum];
		m_nParticleNum = nParticleNum;
	}

	int	nFrameNum = m_pParticleTemplate->nFrameNum;
	float fNumberOfRoutine	= m_pParticleTemplate->fNumberOfRoutine;
	std::vector< F3dParticle* >::iterator it = m_vParticle.begin();
	for(; it != m_vParticle.end();)
	{
		F3dParticle* pParticle = (*it);
		if( m_dCurTime >= pParticle->dEndTime ) {
			it = m_vParticle.erase( it );
			continue;
		}
		++it;
	}
	
	int i=0;
	it = m_vParticle.begin();	
	int kSize=m_vParticle.size();
	for(; it != m_vParticle.end(); ++it )
	{
		F3dParticle* pParticle = (*it);
		float fDt = (float)(m_dCurTime - pParticle->dStartTime);
		fDt = max( 0.f, fDt );
		pParticle->vtxPos.x = pParticle->vtxStartPos.x + (pParticle->vecStartVelocity.x * fDt) + (0.5f * ((vecGravity.x) ) * (fDt*fDt));
		pParticle->vtxPos.y = pParticle->vtxStartPos.y + (pParticle->vecStartVelocity.y * fDt) + (0.5f * ((vecGravity.y) ) * (fDt*fDt));
		pParticle->vtxPos.z = pParticle->vtxStartPos.z + (pParticle->vecStartVelocity.z * fDt) + (0.5f * ((vecGravity.z) ) * (fDt*fDt));		
	}

	std::sort( m_vParticle.begin(), m_vParticle.end(), F3dParticle::Greater );

	float		fParticleSize_X = 1.f;
	float		fParticleSize_Y = 1.f;
	D3DXVECTOR4	ParticleColor;
	it = m_vParticle.begin();	

	for( i = 0; it != m_vParticle.end(); ++it, ++i )
	{
		F3dParticle* pParticle = (*it);
		float fS = (float)( (m_dCurTime - pParticle->dStartTime) / (pParticle->dEndTime - pParticle->dStartTime) );
		if(fS <= 0.5)
			D3DXVec4Lerp( &ParticleColor, &vec4StartColor, &vec4MiddleColor, fS*2 );
		else
			D3DXVec4Lerp( &ParticleColor, &vec4MiddleColor, &vec4EndColor, (fS - 0.5f) *2 );

		fParticleSize_X = vecStartSize.x + fS*(vecEndSize.x - vecStartSize.x);
		fParticleSize_Y = vecStartSize.y + fS*(vecEndSize.y - vecStartSize.y);
		int nFrame = (int)((fS) * nFrameNum * fNumberOfRoutine ) % nFrameNum;

		for( int n = 0; n < 4; ++n )
		{
			int nIdx = (i*4)+n;
			m_pParticleVertex[nIdx].vtx4Point	   = MakeVector4( pParticle->vtxPos, (float)nFrame );
			switch(n)
			{
			case 0:
				m_pParticleVertex[nIdx].vtx2Vertex.x	= 0.f;
				m_pParticleVertex[nIdx].vtx2Vertex.y	= 0.f;
				m_pParticleVertex[nIdx].vtx4Color		= ParticleColor;
				m_pParticleVertex[nIdx].vtx2AxisRot.y	= float((pParticle->fRot * (F3D_PI/180)) * fS + pParticle->fStartRot);
				m_pParticleVertex[nIdx].vtx2AxisRot.x	= m_pParticleVertex[nIdx].vtx2AxisRot.y - 1.57f;
				break;
			case 1:
				m_pParticleVertex[nIdx].vtx2Vertex.x	= 1.f;
				m_pParticleVertex[nIdx].vtx2Vertex.y	= 0.f;
				m_pParticleVertex[nIdx].vtx4Color		= ParticleColor;
				m_pParticleVertex[nIdx].vtx2AxisRot.y	= float((pParticle->fRot * (F3D_PI/180)) * fS + pParticle->fStartRot);
				m_pParticleVertex[nIdx].vtx2AxisRot.x	= m_pParticleVertex[nIdx].vtx2AxisRot.y + 1.57f;
				break;
			case 2:
				m_pParticleVertex[nIdx].vtx2Vertex.x	= 1.f;
				m_pParticleVertex[nIdx].vtx2Vertex.y	= 1.f;
				m_pParticleVertex[nIdx].vtx4Color		= ParticleColor;
				m_pParticleVertex[nIdx].vtx2AxisRot.y	= float((pParticle->fRot * (F3D_PI/180)) * fS + pParticle->fStartRot + 3.14f);
				m_pParticleVertex[nIdx].vtx2AxisRot.x	= m_pParticleVertex[nIdx].vtx2AxisRot.y - 1.57f;
				break;
			case 3:
				m_pParticleVertex[nIdx].vtx2Vertex.x	= 0.f;
				m_pParticleVertex[nIdx].vtx2Vertex.y	= 1.f;
				m_pParticleVertex[nIdx].vtx4Color		= ParticleColor;
				m_pParticleVertex[nIdx].vtx2AxisRot.y	= float((pParticle->fRot * (F3D_PI/180)) * fS + pParticle->fStartRot + 3.14f);
				m_pParticleVertex[nIdx].vtx2AxisRot.x	= m_pParticleVertex[nIdx].vtx2AxisRot.y + 1.57f;
				break;
			};

			m_pParticleVertex[nIdx].vtx2Factor.x = fParticleSize_X/2;
			m_pParticleVertex[nIdx].vtx2Factor.y = fParticleSize_Y/2;
			m_pParticleVertex[nIdx].fRot		 = float((n * 1.57f - 0.785f) + (pParticle->fRot * (F3D_PI/180)) * fS + pParticle->fStartRot);			
		}
	}

	if( m_vParticle.empty() && add_break) {
		m_bPlay = false;
	}
	return true;
}


bool F3dCParticle::Render()
{
	CFeatherXShader* pShader = CFeatherXShaderManager::GetSingleton()->GetShader(_T("FX\\EffectParticleSystem.fx"));
	if(pShader == NULL)
		return false;

	LPD3DXEFFECT pEffect = pShader->GetEffect();
	if(pEffect == NULL)
		return false;

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetVertexDeclaration(pShader->GetDecl());
	DWORD value_src;
	DWORD value_dest;

	F3dCDevice::GetSingleton()->GetD3DDevice()->GetRenderState(D3DRS_SRCBLEND,&value_src);
	F3dCDevice::GetSingleton()->GetD3DDevice()->GetRenderState(D3DRS_DESTBLEND,&value_dest);

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Sampler States
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );

	switch( m_pParticleTemplate->nAlphaType )
	{
	case ALPHA_TYPE_BLENDING:
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		break;
	case ALPHA_TYPE_ADDITIVE:
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		break;
	case ALPHA_TYPE_SUBTRACTIVE:
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_BLENDOP,	D3DBLENDOP_REVSUBTRACT );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE );
		break;
	case ALPHA_TYPE_SRCALPHA_ONE:
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE );	
	};

	UINT nPass;
	D3DXHANDLE hTexture = pShader->GetHandleTexture();
	pEffect->SetTechnique( pShader->GetTechnique() );
	int nFrameWidthNum  = m_pParticleTemplate->nWidthFrameNum;
	int nFrameHeightNum = m_pParticleTemplate->nHeightFrameNum;

	if(hTexture)
		pEffect->SetTexture( hTexture, m_pTexture );

	D3DXVECTOR3 mat = m_matWorld;
	float fScale = VecLength( mat );
	pEffect->SetFloat( "FrameWidthNum",		(float)nFrameWidthNum );
	pEffect->SetFloat( "FrameHeightNum",	(float)nFrameHeightNum );
	pEffect->SetFloat( "Scale",				fScale );

	D3DXHANDLE matview = pShader->GetMatView();
	D3DXHANDLE matprojview = pShader->GetMatProjectionView();
	D3DXMATRIX mtxCameraView;
	D3DXMatrixIdentity(&mtxCameraView);	

	if(m_pParticleTemplate->nEmitterAxisType == EMITTER_TYPE_AXIS_X )
	{
		mtxCameraView._22 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._22;
		mtxCameraView._23 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._23;
		mtxCameraView._32 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._32;
		mtxCameraView._33 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._33;
	}
	else if(m_pParticleTemplate->nEmitterAxisType == EMITTER_TYPE_AXIS_Y )
	{
		mtxCameraView._11 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._11;
		mtxCameraView._13 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._13;
		mtxCameraView._31 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._31;
		mtxCameraView._33 = F3dCRenderMgr::GetSingleton()->GetViewMatrix()._33;
	}
	else if(m_pParticleTemplate->nEmitterAxisType == EMITTER_TYPE_AXIS_NONE )
	{
		D3DXMATRIXA16 temp;
		D3DXMatrixInverse(&temp,NULL,&F3dCRenderMgr::GetSingleton()->GetViewMatrix());
		mtxCameraView = temp;
	}
	else if(m_pParticleTemplate->nEmitterAxisType == EMITTER_TYPE_AXIS_ALL)
	{
		float ceta = 90 * (F3D_PI / 180);
		mtxCameraView._22 = cos(ceta);
		mtxCameraView._23 = -sin(ceta);
		mtxCameraView._32 = sin(ceta);
		mtxCameraView._33 = cos(ceta);
	}

	m_mtxViewProj = F3dCRenderMgr::GetSingleton()->GetViewMatrix() * F3dCRenderMgr::GetSingleton()->GetProjMatrix();
	pEffect->SetMatrix(matview,&mtxCameraView);
	pEffect->SetMatrix(matprojview,&m_mtxViewProj);

	HRESULT hResult = pEffect->Begin(&nPass,0);
	for(unsigned int i=0;i<nPass;++i)
	{	
		pEffect->BeginPass(i);
		pEffect->CommitChanges();
		if( !m_vParticle.empty() )
		{
			F3dCDevice::GetSingleton()->GetD3DDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, m_nVertexNum, (int)m_vParticle.size()*2, s_pIndexBuffer, D3DFMT_INDEX16, m_pParticleVertex,sizeof(F3dParticleVertex) );
		}
		pEffect->EndPass();
	}
	pEffect->End();

	if(m_pParticleTemplate->nAlphaType  == ALPHA_TYPE_SUBTRACTIVE ) 
	{
		F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_BLENDOP,	D3DBLENDOP_ADD );
	}

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SRCBLEND,value_src );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_DESTBLEND,value_dest);	
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	return true;
}

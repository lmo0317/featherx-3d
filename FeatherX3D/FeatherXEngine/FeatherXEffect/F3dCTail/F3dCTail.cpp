#include "FeatherXEnginePrecompile.h"

F3dCTail::F3dCTail()
:	m_fWidth(0),
	m_bRender(true),
	m_nVertexNum(0),
	m_pVB(NULL),
	m_dTailLengthTime(0),
	m_dOldTailTime(0),
	m_vtxPreVertex( 0, 0, 0 ),
	m_bFirst( true ),
	m_TextureID(0)
{
}

F3dCTail::~F3dCTail()
{
	SAFE_RELEASE( m_pVB );
}

void F3dCTail::Init( float fWidth )
{
	m_fWidth = fWidth;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();
	HRESULT hResult = pD3DDevice->CreateVertexBuffer( MAX_VERTEX*sizeof(F3dTexVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, NULL );
	if( hResult != S_OK ) {
		return;
	}

	D3DXCreateTextureFromFile( pD3DDevice, L"Effect/Texture/w_effect05.dds", &m_pTexture );
}

void F3dCTail::Finish()
{
	m_vtxControlPointLine.clear();
	m_vtxLine.clear();
	m_nVertexNum = 0;
}

void F3dCTail::SetTexture( const WCHAR* szTextureName )
{
	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();
	D3DXCreateTextureFromFile( pD3DDevice,szTextureName , &m_pTexture );
}

void F3dCTail::AddTail( double dTime, const F3dVertex& vtxV )
{
	if( dTime - m_dOldTailTime < 0.02 )
		return;

	if( m_bFirst ) {
		m_bFirst = false;
		m_vtxPreVertex = vtxV;
		return;
	}

	F3dVector vecFront( 0, 0, 1.f );
	F3dVector vecDir = m_vtxPreVertex - vtxV;
	VecNormalize( vecDir );
	F3dVector vecSide = vecDir ^ vecFront;
	VecNormalize( vecSide );

	float fHalfWidth = m_fWidth/2.f;
	F3dVector vtxPos[2];
	vtxPos[0] = fHalfWidth * vecSide;
	vtxPos[1] = -fHalfWidth * vecSide;

	vtxPos[0] += vtxV;
	vtxPos[1] += vtxV;

	AddVertexLine( dTime, vtxPos );

	m_vtxPreVertex = vtxV;
	m_dOldTailTime = dTime;
}

void F3dCTail::AddVertexLine( double dTime, F3dVertex* vtxV )
{
	F3dVertexLine VertexLine;

	if( m_vtxControlPointLine.empty() == false ) {
		int nEnd = (int)m_vtxControlPointLine.size()-1;
		if( dTime < m_vtxControlPointLine[ nEnd ].dTime ) {
			m_vtxControlPointLine.clear();
			return;
		}

		if( dTime - m_vtxControlPointLine[ nEnd ].dTime > 0.007f ) {
			VertexLine.dTime = dTime;
			VertexLine.vtxV[0] = vtxV[0];	VertexLine.vtxV[1] = vtxV[1];
			m_vtxControlPointLine.push_back( VertexLine );

			std::vector< F3dVertexLine >::iterator it = m_vtxControlPointLine.begin();
			for(; it != m_vtxControlPointLine.end(); )
			{
				if( dTime - it->dTime > GetTailLengthTime() ) {
					it = m_vtxControlPointLine.erase( it );
					continue;
				}
				++it;
			}
			for( it = m_vtxLine.begin(); it != m_vtxLine.end(); )
			{
				if( dTime - it->dTime > GetTailLengthTime() ) {
					it = m_vtxLine.erase( it );
					continue;
				}
				++it;
			}

			if( m_vtxControlPointLine.size() > 4 ) {
				VertexLine.dTime = dTime;
				int nEnd2 = (int)m_vtxControlPointLine.size()-1;
				float fLength = VecLength( m_vtxControlPointLine[nEnd2-1].vtxV[0] - m_vtxControlPointLine[nEnd2-2].vtxV[0] );
				int nNum = (int)(fLength * 0.2f);
				nNum = std::max( nNum, 1 );
				for( int i = 0; i <= nNum; ++i )
				{
					float fS = 1.f-((float)i/nNum);
					F3dVector vecP1[4], vecP2[4];
					for( int n = 0; n < 4; ++n )
					{
						vecP1[n] = m_vtxControlPointLine[nEnd2-n].vtxV[0];
						vecP2[n] = m_vtxControlPointLine[nEnd2-n].vtxV[1];
					}
					VecCatmullRom( VertexLine.vtxV[0], vecP1[0], vecP1[1], vecP1[2], vecP1[3], fS );
					VecCatmullRom( VertexLine.vtxV[1], vecP2[0], vecP2[1], vecP2[2], vecP2[3], fS );
					m_vtxLine.push_back( VertexLine );
				}
			}
		}
	}
	else {
		m_vtxLine.clear();
		VertexLine.dTime = dTime;
		VertexLine.vtxV[0] = vtxV[0];	VertexLine.vtxV[1] = vtxV[1];
		m_vtxControlPointLine.push_back( VertexLine );
	}

	MakeMesh();
}

void F3dCTail::MakeMesh()
{
	if( m_vtxLine.size() < 2 )
		return;

	F3dTexVertexLine vtxLine1, vtxLine2;
	F3dVector vtxT1[2], vtxT2[2];
	std::vector< F3dTexVertexLine > vTempLine;
	int i = 1;
	for(; i < (int)m_vtxLine.size(); ++i )
	{
		vtxLine1.vtxV[0].vtxPoint = m_vtxLine[i-1].vtxV[0];
		vtxLine1.vtxV[1].vtxPoint = m_vtxLine[i-1].vtxV[1];
		vtxLine2.vtxV[0].vtxPoint = m_vtxLine[i].vtxV[0];
		vtxLine2.vtxV[1].vtxPoint = m_vtxLine[i].vtxV[1];
		vTempLine.push_back( vtxLine1 );
		vTempLine.push_back( vtxLine2 );
	}

	vtxLine1.vtxV[0].vtxPoint = m_vtxLine[i-1].vtxV[0];
	vtxLine1.vtxV[1].vtxPoint = m_vtxLine[i-1].vtxV[1];

	int nEnd = (int)m_vtxControlPointLine.size()-1;
	vtxLine2.vtxV[0].vtxPoint = m_vtxControlPointLine[ nEnd ].vtxV[0];
	vtxLine2.vtxV[1].vtxPoint = m_vtxControlPointLine[ nEnd ].vtxV[1];
	vTempLine.push_back( vtxLine1 );
	vTempLine.push_back( vtxLine2 );

	float fTotalLength[2];
	fTotalLength[0] = fTotalLength[1] = 0;
	for( int si = 1; si < (int)vTempLine.size(); ++si )
	{
		fTotalLength[0] += VecLengthSq( vTempLine[si].vtxV[0].vtxPoint - vTempLine[si-1].vtxV[0].vtxPoint );
		fTotalLength[1] += VecLengthSq( vTempLine[si].vtxV[1].vtxPoint - vTempLine[si-1].vtxV[1].vtxPoint );
	}

	if( vTempLine.empty() == false ) {
		vTempLine[0].vtxV[0].fU = vTempLine[0].vtxV[0].fV = 1.f;
		vTempLine[0].vtxV[1].fU = vTempLine[0].vtxV[1].fV = 1.f;
	}
	float fLengthSq[2];
	fLengthSq[0] = fLengthSq[1] = 0;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();
	int nTempLineNum = (int)vTempLine.size() * 2;
	int nMaxVertex = MAX_VERTEX;
	m_nVertexNum = std::min( nTempLineNum, nMaxVertex );
	int nVertexSize = sizeof(F3dTexVertex);

	F3dTexVertex *pVertices;
	HRESULT hResult	= m_pVB->Lock( 0, nVertexSize*m_nVertexNum, (void**)&pVertices, 0 );
	if( hResult != S_OK ) {
		DX_LOGOUT( hResult, L"F3dCTail MakeMesh lock fail." );
		return;
	}

	pVertices[ 0 ].vtxPoint = vTempLine[0].vtxV[0].vtxPoint;
	pVertices[ 0 ].fU = 1;	pVertices[ 0 ].fV = 0;
	pVertices[ 1 ].vtxPoint = vTempLine[0].vtxV[1].vtxPoint;
	pVertices[ 1 ].fU = 1;	pVertices[ 1 ].fV = 1;
	int v = 2;
	for( int si = 1; si < (int)vTempLine.size(); ++si, v+=2 )
	{
		if( v >= MAX_VERTEX )
			break;
		fLengthSq[0] += VecLengthSq( vTempLine[si].vtxV[0].vtxPoint - vTempLine[si-1].vtxV[0].vtxPoint );
		fLengthSq[1] += VecLengthSq( vTempLine[si].vtxV[1].vtxPoint - vTempLine[si-1].vtxV[1].vtxPoint );

		vTempLine[si].vtxV[0].fU = 1.f - ( fLengthSq[0] / fTotalLength[0] );
		vTempLine[si].vtxV[0].fV = 0.f;
		vTempLine[si].vtxV[1].fU = 1.f - ( fLengthSq[1] / fTotalLength[1] );
		vTempLine[si].vtxV[1].fV = 1.f;

		pVertices[ v ].vtxPoint	 = vTempLine[si].vtxV[0].vtxPoint;
		pVertices[ v ].fU		 = vTempLine[si].vtxV[0].fU;
		pVertices[ v ].fV		 = vTempLine[si].vtxV[0].fV;
		pVertices[ v+1 ].vtxPoint= vTempLine[si].vtxV[1].vtxPoint;
		pVertices[ v+1 ].fU		 = vTempLine[si].vtxV[1].fU;
		pVertices[ v+1 ].fV		 = vTempLine[si].vtxV[1].fV;
	}
	m_pVB->Unlock();
}

void F3dCTail::Render( double dTime, int nLayer )
{
	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();
	// Render State

	
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	// Sampler States
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	

	F3dMatrix matWorld;
	Identity(matWorld);
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pD3DDevice->SetStreamSource( 0,GetVertexBuffer(), 0, sizeof(F3dTexVertex));
	pD3DDevice->SetFVF(F3dTexVertex::FVF);
	pD3DDevice->SetTexture( 0, m_pTexture); 

	HRESULT hResult;
	hResult = pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0,GetVertexNum()-2 );

	if( FAILED( hResult ) ) {
		DX_LOGOUT( hResult, L"DrawPrimitive error - " );
	}

	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}

void F3dCTail::Process( double dTime, double dt )
{
	
}

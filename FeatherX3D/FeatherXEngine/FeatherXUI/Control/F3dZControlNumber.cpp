#include "FeatherXEnginePrecompile.h"
 

F3dZControlNumber::F3dZControlNumber() : m_uCipher(0), m_uCount(0), m_fWidth(1.f), m_fDivWidth(1.f), m_pTex(NULL), m_bTimeMode(false), m_bAllowOne(false), m_vCache(NULL)
{ 
	 
}

F3dZControlNumber::~F3dZControlNumber()
{ 
	 
	if( m_vCache ) {
		m_vCache[0].clear();
		delete [] m_vCache;
	}
}

void F3dZControlNumber::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
	 
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( nPieceCount > 0 ) {
			const F3dZUITexture* pTex = pSprite->vTexList[F3dZUISprite::SET_NORMAL][0];
			m_fWidth    = (float)pTex->nWidth;
			m_fDivWidth = pTex->nWidth/10.f;
			m_pTex = pTex;
		}
	}
}

void F3dZControlNumber::SetSize( const int& nWidth, const int& nHeight )
{ 
	 
	F3dZControl::SetSize( nWidth, nHeight );

	int nDivWidth = m_rect.GetWidth()/m_uCipher;

	int l, t, r, b, prevL;
	prevL = 0;

	F3dCUISprite* pSprite = NULL;

	for( USHORT i(0); m_vSprite[0].size()>i; ++i ) {
	
		l = 0 + prevL;
		t = 0;
		r = l + nDivWidth;
		b = t + m_rect.GetHeight();

		prevL = r;

		pSprite = m_vSprite[0][i].get();
		pSprite->rtScreen.left   = l;
		pSprite->rtScreen.top    = t;
		pSprite->rtScreen.right  = r;
		pSprite->rtScreen.bottom = b;

		pSprite->ori_rect = UI_RECT( l, t, r, b );
	}
}

void F3dZControlNumber::SetCipher( USHORT uCipher )
{ 
	 
	if( uCipher <= 0 ) return;

	if( m_uCipher == uCipher ) return;

	m_uCipher = uCipher;

	int nDivWidth = m_rect.GetWidth()/m_uCipher;
	m_nSetCount = 1;
	if( m_vSprite == NULL ) {
		m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

		m_vCache = new std::vector< F3dCUISpritePtr >[m_nSetCount];
		F3dCUISpritePtr pSprite( new F3dCUISprite );
		pSprite->TextureID = m_pTex->TextureID;
		m_vCache[0].push_back( pSprite );
	}
	else {
		m_vSprite[0].clear();
	}

	int l, t, r, b, prevL;
	prevL = 0;
	for( USHORT i(0); m_uCipher>i; ++i ) {
		F3dCUISpritePtr pRenderSprite( new F3dCUISprite );
		pRenderSprite->SetSkipRelease( true );
		pRenderSprite->bBillboard = m_bBillboard;
		pRenderSprite->b3DRender = m_b3DRender;
		pRenderSprite->b3DCursor = m_b3DCursor;

		l = 0 + prevL;
		t = 0;
		r = l + nDivWidth;
		b = t + m_rect.GetHeight();

		prevL = r;

		pRenderSprite->rtScreen.left   = l;
		pRenderSprite->rtScreen.top    = t;
		pRenderSprite->rtScreen.right  = r;
		pRenderSprite->rtScreen.bottom = b;

		pRenderSprite->lu = 0.f;
		pRenderSprite->ru = m_fDivWidth/m_fWidth;

		pRenderSprite->ori_rect = UI_RECT( l, t, r, b );
		pRenderSprite->TextureID = m_pTex->TextureID;

		m_vSprite[0].push_back( pRenderSprite );
	}

	SetPos( m_rect.left, m_rect.top );
}

namespace
{
	const int c_nMaxCipher = 10;
};

void F3dZControlNumber::SetNumber( int nNumber )
{
	 

	if( nNumber < 0 )
		return;

	m_nNumber = nNumber;

	int aNumberTemp[c_nMaxCipher] = {-1,};
	int aNumber[c_nMaxCipher] = {-1,};
	memset( aNumberTemp, -1, sizeof(aNumberTemp) );
	memset( aNumber    , -1, sizeof(aNumber)     );

	char szBuf[11] =  {0,};
	ZeroMemory( szBuf, sizeof(szBuf) );
	_itoa_s( m_nNumber, szBuf, 11, 10 );

	char cTemp[2];
	cTemp[1] = '\0';
	int  nVarCount = 0;
	for( int i(0); m_uCipher>i; ++i ) {
		cTemp[0] = szBuf[i];
		if( cTemp[0] == 0 ) 
			break;
		++nVarCount;
		aNumberTemp[i] = atoi( cTemp );
	}

	for( int i(0); nVarCount>i; ++i ) {
		aNumber[c_nMaxCipher-nVarCount+i] = aNumberTemp[i];
	}

//	LOGOUT( L"1: %d %d %d %d %d %d %d %d %d %d", aNumber[0], aNumber[1], aNumber[2], aNumber[3], aNumber[4], aNumber[5], aNumber[6], aNumber[7], aNumber[8], aNumber[9] );
//	LOGOUT( L"2: %d %d %d %d %d %d %d %d %d %d", aNumberTemp[0], aNumberTemp[1], aNumberTemp[2], aNumberTemp[3], aNumberTemp[4], aNumberTemp[5], aNumberTemp[6], aNumberTemp[7], aNumberTemp[8], aNumberTemp[9] );

	int	 nIndex = 0;
	bool bShow = false;
	if( m_vSprite ) {

		for( UINT i(0); m_vSprite[F3dZUISprite::SET_NORMAL].size()>i; ++i ) {
			F3dCUISprite* pSprite = m_vSprite[F3dZUISprite::SET_NORMAL][i].get();
			
			if( i >= m_uCipher ) {
				pSprite->bShow = false;
				continue;
			}

			nIndex = c_nMaxCipher-m_uCipher+i;

			if( m_bTimeMode ) {
				bShow = true;
			}
			else {
				if( m_uCipher > 1 ) {
					if( i == (m_uCipher-1) ) { //마지막 수
						if( aNumber[nIndex] > 1 || m_bAllowOne )  bShow = true;
					}
					else {
						if( aNumber[nIndex] >= 1 ) bShow = true;
					}
				}
				else
					bShow = true;
			}

			pSprite->bShow = bShow;
			pSprite->lu =  ((float)aNumber[nIndex]*m_fDivWidth)/m_fWidth;
			pSprite->ru = (((float)aNumber[nIndex]*m_fDivWidth)+m_fDivWidth)/m_fWidth;
		}
	}
}

void F3dZControlNumber::Process( double dTime, double dMeanDT )
{ 
	 
	//TODO
}

void F3dZControlNumber::Render( double dTime )
{ 
	 
	if( IsShow() == FALSE ) return;
	if( m_uCipher > 1 )
	{
		if( !m_bTimeMode ) {			
			if( !m_bAllowOne && m_nNumber <= 1 ) return; //갯수 제한
		}
	}

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[F3dZUISprite::SET_NORMAL].size()>i; ++i ) {
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[F3dZUISprite::SET_NORMAL][i].get() );
		}
	}
#ifdef _DEBUG
	renderWire();
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//

F3dZEditableControlNumber::F3dZEditableControlNumber() : m_dTime(0), m_uCount(0), m_fWidth(1.f), m_fDivWidth(1.f)
{ 
	 
}

void F3dZEditableControlNumber::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
	 
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( nPieceCount > 0 ) {
			const F3dZUITexture* pTex = pSprite->vTexList[F3dZUISprite::SET_NORMAL][0];
			m_fWidth    = (float)pTex->nWidth;
			m_fDivWidth = pTex->nWidth/10.f;
		}
		if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
	}
}

void F3dZEditableControlNumber::Process( double dTime, double dMeanDT )
{ 
	 
	if( dTime - m_dTime > 1.f )	{
		m_dTime = dTime;

		if( m_vSprite ) {
			for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ) {
				F3dCUISprite* pSprite = m_vSprite[m_nState][i].get();

				pSprite->lu = (float)(m_uCount*m_fDivWidth)/m_fWidth;
				pSprite->ru = (float)((m_uCount*m_fDivWidth)+m_fDivWidth)/m_fWidth;

				m_uCount++;
				if( m_uCount > 9 ) m_uCount = 0;
			}
		}
	}
}

void F3dZEditableControlNumber::Render( double dTime )
{ 
	 
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ) {
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}

	renderWire();
}

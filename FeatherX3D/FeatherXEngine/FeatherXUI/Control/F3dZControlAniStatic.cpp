#include "FeatherXEnginePrecompile.h"
 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlAniStatic::F3dZControlAniStatic() : 
m_fStartTime(0.f),
m_fChangeTime(0.2f),
m_bSkipNext(false),
m_bOneTimeRender(false),
m_bAutoNextFrame(true),
m_bLoop(true)
{ 
	 
}

F3dZControlAniStatic::~F3dZControlAniStatic()
{ 
	 

}

void F3dZControlAniStatic::SetChangeTime( const double fValue )
{
	 
	m_fChangeTime = fValue;
}

void F3dZControlAniStatic::SetSkipNextFrame( bool bFlag )
{
	 
	m_bSkipNext = bFlag;
}

void F3dZControlAniStatic::SetLoop( bool bFlag )
{
	m_bLoop = bFlag;
}

void F3dZControlAniStatic::SetOneTimeRender( bool bFlag )
{
	 
	m_bOneTimeRender = bFlag;
}

const   bool  F3dZControlAniStatic::IsEnd() const
{
	if( m_bLoop )
		return false;

	return (m_nState == (m_nSetCount-1));
}

void F3dZControlAniStatic::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
	 
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}
}

const int  F3dZControlAniStatic::getNextFrame()
{  
	int nNext = m_nState + 1;
	if( nNext >= m_nSetCount ) nNext = 0;

	return nNext;
}

void F3dZControlAniStatic::SetFirst()
{
	m_nState = 0;	
}

void F3dZControlAniStatic::setNextFrame()
{ 
	 
	if( m_bEnable == FALSE ) {
		m_nState = 0;	
		return;
	}

	if( m_bStateChange )
		return;

	m_nState++;
	if( m_bLoop ) {
		if( m_nState >= m_nSetCount ) {
			m_nState = 0;
			if( m_bOneTimeRender ) {
				SetShow( FALSE );
			}
		}
	}
	else {
		if( m_nState >= m_nSetCount )
			m_nState = m_nSetCount-1;
	}
}

int F3dZControlAniStatic::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{ 
	 
	if( m_nSetCount <= 0  ) return _RET_CTL_NONE;
	if( IsShow() == FALSE ) return _RET_CTL_NONE;
	if(	!m_bEnable )		return _RET_CTL_NONE;

	return F3dZControl::OnMouseMsg( uMsg, x, y, nPrevRetValue );
}

void F3dZControlAniStatic::Process( double dTime, double dMeanDT )
{ 
	 
	if( m_nSetCount <= 0 ) return;

	if( m_fStartTime == 0.f ) m_fStartTime = dTime;

	double fValue = dTime - m_fStartTime;

	if( fValue >= m_fChangeTime ) {
		setNextFrame();
		m_fStartTime = dTime;
	}

	if( m_bEnable == FALSE ) return;

	if( m_bSkipNext )
	{
		if( m_vSprite ) {
			for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ) {
				F3dCUISpritePtr pSprite = m_vSprite[m_nState][i];
				pSprite->SetColor( 255, 255, 255, 255 );
			}
		}
	}
	else
	{
		int nNext = getNextFrame();
		fValue = fValue/m_fChangeTime;

		if( m_vSprite ) {
			for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ) {
				F3dCUISpritePtr pSprite = m_vSprite[m_nState][i];
				pSprite->SetColor( 255, 255, 255, (int)(fValue*255.f) );
			}

			for( UINT i(0); m_vSprite[nNext].size()>i; ++i ) {
				F3dCUISpritePtr pSprite = m_vSprite[nNext][i];
				pSprite->SetColor( 255, 255, 255, (int)((1.f-fValue)*255.f) );
			}
		}
	}
}

void F3dZControlAniStatic::RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer )
{
	 
	if( m_nSetCount <= 0 ) return;

	//블랜딩 타임을 갖고 있으며, 2장이 서로 섞임
	int nNext = getNextFrame();

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )	{
			RenderLayer.RegUISprite( m_vSprite[m_nState][i].get(), nLayer );
		}

		if( m_bSkipNext ) return;
		if( m_bEnable == FALSE ) return;

		for( UINT i(0); m_vSprite[nNext].size()>i; ++i ) {
			RenderLayer.RegUISprite( m_vSprite[nNext][i].get(), nLayer );
		}
	}
}

void F3dZControlAniStatic::Render( double dTime )
{ 
	 
	if( m_nSetCount <= 0 ) return;

	//블랜딩 타임을 갖고 있으며, 2장이 서로 섞임
	int nNext = getNextFrame();

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )	{
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}

		if( m_bSkipNext ) return;
		if( m_bEnable == FALSE ) return;

		for( UINT i(0); m_vSprite[nNext].size()>i; ++i ) {
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[nNext][i].get() );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlAniDelayStatic::F3dZControlAniDelayStatic() : m_nCurIndex(0), m_nFrameCount(0), m_bLoop(true), m_nPlayStartIndex(0), m_nPlayEndIndex(0), m_nTextureWidth(0), m_nTextureHeight(0)
{																																					
	 
}

F3dZControlAniDelayStatic::~F3dZControlAniDelayStatic()
{
	 
}

const int F3dZControlAniDelayStatic::GetTextureWidth() const 
{
	return m_nTextureWidth;
}

const int F3dZControlAniDelayStatic::GetTextureHeight() const 
{
	return m_nTextureHeight;
}

void F3dZControlAniDelayStatic::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	 
	F3dZControl::CreateControl( pSprite, control_arg );

	m_nTextureWidth  = control_arg.nWidth;
	m_nTextureHeight = control_arg.nHeight;
}

void F3dZControlAniDelayStatic::Process( double dTime, double dMeanDT )
{
	 
	if( IsShow() == FALSE ) return;

	if( m_vDelay.empty() ) return;

	++m_nFrameCount;
	if( m_vDelay[m_nCurIndex] < m_nFrameCount ) {
		m_nFrameCount = 0;
		++m_nCurIndex;

		if( m_nCurIndex > m_nPlayEndIndex ) {
			if( IsLoop() )
				m_nCurIndex = m_nPlayStartIndex;
			else
				m_nCurIndex = m_nPlayEndIndex;
		}
	}
}

void F3dZControlAniDelayStatic::Render( double dTime )
{
	 
	if( IsShow() == FALSE ) return;

	if( m_vSprite ) {
		F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nCurIndex][0].get() );
	}
}

void F3dZControlAniDelayStatic::RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer )
{
	 
	if( IsShow() == FALSE ) return;

	RenderLayer.RegUISprite( m_vSprite[m_nCurIndex][0].get(), nLayer );
}

void F3dZControlAniDelayStatic::SetPlayIndex( int nStartIndex, int nEndIndex )
{
	 

	m_nPlayStartIndex = nStartIndex;

	if( m_nPlayStartIndex < 0 )
		m_nPlayStartIndex = 0;
	if( !m_vDelay.empty() && m_nPlayStartIndex >= m_nSetCount )
		m_nPlayStartIndex = m_nSetCount-1;

	m_nPlayEndIndex   = nEndIndex;

	if( m_nPlayEndIndex < 0 )
		m_nPlayEndIndex = 0;
	if( !m_vDelay.empty() && m_nPlayEndIndex >= m_nSetCount )
		m_nPlayEndIndex = m_nSetCount-1;

	m_nFrameCount = 0;
	m_nCurIndex   = m_nPlayStartIndex;
}

void F3dZControlAniDelayStatic::ChangeFrame( std::vector<const F3dZUISprite*>& vUISpriteList, std::vector<int>& vDelayList )
{
	 
	clearSprite();
	m_vDelay.clear();

	AddFrame( vUISpriteList, vDelayList );

	m_nFrameCount = 0;
	m_nCurIndex   = 0;
}

void F3dZControlAniDelayStatic::AddFrame( std::vector<const F3dZUISprite*>& vUISpriteList, std::vector<int>& vDelayList )
{
	 

	_MBASSERT( vUISpriteList.size() == vDelayList.size() );

	m_vDelay = vDelayList;
	m_vSprite = new std::vector< F3dCUISpritePtr >[vUISpriteList.size()];
	m_nSetCount = (short)vUISpriteList.size();

	m_nPlayStartIndex = 0;
	m_nPlayEndIndex   = m_nSetCount-1;

	for( UINT i(0); vUISpriteList.size()>i; ++i ) 
	{
		if( !vUISpriteList[i]->vTexList[0].empty() )
		{
			int l, t, r, b;
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			l = 0;
			t = 0;
			r = l + vUISpriteList[i]->vTexList[0][0]->nWidth;
			b = t + vUISpriteList[i]->vTexList[0][0]->nHeight;

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );
			pRenderSprite->TextureID   = vUISpriteList[i]->vTexList[0][0]->TextureID;

			m_vSprite[i].push_back( pRenderSprite );
		}
	}
}

int	 F3dZControlAniDelayStatic::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{
	 
	if( m_nSetCount <= 0  ) return _RET_CTL_NONE;
	if( IsShow() == FALSE ) return _RET_CTL_NONE;
	if(	!m_bEnable )		return _RET_CTL_NONE;

	return F3dZControl::OnMouseMsg( uMsg, x, y, nPrevRetValue );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void F3dZEditableControlAniStatic::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}
}

void F3dZEditableControlAniStatic::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	if( m_nSetCount <= 0 ) return;

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )	{
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}
}

#include "FeatherXEnginePrecompile.h"

namespace
{
	const short c_nDefSliderPieceCount = 6; //BG 3개, 버튼 3개
	const short c_nDefBackGroundPieceCount = 3;
	const short c_nDefSetCount = 3;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//SliderBase
F3dZControlSliderBase::F3dZControlSliderBase()
: m_nMinPos(0), m_nMaxPos(100), m_bMouseDrag(FALSE), m_nCurrentPos(0)
{  
	m_pCtrl[_BACK]  = NULL;
	m_pCtrl[_THUMB] = NULL;
}

F3dZControlSliderBase::~F3dZControlSliderBase()
{  

}

void F3dZControlSliderBase::SetRange( int nMinPos, int nMaxPos )
{  
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;
}

void F3dZControlSliderBase::SetPos( int nPos )
{  
	m_nCurrentPos = nPos;

	_EvaluationThumbRect();
}

int F3dZControlSliderBase::GetPos( int& nPos )
{  
	nPos = m_nCurrentPos;
	return m_nCurrentPos;
}

void F3dZControlSliderBase::GetRange( int& nMinPos, int& nMaxPos )
{  
	nMinPos = m_nMinPos;
	nMaxPos = m_nMaxPos;
}

void F3dZControlSliderBase::onEnable( const BOOL bFlag )
{
	if( m_pCtrl[_THUMB] )
		m_pCtrl[_THUMB]->SetShow( bFlag );
}

int	F3dZControlSliderBase::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{ 
	 
	int nRetValue = _RET_CTL_NONE;

	if(	!m_bEnable ) return _RET_CTL_NONE;

	if( !m_pCtrl[_THUMB] ) return nRetValue;

	// 여기에 처리
	int nOldPos = m_nCurrentPos;

	// Drag 처리
	if( m_bMouseDrag ) {
		switch( uMsg )  {
		case WM_MOUSEMOVE:
			_SetThumbRect(x,y);		
			_EvaluationPositionFromThumRect();
			break;
		case WM_LBUTTONUP:
			_EvaluationThumbRect();
			m_bMouseDrag = FALSE;
			break;
		default:
			break;
		}

		nRetValue = _RET_CTL_USED;
	}
	// Scroll 이동범위 체크 
	else if( m_rect.IsInRect(x,y) ) {	

		switch(uMsg) {
		case WM_LBUTTONDOWN:
			if(m_pCtrl[_THUMB]->GetRect().IsInRect(x,y) ) {
				m_ptMovingOffset.x = x - m_pCtrl[_THUMB]->GetRect().left;
				m_ptMovingOffset.y = y - m_pCtrl[_THUMB]->GetRect().top;
				m_bMouseDrag = TRUE;
				//	nRetValue |= _RET_CTL_USED;
			}
			else {
				m_ptMovingOffset.x = m_pCtrl[_THUMB]->GetRect().GetWidth()  / 2;
				m_ptMovingOffset.y = m_pCtrl[_THUMB]->GetRect().GetHeight() / 2;
				// Thumb의 포지션을 옮긴다. 
				_EvaluationPosition(x,y);
				_EvaluationThumbRect();
			}
			break;
		case WM_MBUTTONDOWN: {
			if(m_nCurrentPos < (int)m_nMaxPos - 1)	{
				m_nCurrentPos++;
				_EvaluationThumbRect();
			}
							 }
							 break;
		case WM_MBUTTONUP:
			_EvaluationThumbRect();
			break;

		default:
			break;
		}		
		

		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}
	else {
		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}

	if(nOldPos != m_nCurrentPos )
	{
		OnControlNotify( SLIDER_CHANGE, GetID(), nOldPos      , 0);
		OnControlNotify( SLIDER_SELECT, GetID(), m_nCurrentPos, 0);	
	}

	return nRetValue;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VSlider
F3dZControlVSlider::F3dZControlVSlider()
{  

}

F3dZControlVSlider::~F3dZControlVSlider()
{  

}

void F3dZControlVSlider::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefSliderPieceCount ); 

	createBackGround( pSprite, control_arg );
	createButtonThumb( pSprite, control_arg );
}

void F3dZControlVSlider::createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nBGWidth=0;
	for( short i(0); c_nDefBackGroundPieceCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][i]->TextureID );
			nBGWidth = pTexture->GetWidth();
		}
		texlist.push_back( pSprite->vTexList[0][i] );
	}

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_BG", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x+control_arg.nWidth/2-nBGWidth/2;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = control_arg.nWidth;
		child_control_arg.nHeight = control_arg.nHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createStatic();
		if( pControl ) {
			pControl->SetVertical( TRUE );
			pControl->SetParent( GetParent() );
			pControl->AddFlag( _MSG_SKIP_FLAG );
			pControl->createCustomControl_Vertical_3( 1, texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pCtrl[_BACK]  = pControl;
		}
	}
}

void F3dZControlVSlider::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 3;
	int nThumbWidth, nThumbHeight;
	nThumbWidth = nThumbHeight = 0;
	for( short i(0); c_nDefSetCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][nStartIdx+i]->TextureID );
			nThumbWidth  = pTexture->GetWidth();
			nThumbHeight = pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x + m_rect.GetWidth()/2-nThumbWidth/2;
	child_control_arg.y       = control_arg.y + m_rect.GetHeight()/2-nThumbHeight/2;
	child_control_arg.nWidth  = nThumbWidth;
	child_control_arg.nHeight = nThumbHeight;

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_1( c_nDefSetCount, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );

		m_pCtrl[_THUMB] = pControl;
	}
}

void F3dZControlVSlider::Process( double dTime, double dMeanDT )
{  
	F3dZControl::Process( dTime, dMeanDT );
}

void F3dZControlVSlider::Render( double dTime )
{  
	if( IsShow() == FALSE ) return;

	renderChild( dTime );
}

void F3dZControlVSlider::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT bg_rect, thumb_rect;

	//BackGround
	if( m_pCtrl[_BACK] ) {
		m_pCtrl[_BACK]->GetRect( bg_rect );
		m_pCtrl[_BACK]->SetSize( bg_rect.GetWidth(), m_rect.GetHeight() );
		m_pCtrl[_BACK]->SetPos( m_rect.left+m_rect.GetWidth()/2-bg_rect.GetWidth()/2, m_rect.top );
	}

	//Thumb
	if( m_pCtrl[_THUMB] ) {
		m_pCtrl[_THUMB]->GetRect( thumb_rect );
		m_pCtrl[_THUMB]->SetPos( m_rect.left + m_rect.GetWidth()/2-thumb_rect.GetWidth()/2, m_rect.top+m_rect.GetHeight()/2-thumb_rect.GetHeight()/2 );
	}
}

void F3dZControlVSlider::_EvaluationPosition(int x, int y)
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nHeight  = m_rect.GetHeight();
	int nDiffThumbHeight = nHeight - ( (m_pCtrl[_THUMB]->GetRect().GetHeight()+1) / 2 );

	// 위에서부터 이동된 거리
	int nHeightDiff = y - m_rect.top;

	if(nHeightDiff < 0)
		nHeightDiff = 0;

	float fPosRatio = (float)nHeightDiff / (float)nDiffThumbHeight;

	if(fPosRatio > 1.0f)
		fPosRatio = 1.0f;

	m_nCurrentPos = (int)(m_nMaxPos* fPosRatio);
}

void F3dZControlVSlider::_EvaluationPositionFromThumRect()
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nHeight  = m_rect.GetHeight();
	int dwHeightDiff = nHeight - m_pCtrl[_THUMB]->GetRect().GetHeight();

	float fPosRatio = 0.f;

	if( dwHeightDiff > 0)
		fPosRatio = (float) (m_pCtrl[_THUMB]->GetRect().top - m_rect.top) / (float)dwHeightDiff;

	if( m_nMaxPos > 0)
		fPosRatio += 1 / (float)m_nMaxPos / 2.0f;

	m_nCurrentPos = (int)(m_nMaxPos * fPosRatio);
}

void F3dZControlVSlider::_EvaluationThumbRect()
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nHeight  = m_rect.GetHeight();
	int nThumbHeight = m_pCtrl[_THUMB]->GetRect().GetHeight();

	float fRatioPos = 0.0f;

	if( m_nMaxPos > 0 )
		fRatioPos = (float)m_nCurrentPos/m_nMaxPos;

	int dwAddHeight = (int)((nHeight - nThumbHeight) * fRatioPos);

	m_pCtrl[_THUMB]->SetPos( m_pCtrl[_THUMB]->GetRect().left, m_rect.top + dwAddHeight );
}

void F3dZControlVSlider::_SetThumbRect(int x, int y)
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nMaxPosY = m_rect.bottom - m_pCtrl[_THUMB]->GetRect().GetHeight();

	y -= m_ptMovingOffset.y;
	y = std::max( y, m_rect.top );
	y = std::min( y, nMaxPosY );

	m_pCtrl[_THUMB]->MovePos( m_rect.left, y );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//HSlider
F3dZControlHSlider::F3dZControlHSlider()
{  

}

F3dZControlHSlider::~F3dZControlHSlider()
{  

}

void F3dZControlHSlider::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefSliderPieceCount ); 

	createBackGround( pSprite, control_arg );
	createButtonThumb( pSprite, control_arg );
}

void F3dZControlHSlider::createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nBGHeight = 0;
	for( short i(0); c_nDefBackGroundPieceCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][i]->TextureID );
			nBGHeight = pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i] );
	}

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_BG", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y+control_arg.nHeight/2-nBGHeight/2;
		child_control_arg.nWidth  = control_arg.nWidth;
		child_control_arg.nHeight = nBGHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createStatic();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->AddFlag( _MSG_SKIP_FLAG );
			pControl->createCustomControl_3( 1, texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pCtrl[_BACK]  = pControl;
		}
	}
}

void F3dZControlHSlider::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 3;
	int nThumbWidth, nThumbHeight;
	nThumbWidth = nThumbHeight = 0;
	for( short i(0); c_nDefSetCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][nStartIdx+i]->TextureID );
			nThumbWidth  = pTexture->GetWidth();
			nThumbHeight = pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x + m_rect.GetWidth()/2-nThumbWidth/2;
	child_control_arg.y       = control_arg.y + m_rect.GetHeight()/2-nThumbHeight/2;
	child_control_arg.nWidth  = nThumbWidth;
	child_control_arg.nHeight = nThumbHeight;

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_1( c_nDefSetCount, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );

		m_pCtrl[_THUMB] = pControl;
	}
}

void F3dZControlHSlider::Process( double dTime, double dMeanDT )
{  
	F3dZControl::Process( dTime, dMeanDT );
}

void F3dZControlHSlider::Render( double dTime )
{  
	if( IsShow() == FALSE ) return;

	renderChild( dTime );
}

void F3dZControlHSlider::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT bg_rect, thumb_rect;

	//BackGround
	if( m_pCtrl[_BACK] ) {
		m_pCtrl[_BACK]->GetRect( bg_rect );
		m_pCtrl[_BACK]->SetSize( m_rect.GetWidth(), bg_rect.GetHeight() );
		m_pCtrl[_BACK]->SetPos( m_rect.left, m_rect.top+m_rect.GetHeight()/2-bg_rect.GetHeight()/2 );
	}

	//Thumb
	if( m_pCtrl[_THUMB] ) {
		m_pCtrl[_THUMB]->GetRect( thumb_rect );
		m_pCtrl[_THUMB]->SetPos( m_rect.left+m_rect.GetWidth()/2-thumb_rect.GetWidth()/2, m_rect.top+m_rect.GetHeight()/2-thumb_rect.GetHeight()/2 );
	}
}

void F3dZControlHSlider::_EvaluationPosition(int x, int y)
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nWidth          = m_rect.GetWidth();
	int nDiffThumbWidth = nWidth - ( (m_pCtrl[_THUMB]->GetRect().GetWidth()+1) / 2 );

	// 위에서부터 이동된 거리
	int nWidthDiff = x - m_rect.left;

	if(nWidthDiff < 0)
		nWidthDiff = 0;

	float fPosRatio = (float)nWidthDiff / (float)nDiffThumbWidth;

	if(fPosRatio > 1.0f)
		fPosRatio = 1.0f;

	m_nCurrentPos = (int)(m_nMaxPos* fPosRatio);
}

void F3dZControlHSlider::_EvaluationPositionFromThumRect()
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nWidth      = m_rect.GetWidth();
	int dwWidthDiff = nWidth - m_pCtrl[_THUMB]->GetRect().GetWidth();

	float fPosRatio = 0.f;

	if( dwWidthDiff > 0)
		fPosRatio = (float) (m_pCtrl[_THUMB]->GetRect().left - m_rect.left) / (float)dwWidthDiff;

	if( m_nMaxPos > 0)
		fPosRatio += 1 / (float)m_nMaxPos / 2.0f;

	m_nCurrentPos = (int)(m_nMaxPos * fPosRatio);
}

void F3dZControlHSlider::_EvaluationThumbRect()
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nWidth      = m_rect.GetWidth();
	int nThumbWidth = m_pCtrl[_THUMB]->GetRect().GetWidth();

	float fRatioPos = 0.0f;

	if( m_nMaxPos > 0 )
		fRatioPos = (float)m_nCurrentPos/m_nMaxPos;

	int dwAddWidth = (int)((nWidth - nThumbWidth) * fRatioPos);

	if( m_rect.right < m_rect.left+dwAddWidth )
	{
		assert(0);
	}

	m_pCtrl[_THUMB]->SetPos( m_rect.left+dwAddWidth, m_pCtrl[_THUMB]->GetRect().top );
}

void F3dZControlHSlider::_SetThumbRect(int x, int y)
{  
	if( !m_pCtrl[_THUMB] ) return;

	int nMaxPosX = m_rect.right - m_pCtrl[_THUMB]->GetRect().GetWidth();

	x -= m_ptMovingOffset.x;
	x = std::max( x, m_rect.left );
	x = std::min( x, nMaxPosX );

	m_pCtrl[_THUMB]->MovePos( x, m_rect.top );
}

//-----------------------------------------------------------


void F3dZEditableControlVSlider::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefSliderPieceCount ); 

	createBackGround( pSprite, control_arg );
	createButtonThumb( pSprite, control_arg );
}

void F3dZEditableControlVSlider::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	renderChild( dTime );
}

void F3dZEditableControlVSlider::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT bg_rect, mid_rect;

	std::wstring strID;
	//BackGround
	MBStringUtil::StringFormat( strID, L"%s_BG", GetID() );
	F3dZEditableControl* pBG = GetChildControl( strID.c_str() );
	if( pBG ) {
		pBG->GetRect( bg_rect );
		pBG->SetSize( bg_rect.GetWidth(), m_rect.GetHeight() );
		pBG->SetPos( m_rect.left+m_rect.GetWidth()/2-bg_rect.GetWidth()/2, m_rect.top );
	}

	//Mid
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );
	F3dZEditableControl* pMid = GetChildControl( strID.c_str() );
	if( pMid ) {
		pMid->GetRect( mid_rect );
		pMid->SetPos( m_rect.left, m_rect.top+m_rect.GetHeight()/2-mid_rect.GetHeight()/2 );
	}
}

void F3dZEditableControlVSlider::createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nBGWidth=0;
	for( short i(0); c_nDefBackGroundPieceCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][i]->TextureID );
			nBGWidth = pTexture->GetWidth();
		}
		texlist.push_back( pSprite->vTexList[0][i] );
	}

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_BG", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x+control_arg.nWidth/2-nBGWidth/2;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = nBGWidth;
		child_control_arg.nHeight = control_arg.nHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createStatic();
		if( pControl ) {
			pControl->SetVertical( TRUE );
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_Vertical_3( 1, texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );
		}
	}
}

void F3dZEditableControlVSlider::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 3;
	int nThumbWidth, nThumbHeight;
	nThumbWidth = nThumbHeight = 0;
	for( short i(0); c_nDefSetCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][nStartIdx+i]->TextureID );
			nThumbWidth  = pTexture->GetWidth();
			nThumbHeight = pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x + m_rect.GetWidth()/2-nThumbWidth/2;
	child_control_arg.y       = control_arg.y + m_rect.GetHeight()/2-nThumbHeight/2;
	child_control_arg.nWidth  = nThumbWidth;
	child_control_arg.nHeight = nThumbHeight;

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_1( c_nDefSetCount, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );
	}
}

int	F3dZEditableControlVSlider::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{  
	int nRetValue = _RET_CTL_NONE;

	if( m_nState == F3dZUISprite::SET_DISABLE )
		return nRetValue;

	//Mouse Move
	if( uMsg == WM_MOUSEMOVE )
	{
		if( m_bIsMoveAble )
		{
			int XOffset = x - m_ptMovingOffset.x;
			int YOffset = y - m_ptMovingOffset.y;

			if(XOffset >= 0 || YOffset >= 0)
			{
				int offsetx = (XOffset - m_rect.left)/s_nGrid;
				int offsety = (YOffset - m_rect.top )/s_nGrid;
				MovePos( offsetx*s_nGrid, offsety*s_nGrid );
			}
			nRetValue = _RET_CTL_USED;
			return nRetValue;
		}
	}

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
	{
		switch( uMsg )
		{
		case WM_LBUTTONDOWN :
			{
				if( GetParent()->SetCaptureControl(this) )
				{
					SetSelect( TRUE );
					setState( F3dZUISprite::SET_DOWN );
					m_bLeftButton = TRUE;
					m_bIsMoveAble = TRUE;
				}

				m_ptMovingOffset.x = x - m_rect.left;
				m_ptMovingOffset.y = y - m_rect.top;
			}
			break;
		case WM_LBUTTONUP: {
			if( m_bLeftButton )			{
				OnControlNotify( LBUTTON_CLICK, GetID() );
				if( GetParent() )
					GetParent()->ReleaseCaptureControl(this);
				m_bIsMoveAble = FALSE;
			}
			m_bLeftButton = FALSE;
						   }
						   break;
		}

		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}
	else
	{
		SetSelect( FALSE );
		if( uMsg == WM_LBUTTONUP )
		{
			if( m_bLeftButton && GetParent() )
				GetParent()->ReleaseCaptureControl(this);
			m_bLeftButton = FALSE;
		}
		else if( uMsg == WM_RBUTTONUP )
			m_bRightButton = FALSE;
		else if( uMsg == WM_MBUTTONUP )
			m_bMiddleButton = FALSE;

		m_bIsMoveAble = FALSE;

		for( UINT i(0); m_vChildList.size()>i; ++i )	{
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
		}
	}

	return nRetValue;
}

//////////////////////////////////////////////////////////////////////////
void F3dZEditableControlHSlider::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefSliderPieceCount ); 

	createBackGround( pSprite, control_arg );
	createButtonThumb( pSprite, control_arg );
}

void F3dZEditableControlHSlider::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	renderChild( dTime );
}

void F3dZEditableControlHSlider::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT bg_rect, mid_rect;

	std::wstring strID;
	//BackGround
	MBStringUtil::StringFormat( strID, L"%s_BG", GetID() );
	F3dZEditableControl* pBG = GetChildControl( strID.c_str() );
	if( pBG ) {
		pBG->GetRect( bg_rect );
		pBG->SetSize( m_rect.GetWidth(), bg_rect.GetHeight() );
		pBG->SetPos( m_rect.left, m_rect.top+m_rect.GetHeight()/2-bg_rect.GetHeight()/2 );
	}

	//Mid
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );
	F3dZEditableControl* pMid = GetChildControl( strID.c_str() );
	if( pMid ) {
		pMid->GetRect( mid_rect );
		pMid->SetPos( m_rect.left+m_rect.GetWidth()/2-mid_rect.GetWidth()/2, m_rect.top );
	}
}

void F3dZEditableControlHSlider::createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nBGHeight = 0;
	for( short i(0); c_nDefBackGroundPieceCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][i]->TextureID );
			nBGHeight = pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i] );
	}

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_BG", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y+control_arg.nHeight/2-nBGHeight/2;
		child_control_arg.nWidth  = control_arg.nWidth;
		child_control_arg.nHeight = nBGHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createStatic();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_3( 1, texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );
		}
	}
}

void F3dZEditableControlHSlider::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 3;
	int nThumbWidth, nThumbHeight;
	nThumbWidth = nThumbHeight = 0;
	for( short i(0); c_nDefSetCount>i; ++i ) {
		if( i == 0 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][nStartIdx+i]->TextureID );
			nThumbWidth  = pTexture->GetWidth();
			nThumbHeight = pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x + m_rect.GetWidth()/2-nThumbWidth/2;
	child_control_arg.y       = control_arg.y + m_rect.GetHeight()/2-nThumbHeight/2;
	child_control_arg.nWidth  = nThumbWidth;
	child_control_arg.nHeight = nThumbHeight;

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_1( c_nDefSetCount, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );
	}
}


int	F3dZEditableControlHSlider::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{  
	int nRetValue = _RET_CTL_NONE;

	if( m_nState == F3dZUISprite::SET_DISABLE )
		return nRetValue;

	//Mouse Move
	if( uMsg == WM_MOUSEMOVE )
	{
		if( m_bIsMoveAble )
		{
			int XOffset = x - m_ptMovingOffset.x;
			int YOffset = y - m_ptMovingOffset.y;

			if(XOffset >= 0 || YOffset >= 0)
			{
				int offsetx = (XOffset - m_rect.left)/s_nGrid;
				int offsety = (YOffset - m_rect.top )/s_nGrid;
				MovePos( offsetx*s_nGrid, offsety*s_nGrid );
			}
			nRetValue = _RET_CTL_USED;
			return nRetValue;
		}
	}

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
	{
		switch( uMsg )
		{
		case WM_LBUTTONDOWN :
			{
				if( GetParent()->SetCaptureControl(this) )
				{
					SetSelect( TRUE );
					setState( F3dZUISprite::SET_DOWN );
					m_bLeftButton = TRUE;
					m_bIsMoveAble = TRUE;
				}

				m_ptMovingOffset.x = x - m_rect.left;
				m_ptMovingOffset.y = y - m_rect.top;
			}
			break;
		case WM_LBUTTONUP: {
			if( m_bLeftButton )			{
				OnControlNotify( LBUTTON_CLICK, GetID() );
				if( GetParent() )
					GetParent()->ReleaseCaptureControl(this);
				m_bIsMoveAble = FALSE;
			}
			m_bLeftButton = FALSE;
						   }
						   break;
		}

		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}
	else
	{
		SetSelect( FALSE );
		if( uMsg == WM_LBUTTONUP )
		{
			if( m_bLeftButton && GetParent() )
				GetParent()->ReleaseCaptureControl(this);
			m_bLeftButton = FALSE;
		}
		else if( uMsg == WM_RBUTTONUP )
			m_bRightButton = FALSE;
		else if( uMsg == WM_MBUTTONUP )
			m_bMiddleButton = FALSE;

		m_bIsMoveAble = FALSE;

		for( UINT i(0); m_vChildList.size()>i; ++i )	{
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
		}
	}

	return nRetValue;
}
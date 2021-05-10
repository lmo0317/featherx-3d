
#include "FeatherXEnginePrecompile.h"
#include "MBStringUtill.h"

using namespace MBStringUtil;

namespace
{
	const short c_nDefScrollPieceCount = 17; //위, 아래 버튼 1조각씩 상태 4개, 가운데 버튼 3조각씩 상태 3개( 4+4+9 )
	const short c_nDefSetCount = 4;
	const short c_nDefMidCount = 9;
}

//Base
F3dZControlScrollBase::F3dZControlScrollBase()
: m_nMinPos(0), m_nMaxPos(0), m_bMouseDrag(FALSE), m_nCurrentPos(0)
{  
	m_pCtrl[_TOP] = NULL;
	m_pCtrl[_BOTTOM] = NULL;
	m_pCtrl[_THUMB] = NULL;
}
F3dZControlScrollBase::~F3dZControlScrollBase()
{  

}

void F3dZControlScrollBase::SetScrollRange( int nMinPos, int nMaxPos )
{  
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;

	_ResizeThumb();
}
void F3dZControlScrollBase::SetScrollPos( int nPos )
{  
	m_nCurrentPos = nPos;

	_EvaluationThumbRect();
}

void F3dZControlScrollBase::GetScrollRange( int& nMinPos, int& nMaxPos )
{  
	nMinPos = m_nMinPos;
	nMaxPos = m_nMaxPos;
}
int F3dZControlScrollBase::GetScrollPos( int& nPos )
{  
	nPos = m_nCurrentPos;
	return m_nCurrentPos;
}

int F3dZControlScrollBase::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
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
		UI_RECT mid_rect = m_pCtrl[_TOP]->GetRect();
		mid_rect.top     = m_pCtrl[_TOP]->GetRect().bottom;
		mid_rect.bottom  = m_pCtrl[_BOTTOM]->GetRect().top;

		if( mid_rect.IsInRect(x,y) ) {
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
			case WM_MOUSEWHEEL: {
					//if(m_nCurrentPos > 0)
					//{
					//	m_nCurrentPos--;
					//	_EvalThumbRect();
					//}
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
		}

		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}
	else {
		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}
	//else if( GetParent()->IsInRect( x, y ) )
	//{
	//	switch(dwMessage) 
	//	{
	//	case KWHEEL_UP:
	//		{
	//			if(m_nCurrentPos > 0)
	//			{
	//				m_nCurrentPos--;
	//				_EvalThumbRect();
	//			}
	//		}
	//		break;
	//	case KWHEEL_DOWN:
	//		{
	//			if(m_nCurrentPos < (int)m_dwMaxRange - 1)
	//			{
	//				m_nCurrentPos++;
	//				_EvalThumbRect();
	//			}
	//		}
	//		break;
	//	}
	//}

	if(nOldPos != m_nCurrentPos )
	{
		OnControlNotify( SCROLL_CHANGE, GetID(), nOldPos      , 0);
		OnControlNotify( SCROLL_SELECT, GetID(), m_nCurrentPos, 0);	
	}

	return nRetValue;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VScroll
F3dZControlVScroll::F3dZControlVScroll()
{  
	
}

F3dZControlVScroll::~F3dZControlVScroll()
{  

}

void F3dZControlVScroll::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );
	//TODO

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefScrollPieceCount ); 
	createButtonTop   ( pSprite, control_arg );
	createButtonBottom( pSprite, control_arg );
	createButtonThumb ( pSprite, control_arg );
}

void F3dZControlVScroll::Process( double dTime, double dMeanDT )
{  
	//TODO
	F3dZControl::Process( dTime, dMeanDT );
}

void F3dZControlVScroll::Render( double dTime )
{  
	if( IsShow() == FALSE ) return;

	renderChild( dTime );
}

void F3dZControlVScroll::createButtonTop   ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_TopBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pCtrl[_TOP] = pControl;
		}
	}
}

void F3dZControlVScroll::createButtonThumb   ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 8;
	m_nMinThumbWidth = 1;
	for( short i(0); c_nDefMidCount>i; ++i ) {
		if( i == 0 || i == 2 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][nStartIdx+i]->TextureID );
			if( pTexture )
				m_nMinThumbWidth += pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y + m_pCtrl[_TOP]->GetRect().GetHeight();
	child_control_arg.nWidth  = m_pCtrl[_TOP]->GetRect().GetWidth();
	child_control_arg.nHeight = control_arg.nHeight - m_pCtrl[_TOP]->GetRect().GetHeight()-m_pCtrl[_BOTTOM]->GetRect().GetHeight();

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetVertical( TRUE );
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_Vertical_3( c_nDefMidCount/3, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
//		pControl->SetFlag( _NONE_FLAG );
		addChildControl( pControl );

		m_pCtrl[_THUMB] = pControl;
	}
}

void F3dZControlVScroll::createButtonBottom( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 4;
	for( short i(0); c_nDefSetCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][nStartIdx+i] );
	}

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_BottomBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y + control_arg.nHeight - pTex->nHeight;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pCtrl[_BOTTOM] = pControl;
		}
	}
}

//int	 F3dZControlVScroll::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
//{
//	int nRetValue = _RET_CTL_NONE;
//
//	if( m_nState == F3dZUISprite::SET_DISABLE )
//		return nRetValue;
//
//	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
//	{
//		switch( uMsg )
//		{
//		case WM_LBUTTONDBLCLK: {
//			m_bLeftButton = FALSE;
//							   }
//							   break;
//		case WM_RBUTTONDBLCLK: {
//			m_bRightButton = FALSE;
//							   }
//							   break;
//		case WM_MBUTTONDBLCLK: {
//			m_bMiddleButton = FALSE;
//							   }
//							   break;
//		case WM_LBUTTONDOWN: {
//					m_bLeftButton = TRUE;
//							 }
//							 break;
//		case WM_RBUTTONDOWN:		m_bRightButton  = TRUE;			break;
//		case WM_MBUTTONDOWN:		m_bMiddleButton = TRUE;			break;
//		case WM_LBUTTONUP: {
//			if( m_bLeftButton )
//			{
//			}
//			m_bLeftButton = FALSE;
//						   }
//						   break;
//		case WM_RBUTTONUP: {
//			m_bRightButton = FALSE;
//						   }
//						   break;
//		case WM_MBUTTONUP: {
//			m_bMiddleButton = FALSE;
//						   }
//						   break;
//		case WM_MOUSEMOVE: {
//						   }
//						   break;
//
//		case WM_MOUSEWHEEL:
//			{
//				//POINT pt; 
//				//pt.x = (short)LOWORD(lParam); 
//				//pt.y = (short)HIWORD(lParam);
//				//ScreenToClient( F3dZWndManager::GetSingleton()->GetHWND(), &pt );
//				//short z = HIWORD(wParam);
//				//if(z > 0) {
//				//	//UP
//				//}
//				//else      {
//				//	//DOWN
//				//}
//				//break;
//			}
//		}
//
//		for( UINT i(0); m_vChildList.size()>i; ++i )
//			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
//
//		if( m_bMouseDrag )
//		{
//			switch( uMsg )
//			{
//			case WM_LBUTTONUP:
//				m_bMouseDrag = FALSE;
//				break;
//			case WM_MOUSEMOVE: {
//							   }
//							   break;
//			}
//		}
//		else if( m_Bottom_rect.IsInRect(x, y) )
//		{
//			switch( uMsg )
//			{
//				case WM_LBUTTONDOWN:
//					m_ptMovingOffset.x = x - m_Bottom_rect.left, 
//					m_ptMovingOffset.y = y - m_Bottom_rect.top;
//					m_bMouseDrag = TRUE;
//					break;
//			}
//		}
//
//		nRetValue = _RET_CTL_USED;
//	}
//	else
//	{
//		if( uMsg == WM_LBUTTONUP )
//		{
//			m_bLeftButton = FALSE;
//		}
//		else if( uMsg == WM_RBUTTONUP )
//			m_bRightButton = FALSE;
//		else if( uMsg == WM_MBUTTONUP )
//			m_bMiddleButton = FALSE;
//
//		for( UINT i(0); m_vChildList.size()>i; ++i )
//			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
//	}
//
//	return nRetValue;
//}

void F3dZControlVScroll::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT top_rect, mid_rect, bottom_rect;

	//Top
	if( m_pCtrl[_TOP] ) {
		m_pCtrl[_TOP]->GetRect( top_rect );
		m_pCtrl[_TOP]->SetSize( top_rect.GetWidth(), top_rect.GetHeight() );
		m_pCtrl[_TOP]->SetPos( m_rect.left, m_rect.top );
	}

	//Bottom
	if( m_pCtrl[_BOTTOM] ) {
		m_pCtrl[_BOTTOM]->GetRect( bottom_rect );
		m_pCtrl[_BOTTOM]->SetSize( bottom_rect.GetWidth(), bottom_rect.GetHeight() );
		m_pCtrl[_BOTTOM]->SetPos( m_rect.left, m_rect.bottom-bottom_rect.GetHeight() );
	}

	//Thumb
	if( m_pCtrl[_THUMB] ) {
		m_pCtrl[_THUMB]->GetRect( mid_rect );
		m_pCtrl[_THUMB]->SetSize( mid_rect.GetWidth(), m_rect.GetHeight()-top_rect.GetHeight()-bottom_rect.GetHeight() );
		m_pCtrl[_THUMB]->SetPos( m_rect.left, m_rect.top+top_rect.GetHeight() );
	}
}

//마우스 Down 후, CurPos 계산
void F3dZControlVScroll::_EvaluationPosition(int x, int y)
{  
	if( !m_pCtrl[_TOP] || !m_pCtrl[_BOTTOM] || !m_pCtrl[_THUMB] ) return;

	int nHeight  = (m_pCtrl[_BOTTOM]->GetRect().top - m_pCtrl[_TOP]->GetRect().bottom);
	int nDiffThumbHeight = nHeight - ( (m_pCtrl[_THUMB]->GetRect().GetHeight()+1) / 2 );

	// 위에서부터 이동된 거리
	int nHeightDiff = y - m_pCtrl[_TOP]->GetRect().bottom;

	if(nHeightDiff < 0)
		nHeightDiff = 0;

	float fPosRatio = (float)nHeightDiff / (float)nDiffThumbHeight;

	if(fPosRatio > 1.0f)
		fPosRatio = 1.0f;

	m_nCurrentPos = (int)(m_nMaxPos* fPosRatio);
}

//Mouse Drag 후, CurPos 계산
void F3dZControlVScroll::_EvaluationPositionFromThumRect()
{  
	if( !m_pCtrl[_TOP] || !m_pCtrl[_BOTTOM] || !m_pCtrl[_THUMB] ) return;

	int nHeight  = (m_pCtrl[_BOTTOM]->GetRect().top - m_pCtrl[_TOP]->GetRect().bottom);
	int dwHeightDiff = nHeight - m_pCtrl[_THUMB]->GetRect().GetHeight();

	float fPosRatio = 0.f;

	if( dwHeightDiff > 0)
		fPosRatio = (float) (m_pCtrl[_THUMB]->GetRect().top - m_pCtrl[_TOP]->GetRect().bottom) / (float)dwHeightDiff;

	if( m_nMaxPos > 0)
		fPosRatio += 1 / (float)m_nMaxPos / 2.0f;

	m_nCurrentPos = (int)(m_nMaxPos * fPosRatio);
}

//CurPos 로 위치 계산
void F3dZControlVScroll::_EvaluationThumbRect()
{  
	if( !m_pCtrl[_TOP] || !m_pCtrl[_BOTTOM] || !m_pCtrl[_THUMB] ) return;

	int nHeight  = (m_pCtrl[_BOTTOM]->GetRect().top - m_pCtrl[_TOP]->GetRect().bottom);
	int nThumbHeight = m_pCtrl[_THUMB]->GetRect().GetHeight();

	float fRatioPos = 0.0f;

	if( m_nMaxPos > 0 )
		fRatioPos = (float)m_nCurrentPos/m_nMaxPos;

	int dwAddHeight = (int)((nHeight - nThumbHeight) * fRatioPos);

	m_pCtrl[_THUMB]->SetPos( m_pCtrl[_THUMB]->GetRect().left, m_pCtrl[_TOP]->GetRect().bottom + dwAddHeight );
}

void F3dZControlVScroll::_SetThumbRect(int x, int y)
{  
	if( !m_pCtrl[_TOP] || !m_pCtrl[_BOTTOM] || !m_pCtrl[_THUMB] ) return;

	int nMaxPosY = m_pCtrl[_BOTTOM]->GetRect().top - m_pCtrl[_THUMB]->GetRect().GetHeight();

	y -= m_ptMovingOffset.y;
	y = std::max( y, m_pCtrl[_TOP]->GetRect().bottom );
	y = std::min( y, nMaxPosY );

	m_pCtrl[_THUMB]->MovePos( m_rect.left, y );
}

void F3dZControlVScroll::_ResizeThumb()
{  
	if( !m_pCtrl[_TOP] || !m_pCtrl[_BOTTOM] || !m_pCtrl[_THUMB] ) return;

	float fHeight  = (float)(m_pCtrl[_BOTTOM]->GetRect().top - m_pCtrl[_TOP]->GetRect().bottom);

	int nScrollUnit = (int)( fHeight/m_nMinThumbWidth );
	int nNewHeight = (int)fHeight - nScrollUnit * ( (int)m_nMaxPos );
	nNewHeight = std::max( nNewHeight, m_nMinThumbWidth );

	m_pCtrl[_THUMB]->GetRect().bottom = m_pCtrl[_THUMB]->GetRect().top + nNewHeight;

	//Thumb
	if( m_pCtrl[_THUMB] ) {
		m_pCtrl[_THUMB]->SetSize( m_pCtrl[_THUMB]->GetRect().GetWidth(), m_pCtrl[_THUMB]->GetRect().GetHeight() );

		if( m_pCtrl[_THUMB]->GetRect().GetHeight() >= fHeight ) {
			m_pCtrl[_THUMB]->SetShow(TRUE);
			m_pCtrl[_TOP]->SetShow(TRUE);
			m_pCtrl[_BOTTOM]->SetShow(TRUE);
		}
		else {
			m_pCtrl[_THUMB]->SetShow(TRUE);
			m_pCtrl[_TOP]->SetShow(TRUE);
			m_pCtrl[_BOTTOM]->SetShow(TRUE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//HScroll
F3dZControlHScroll::F3dZControlHScroll()
{  

}

F3dZControlHScroll::~F3dZControlHScroll()
{  

}

void F3dZControlHScroll::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );
	//TODO

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefScrollPieceCount ); 
	createButtonLeft ( pSprite, control_arg );
	createButtonRight( pSprite, control_arg );
	createButtonThumb( pSprite, control_arg );
}

void F3dZControlHScroll::createButtonLeft ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_LeftBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pCtrl[_LEFT] = pControl;
		}
	}
}
void F3dZControlHScroll::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 8;
	m_nMinThumbWidth = 1;
	for( short i(0); c_nDefMidCount>i; ++i ) {
		if( i == 0 || i == 2 ) {
			F3dCTexture* pTexture = F3dCTextureManager::GetSingleton()->FindTexture( pSprite->vTexList[0][nStartIdx+i]->TextureID );
			if( pTexture )
				m_nMinThumbWidth += pTexture->GetHeight();
		}
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x + m_pCtrl[_LEFT]->GetRect().GetWidth();
	child_control_arg.y       = control_arg.y;
	child_control_arg.nWidth  = control_arg.nWidth - m_pCtrl[_LEFT]->GetRect().GetWidth()-m_pCtrl[_RIGHT]->GetRect().GetWidth();
	child_control_arg.nHeight = m_pCtrl[_LEFT]->GetRect().GetHeight();

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_3( c_nDefMidCount/3, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		//		pControl->SetFlag( _NONE_FLAG );
		addChildControl( pControl );

		m_pCtrl[_THUMB] = pControl;
	}
}
void F3dZControlHScroll::createButtonRight( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 4;
	for( short i(0); c_nDefSetCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][nStartIdx+i] );
	}

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_RightBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x + control_arg.nWidth - pTex->nWidth;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pCtrl[_RIGHT] = pControl;
		}
	}
}

void F3dZControlHScroll::Process( double dTime, double dMeanDT )
{  
	//TODO
	F3dZControl::Process( dTime, dMeanDT );
}

void F3dZControlHScroll::Render( double dTime )
{  
	//TODO
	if( IsShow() == FALSE ) return;

	renderChild( dTime );
}

void F3dZControlHScroll::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT top_rect, mid_rect, bottom_rect;

	//Left
	if( m_pCtrl[_LEFT] ) {
		m_pCtrl[_LEFT]->GetRect( top_rect );
		m_pCtrl[_LEFT]->SetSize( top_rect.GetWidth(), top_rect.GetHeight() );
		m_pCtrl[_LEFT]->SetPos( m_rect.left, m_rect.top );
	}

	//Right
	if( m_pCtrl[_RIGHT] ) {
		m_pCtrl[_RIGHT]->GetRect( bottom_rect );
		m_pCtrl[_RIGHT]->SetSize( bottom_rect.GetWidth(), bottom_rect.GetHeight() );
		m_pCtrl[_RIGHT]->SetPos( m_rect.right-bottom_rect.GetWidth(), m_rect.top );
	}

	//Thumb
	if( m_pCtrl[_THUMB] ) {
		m_pCtrl[_THUMB]->GetRect( mid_rect );
		m_pCtrl[_THUMB]->SetSize( m_rect.GetWidth()-top_rect.GetWidth()-bottom_rect.GetWidth(), mid_rect.GetHeight() );
		m_pCtrl[_THUMB]->SetPos( m_rect.left+top_rect.GetWidth(), m_rect.top );
	}
}

//마우스 Down 후, CurPos 계산
void F3dZControlHScroll::_EvaluationPosition(int x, int y)
{  
	if( !m_pCtrl[_LEFT] || !m_pCtrl[_RIGHT] || !m_pCtrl[_THUMB] ) return;

	int nWidth          = (m_pCtrl[_RIGHT]->GetRect().left - m_pCtrl[_LEFT]->GetRect().right);
	int nDiffThumbWidth = nWidth - ( (m_pCtrl[_THUMB]->GetRect().GetWidth()+1) / 2 );

	// 위에서부터 이동된 거리
	int nWidthDiff = x - m_pCtrl[_LEFT]->GetRect().right;

	if(nWidthDiff < 0)
		nWidthDiff = 0;

	float fPosRatio = (float)nWidthDiff / (float)nDiffThumbWidth;

	if(fPosRatio > 1.0f)
		fPosRatio = 1.0f;

	m_nCurrentPos = (int)(m_nMaxPos* fPosRatio);
}

//Mouse Drag 후, CurPos 계산
void F3dZControlHScroll::_EvaluationPositionFromThumRect()
{  
	if( !m_pCtrl[_LEFT] || !m_pCtrl[_RIGHT] || !m_pCtrl[_THUMB] ) return;

	int nWidth      = (m_pCtrl[_RIGHT]->GetRect().left - m_pCtrl[_LEFT]->GetRect().right);
	int dwWidthDiff = nWidth - m_pCtrl[_THUMB]->GetRect().GetWidth();

	float fPosRatio = 0.f;

	if( dwWidthDiff > 0)
		fPosRatio = (float) (m_pCtrl[_THUMB]->GetRect().left - m_pCtrl[_LEFT]->GetRect().right) / (float)dwWidthDiff;

	if( m_nMaxPos > 0)
		fPosRatio += 1 / (float)m_nMaxPos / 2.0f;

	m_nCurrentPos = (int)(m_nMaxPos * fPosRatio);
}

//CurPos 로 위치 계산
void F3dZControlHScroll::_EvaluationThumbRect()
{  
	if( !m_pCtrl[_LEFT] || !m_pCtrl[_RIGHT] || !m_pCtrl[_THUMB] ) return;

	int nWidth      = (m_pCtrl[_RIGHT]->GetRect().left - m_pCtrl[_LEFT]->GetRect().right);
	int nThumbWidth = m_pCtrl[_THUMB]->GetRect().GetWidth();

	float fRatioPos = 0.0f;

	if( m_nMaxPos > 0 )
		fRatioPos = (float)m_nCurrentPos/m_nMaxPos;

	int dwAddWidth = (int)((nWidth - nThumbWidth) * fRatioPos);

	m_pCtrl[_THUMB]->SetPos( m_pCtrl[_LEFT]->GetRect().right + dwAddWidth, m_pCtrl[_THUMB]->GetRect().top );
}

void F3dZControlHScroll::_SetThumbRect(int x, int y)
{  
	if( !m_pCtrl[_LEFT] || !m_pCtrl[_RIGHT] || !m_pCtrl[_THUMB] ) return;

	int nMaxPosX = m_pCtrl[_RIGHT]->GetRect().left - m_pCtrl[_THUMB]->GetRect().GetWidth();

	x -= m_ptMovingOffset.x;
	x = std::max( x, m_pCtrl[_LEFT]->GetRect().right );
	x = std::min( x, nMaxPosX );

	m_pCtrl[_THUMB]->MovePos( x, m_rect.top );
}

void F3dZControlHScroll::_ResizeThumb()
{  
	if( !m_pCtrl[_LEFT] || !m_pCtrl[_RIGHT] || !m_pCtrl[_THUMB] ) return;

	float fWidth  = (float)(m_pCtrl[_RIGHT]->GetRect().left - m_pCtrl[_LEFT]->GetRect().right);

	int nScrollUnit = (int)( fWidth/m_nMinThumbWidth );
	int nNewWidth = (int)fWidth - nScrollUnit * ( (int)m_nMaxPos );
	nNewWidth = std::max( nNewWidth, m_nMinThumbWidth );

	m_pCtrl[_THUMB]->GetRect().right = m_pCtrl[_THUMB]->GetRect().left + nNewWidth;

	//Thumb
	if( m_pCtrl[_THUMB] ) {
		m_pCtrl[_THUMB]->SetSize( m_pCtrl[_THUMB]->GetRect().GetWidth(), m_pCtrl[_THUMB]->GetRect().GetHeight() );

		if( m_pCtrl[_THUMB]->GetRect().GetWidth() >= fWidth ) {
			m_pCtrl[_THUMB]->SetShow(FALSE);
			m_pCtrl[_LEFT]->SetShow(FALSE);
			m_pCtrl[_RIGHT]->SetShow(FALSE);
		}
		else {
			m_pCtrl[_THUMB]->SetShow(TRUE);
			m_pCtrl[_LEFT]->SetShow(TRUE);
			m_pCtrl[_RIGHT]->SetShow(TRUE);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Editable VScroll
void F3dZEditableControlVScroll::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();	
	assert( nPieceCount == c_nDefScrollPieceCount ); 
	createButtonTop   ( pSprite, control_arg );
	createButtonBottom( pSprite, control_arg );
	createButtonThumb ( pSprite, control_arg );
}

void F3dZEditableControlVScroll::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	renderChild( dTime );
}

void F3dZEditableControlVScroll::createButtonTop   ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_TopBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;
	 
		m_Top_rect.left   = child_control_arg.x;
		m_Top_rect.top    = child_control_arg.y;
		m_Top_rect.right  = m_Top_rect.left + pTex->nWidth;
		m_Top_rect.bottom = m_Top_rect.top  + pTex->nHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			addChildControl( pControl );
		}
	}
}

void F3dZEditableControlVScroll::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 8;
	for( short i(0); c_nDefMidCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y + m_Top_rect.GetHeight();
	child_control_arg.nWidth  = m_Top_rect.GetWidth();
	child_control_arg.nHeight = control_arg.nHeight - m_Top_rect.GetHeight()-m_Bottom_rect.GetHeight();

	m_Thumb_rect.left   = child_control_arg.x;
	m_Thumb_rect.top    = child_control_arg.y;
	m_Thumb_rect.right  = m_Top_rect.left + m_Top_rect.GetWidth();
	m_Thumb_rect.bottom = m_Top_rect.top  + m_Bottom_rect.GetHeight();

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetVertical( TRUE );
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_Vertical_3( c_nDefMidCount/3, texlist, child_control_arg );
		addChildControl( pControl );
	}
}

void F3dZEditableControlVScroll::createButtonBottom( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 4;
	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][nStartIdx+i] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_BottomBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y + control_arg.nHeight - pTex->nHeight;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		m_Bottom_rect.left   = child_control_arg.x;
		m_Bottom_rect.top    = child_control_arg.y;
		m_Bottom_rect.right  = m_Bottom_rect.left + pTex->nWidth;
		m_Bottom_rect.bottom = m_Bottom_rect.top  + pTex->nHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			addChildControl( pControl );
		}
	}
}

void F3dZEditableControlVScroll::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT top_rect, mid_rect, bottom_rect;

	//Top
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_TopBtn", GetID() );
	F3dZEditableControl* pTop = GetChildControl( strID.c_str() );
	if( pTop ) {
		pTop->GetRect( top_rect );
		pTop->SetSize( top_rect.GetWidth(), top_rect.GetHeight() );
		pTop->SetPos( m_rect.left, m_rect.top );
	}

	//Bottom
	MBStringUtil::StringFormat( strID, L"%s_BottomBtn", GetID() );
	F3dZEditableControl* pBottom = GetChildControl( strID.c_str() );
	if( pBottom ) {
		pBottom->GetRect( bottom_rect );
		pBottom->SetSize( bottom_rect.GetWidth(), bottom_rect.GetHeight() );
		pBottom->SetPos( m_rect.left, m_rect.bottom-bottom_rect.GetHeight() );
	}

	//Mid
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );
	F3dZEditableControl* pMid = GetChildControl( strID.c_str() );
	if( pMid ) {
		pMid->GetRect( mid_rect );
		pMid->SetSize( mid_rect.GetWidth(), m_rect.GetHeight()-top_rect.GetHeight()-bottom_rect.GetHeight() );
		pMid->SetPos( m_rect.left, m_rect.top+top_rect.GetHeight() );
	}
}

int	F3dZEditableControlVScroll::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
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
//////////////////////////////////////////////////////////////////////////
//Editable HScroll
void F3dZEditableControlHScroll::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();	
	assert( nPieceCount == c_nDefScrollPieceCount ); 
	createButtonLeft ( pSprite, control_arg );
	createButtonRight( pSprite, control_arg );
	createButtonThumb( pSprite, control_arg );
}

void F3dZEditableControlHScroll::createButtonLeft   ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_LeftBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		m_Left_rect.left   = child_control_arg.x;
		m_Left_rect.top    = child_control_arg.y;
		m_Left_rect.right  = m_Left_rect.left + pTex->nWidth;
		m_Left_rect.bottom = m_Left_rect.top  + pTex->nHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			addChildControl( pControl );
		}
	}
}

void F3dZEditableControlHScroll::createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 8;
	for( short i(0); c_nDefMidCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x + m_Left_rect.GetWidth();
	child_control_arg.y       = control_arg.y;
	child_control_arg.nWidth  = control_arg.nWidth - m_Left_rect.GetWidth()-m_Right_rect.GetWidth();
	child_control_arg.nHeight = m_Left_rect.GetHeight();

	m_Thumb_rect.left   = child_control_arg.x;
	m_Thumb_rect.top    = child_control_arg.y;
	m_Thumb_rect.right  = m_Left_rect.left + m_Right_rect.GetWidth();
	m_Thumb_rect.bottom = m_Left_rect.top  + m_Left_rect.GetHeight();

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_3( c_nDefMidCount/3, texlist, child_control_arg );
		addChildControl( pControl );
	}
}

void F3dZEditableControlHScroll::createButtonRight( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 4;
	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][nStartIdx+i] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_RightBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x + control_arg.nWidth - pTex->nWidth;
		child_control_arg.y       = control_arg.y;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		m_Right_rect.left   = child_control_arg.x;
		m_Right_rect.top    = child_control_arg.y;
		m_Right_rect.right  = m_Right_rect.left + pTex->nWidth;
		m_Right_rect.bottom = m_Right_rect.top  + pTex->nHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			addChildControl( pControl );
		}
	}
}


void F3dZEditableControlHScroll::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	renderChild( dTime );
}

void F3dZEditableControlHScroll::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT top_rect, mid_rect, bottom_rect;

	//Top
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_LeftBtn", GetID() );
	F3dZEditableControl* pLeft = GetChildControl( strID.c_str() );
	if( pLeft ) {
		pLeft->GetRect( top_rect );
		pLeft->SetSize( top_rect.GetWidth(), top_rect.GetHeight() );
		pLeft->SetPos( m_rect.left, m_rect.top );
	}

	//Bottom
	MBStringUtil::StringFormat( strID, L"%s_RightBtn", GetID() );
	F3dZEditableControl* pRight = GetChildControl( strID.c_str() );
	if( pRight ) {
		pRight->GetRect( bottom_rect );
		pRight->SetSize( bottom_rect.GetWidth(), bottom_rect.GetHeight() );
		pRight->SetPos( m_rect.right-bottom_rect.GetWidth(), m_rect.top );
	}

	//Mid
	MBStringUtil::StringFormat( strID, L"%s_ThumbBtn", GetID() );
	F3dZEditableControl* pMid = GetChildControl( strID.c_str() );
	if( pMid ) {
		pMid->GetRect( mid_rect );
		pMid->SetSize( m_rect.GetWidth()-top_rect.GetWidth()-bottom_rect.GetWidth(), mid_rect.GetHeight() );
		pMid->SetPos( m_rect.left+top_rect.GetWidth(), m_rect.top );
	}
}

int	F3dZEditableControlHScroll::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
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
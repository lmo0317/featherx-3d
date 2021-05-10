#include "FeatherXEnginePrecompile.h"
 

//////////////////////////////////////////////////////////////////////////
//F3dZControlButton
F3dZControlButton::F3dZControlButton()
{

}

F3dZControlButton::~F3dZControlButton()
{

}

void F3dZControlButton::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount < 0 ) return;

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
	     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
	else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
	else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
}

void F3dZControlButton::Process( double dTime, double dMeanDT )
{
	F3dZControl::Process( dTime, dMeanDT );
}

void F3dZControlButton::RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer )
{
	for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
	{
		RenderLayer.RegUISprite( m_vSprite[m_nState][i].get(), nLayer );
	}
}

void F3dZControlButton::Render( double dTime )
{ 
	if( IsShow() == FALSE ) return;

	for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
	{
		F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	renderCaption( dTime );

	renderWire();
}

//////////////////////////////////////////////////////////////////////////
//F3dZControlPushButton
F3dZControlPushButton::F3dZControlPushButton() : m_bIsDown(FALSE), m_bSkipOver(FALSE)
{

}

F3dZControlPushButton::~F3dZControlPushButton()
{

}

void F3dZControlPushButton::SetDown( BOOL bFlag ) 
{ 
	m_bIsDown = bFlag; 

	if( IsDown() )
		setState( F3dZUISprite::SET_DOWN );
	else
		setState( F3dZUISprite::SET_NORMAL );
}

int	F3dZControlPushButton::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{
	int nRetValue = _RET_CTL_NONE;

	if( m_nState == F3dZUISprite::SET_DISABLE || m_bEnable == FALSE )
		return nRetValue;

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
	{
		switch( uMsg )
		{
		case WM_LBUTTONDBLCLK: {
			m_bLeftButton = FALSE;
			OnControlNotify( LBUTTON_DBL_CLICK, GetID() );
							   }
							   break;
		case WM_RBUTTONDBLCLK: {
			m_bRightButton = FALSE;
			OnControlNotify( RBUTTON_DBL_CLICK, GetID() );
							   }
							   break;
		case WM_MBUTTONDBLCLK: {
			m_bMiddleButton = FALSE;
			OnControlNotify( MBUTTON_DBL_CLICK, GetID() );
							   }
							   break;
		case WM_LBUTTONDOWN: {
			if( GetParent() )
			{
				if( IsFlag(_CAPTURE_FLAG) && GetParent()->SetCaptureControl(this) )
				{
					if( IsDown() )
						setState( F3dZUISprite::SET_DOWN );
					else
						setState( F3dZUISprite::SET_NORMAL );

					OnControlNotify( PUSH_TYPE_CHANGE, GetID() );

					m_bLeftButton = TRUE;
				}
			}
							 }
							 break;
		case WM_RBUTTONDOWN:		m_bRightButton  = TRUE;			break;
		case WM_MBUTTONDOWN:		m_bMiddleButton = TRUE;			break;
		case WM_LBUTTONUP: {
			if( m_bLeftButton )
			{
				if( IsDown() )
					SetDown( FALSE );
				else
					SetDown( TRUE );

				OnControlNotify( LBUTTON_CLICK, GetID(), (IsDown() ? 1 : 0) );
				if( GetParent() )
					GetParent()->ReleaseCaptureControl(this);
			}
			m_bLeftButton = FALSE;
						   }
						   break;
		case WM_RBUTTONUP: {
			if( m_bRightButton )
				OnControlNotify( RBUTTON_CLICK, GetID() );
			m_bRightButton = FALSE;
						   }
						   break;
		case WM_MBUTTONUP: {
			if( m_bMiddleButton )
				OnControlNotify( MBUTTON_CLICK, GetID() );
			m_bMiddleButton = FALSE;
						   }
						   break;
		case WM_MOUSEMOVE: {
			if( m_bLeftButton )
			{
				if( IsDown() )
					setState( F3dZUISprite::SET_DOWN );
				else
					setState( F3dZUISprite::SET_NORMAL );
			}
			else
			{
				if( IsDown() && IsSkipOver() )	{

				}
				else {
					if( m_nState != F3dZUISprite::SET_OVER ) 
						OnControlNotify( MOUSE_OVER, GetID() );
					setState( F3dZUISprite::SET_OVER );
				}
			}
			//ToolTip 활성화 요청
			GetParent()->SetToolTip( IsFlag(_MSG_TOOL_TIP) ? this : NULL, GetToolTipType() ); 
						   }
						   break;
		}
		nRetValue = _RET_CTL_USED;
	}
	else
	{
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

		if( m_nState == F3dZUISprite::SET_DOWN || m_nState == F3dZUISprite::SET_OVER ) {
			if( IsDown() )
				setState( F3dZUISprite::SET_DOWN   );
			else
				setState( F3dZUISprite::SET_NORMAL );
		}
	}

	return nRetValue;	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlCheckButton::F3dZControlCheckButton()
{
	
}

F3dZControlCheckButton::~F3dZControlCheckButton()
{
	
}

int	F3dZControlCheckButton::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{
	int nRetValue = _RET_CTL_NONE;

	if( m_bReact == false)
		return nRetValue;

	if( m_nState == F3dZUISprite::SET_CHECK_DISABLE || m_bEnable == FALSE )
		return nRetValue;

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
	{
		switch( uMsg )
		{
		case WM_LBUTTONDBLCLK: {
			m_bLeftButton = FALSE;
			OnControlNotify( LBUTTON_DBL_CLICK, GetID() );
							   }
							   break;
		case WM_RBUTTONDBLCLK: {
			m_bRightButton = FALSE;
			OnControlNotify( RBUTTON_DBL_CLICK, GetID() );
							   }
							   break;
		case WM_MBUTTONDBLCLK: {
			m_bMiddleButton = FALSE;
			OnControlNotify( MBUTTON_DBL_CLICK, GetID() );
							   }
							   break;
		case WM_LBUTTONDOWN: {
			if( GetParent() )
			{
				if( IsFlag(_CAPTURE_FLAG) && GetParent()->SetCaptureControl(this) )
				{
					if( IsDown() )
						setState( F3dZUISprite::SET_CHECK_ON );
					else
						setState( F3dZUISprite::SET_CHECK_OFF );

					OnControlNotify( PUSH_TYPE_CHANGE, GetID() );

					m_bLeftButton = TRUE;
				}
			}
							 }
							 break;
		case WM_RBUTTONDOWN:		m_bRightButton  = TRUE;			break;
		case WM_MBUTTONDOWN:		m_bMiddleButton = TRUE;			break;
		case WM_LBUTTONUP: {
			if( m_bLeftButton )
			{
				if( IsDown() )
					SetDown( FALSE );
				else
					SetDown( TRUE );

				OnControlNotify( LBUTTON_CLICK, GetID(), (IsDown() ? 1 : 0) );
				if( GetParent() )
					GetParent()->ReleaseCaptureControl(this);
			}
			m_bLeftButton = FALSE;
						   }
						   break;
		case WM_RBUTTONUP: {
			if( m_bRightButton )
				OnControlNotify( RBUTTON_CLICK, GetID() );
			m_bRightButton = FALSE;
						   }
						   break;
		case WM_MBUTTONUP: {
			if( m_bMiddleButton )
				OnControlNotify( MBUTTON_CLICK, GetID() );
			m_bMiddleButton = FALSE;
						   }
						   break;
		case WM_MOUSEMOVE: {
			if( m_bLeftButton )
			{
				if( IsDown() )
					setState( F3dZUISprite::SET_CHECK_ON );
				else
					setState( F3dZUISprite::SET_CHECK_OFF );
			}
			else
			{
				if( IsDown() )
					setState( F3dZUISprite::SET_CHECK_ON_OVER );
				else
					setState( F3dZUISprite::SET_CHECK_OFF_OVER );
			}
			//ToolTip 활성화 요청
			GetParent()->SetToolTip( IsFlag(_MSG_TOOL_TIP) ? this : NULL, GetToolTipType() ); 
						   }
						   break;
		}
		nRetValue = _RET_CTL_USED;
	}
	else
	{
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

		if( m_nState == F3dZUISprite::SET_CHECK_ON_OVER || m_nState == F3dZUISprite::SET_CHECK_OFF_OVER ) {
			if( IsDown() )
				setState( F3dZUISprite::SET_CHECK_ON   );
			else
				setState( F3dZUISprite::SET_CHECK_OFF );
		}
	}

	return nRetValue;	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZEditableControlButton::F3dZEditableControlButton()
{

}

F3dZEditableControlButton::~F3dZEditableControlButton()
{

}

void F3dZEditableControlButton::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount < 0 ) return;

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
	     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
	else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
	else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );

	AfterCreateControl();
}

void F3dZEditableControlButton::Process( double dTime, double dMeanDT )
{

}

void F3dZEditableControlButton::Render( double dTime )
{
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
	{
		F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	renderCaption( dTime );
}
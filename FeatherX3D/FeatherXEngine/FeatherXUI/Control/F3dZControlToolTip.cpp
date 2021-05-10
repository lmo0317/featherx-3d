#include "FeatherXEnginePrecompile.h"
 

namespace
{
	const double c_dToolTipHideTime  = 0.04f;
	const double c_dToolTipShowTime  = 0.1f;
	const int	 c_nToolTipDefHeight = 18;
};

F3dZControlToolTip::F3dZControlToolTip() : m_pOwnerControl(NULL), m_bToolTipEnable(false), m_dEnableTime(0.f)
{ 
	 
}

F3dZControlToolTip::~F3dZControlToolTip()
{ 
	 
}

void F3dZControlToolTip::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
	 
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}
}

void F3dZControlToolTip::CheckOwnerWindowClose( const WCHAR* pID )
{
	 
	if( m_pOwnerControl && m_pOwnerControl->GetParent() ) 
	{
		if( !_wcsicmp( pID, m_pOwnerControl->GetParent()->GetID() ) )	
		{
			SetOwner( NULL );
			SetToolTipEnable( false );
		}
	}
}

void F3dZControlToolTip::Process( double dTime, double dMeanDT )
{ 
	 
	m_dCurTime = dTime;
	if( IsShow() ) {
		if( !m_bToolTipEnable ) {
			if( (m_dCurTime-m_dEnableTime) >= c_dToolTipHideTime ) {
				SetShow( FALSE );
				SetOwner( NULL );
			}
		}
	}
	else {
		if( m_bToolTipEnable ) {
//			if( (m_dCurTime-m_dEnableTime) >= c_dToolTipShowTime ) {
				SetShow( TRUE );
//			}	
		}
	}
}

void F3dZControlToolTip::Render( double dTime )
{ 
	 
	if( IsShow() == FALSE ) return;

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	renderChild( dTime );
//	renderCaption( dTime );

	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->Render( dTime );

	for( UINT i(0); m_vSubTextList.size()>i; ++i )
		m_vSubTextList[i]->Render( dTime );

	renderWire();
}

int	F3dZControlToolTip::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{ 
	 
	if(	!m_bEnable )		return _RET_CTL_NONE;

	return F3dZControl::OnMouseMsg( uMsg, x, y, nPrevRetValue );
}

void F3dZControlToolTip::LimitResolution( int nLeft, int nTop, int nRight, int nBottom )
{ 
	 
	int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();

	UI_RECT ui_rect;
	UI_RECT full_rect( 0+nLeft, 0+nTop, nWidth+nRight, nHeight+nBottom );
	GetRect( ui_rect );

	POINT sizeMoveOffset;
	sizeMoveOffset.x = 0;
	sizeMoveOffset.y = 0;

	if( ui_rect.left   < full_rect.left   ) sizeMoveOffset.x += (full_rect.left   - ui_rect.left  );
	if( ui_rect.top    < full_rect.top    ) sizeMoveOffset.y += (full_rect.top    - ui_rect.top   );
	if( ui_rect.right  > full_rect.right  ) sizeMoveOffset.x += (full_rect.right  - ui_rect.right );
	if( ui_rect.bottom > full_rect.bottom ) sizeMoveOffset.y += (full_rect.bottom - ui_rect.bottom);

	if( 0 != sizeMoveOffset.x || 0 != sizeMoveOffset.y ) {
		MovePosOffset( sizeMoveOffset.x, sizeMoveOffset.y );
		TextMove();
	}
}

void F3dZControlToolTip::TextMove()
{ 
	 
	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->SetPos( m_nPosX, m_nPosY+(i*c_nToolTipDefHeight) );	

	for( UINT i(0); m_vSubTextList.size()>i; ++i )
		m_vSubTextList[i]->SetPos( m_nPosX, m_nPosY+(i*c_nToolTipDefHeight) );	
}

void F3dZControlToolTip::SetToolTipEnable( bool bFlag ) 
{ 
	 
	m_bToolTipEnable = bFlag; 
	m_dEnableTime    = m_dCurTime; 
}

void F3dZControlToolTip::MovePos( int x, int y )
{ 
	 
	F3dZControl::MovePos( x, y );

	TextMove();
}

void F3dZControlToolTip::ParseToolTip( const WCHAR* pStrToolTip )
{ 
	 
}

void F3dZControlToolTip::ParseToolTipEx( const WCHAR* pStrToolTip, const WCHAR* pStrToolTipTitle )
{ 
	 
}

void F3dZControlToolTip::ParseSubToolTipEx( const WCHAR* pStrToolTip, const WCHAR* pStrToolTipTitle )
{
	 
}
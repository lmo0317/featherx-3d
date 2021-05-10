
#include "FeatherXEnginePrecompile.h"

using namespace MBStringUtil;

int  F3dZEditableWnd::s_nGrid = 1;
bool F3dZEditableWnd::s_bRenderBG = false;

F3dZEditableWnd::F3dZEditableWnd( F3dZEditableWndManager* pWndManager ) : m_pWndManager(pWndManager), m_bShow(FALSE), m_bMoving(FALSE), m_pCaptureControl(NULL)
{  
	m_pBG = NULL;
}

F3dZEditableWnd::~F3dZEditableWnd()
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pCurControl = (*it);
		delete pCurControl;
	}
}

void F3dZEditableWnd::RefreshBG()
{  
	if( m_pBG )
	{
		delete m_pBG;
		m_pBG = NULL;
	}

	if( !m_strSprBG.empty() )
	{
		CONTROL_ARG control_arg;
		control_arg.ID      = L"window_BG";
		control_arg.strAni  = m_strSprBG;

		control_arg.x       = m_rect.left;
		control_arg.y       = m_rect.top;
		control_arg.nWidth  = m_rect.right;
		control_arg.nHeight = m_rect.bottom;

		m_pBG = F3dZEditableUIFactory::GetSingleton()->CreateControl( control_arg );
	}
}

BOOL F3dZEditableWnd::CreateWnd( const WINDOW_ARG& window_arg )
{  
	m_ID        = window_arg.window_info.ID;
//	m_CaptionID = window_arg.window_info.nCaptionID;

	m_CaptionID = L"Not Found String";

	int left   = 0;
	int top    = 0;
	int right  = window_arg.window_info.nWidth  + left;
	int bottom = window_arg.window_info.nHeight + top;

	m_rect     = UI_RECT( left, top, right, bottom );
	ori_m_rect = UI_RECT( 0, 0, window_arg.window_info.nWidth, window_arg.window_info.nHeight );

	//TODO
//	window_arg.window_info.strStyle;
//	window_arg.window_info.nToolTipID;
//	;

	return FALSE;
}

struct CONTROL_GREATE
{
	bool operator()( const F3dZEditableControl* pIt1, const F3dZEditableControl* pIt2 )
	{
		if( pIt1->GetDepth() > pIt2->GetDepth() )
			return true;

		return false;
	}
};

struct CONTROL_LESS
{
	bool operator()( const F3dZEditableControl* pIt1, const F3dZEditableControl* pIt2 )
	{
		if( pIt1->GetDepth() < pIt2->GetDepth() )
			return true;

		return false;
	}
};

void F3dZEditableWnd::SortControl()
{  
	std::sort( m_vControlList.begin(), m_vControlList.end(), CONTROL_GREATE() );
}

BOOL F3dZEditableWnd::RemoveControl( F3dZEditableControl* pControl )
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pCurControl = (*it);
		if( pCurControl == pControl )
		{
			delete pCurControl;
			m_vControlList.erase( it );
			return TRUE;
		}
	}

	return FALSE;
}

void F3dZEditableWnd::AddControl( F3dZEditableControl* pControl )
{  
	pControl->SetParent(this);
	m_vControlList.push_back( pControl );
}

F3dZEditableControl* F3dZEditableWnd::GetControl( const WCHAR* ID )
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		if( !_wcsicmp( pControl->GetID(), ID ) )
			return pControl;
	}

	return NULL;
}

void F3dZEditableWnd::GetControlList( std::vector<std::wstring>& control_list )
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		control_list.push_back( pControl->GetID() );
	}
}

void F3dZEditableWnd::CheckGroup( int nX, int nY, UI_RECT& rect )
{  
	m_vGroupList.clear();

	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);

		if( !pControl->IsShow() ) continue;

		UI_RECT self_rect;
		pControl->GetRect( self_rect );
		
		if( rect.IsIntersect( self_rect ) ) {
			m_GroupRect.Union( self_rect ); //그룹 전체 포괄 범위
			pControl->SetSelect( TRUE );

			pControl->SetMoveAble( TRUE, nX, nY );

			m_vGroupList.push_back( pControl );
		}
		else {
			pControl->SetSelect( FALSE );
			pControl->SetMoveAble( FALSE, nX, nY );
		}
	}
}

void F3dZEditableWnd::Process( double dTime, double dMeanDT )
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		pControl->Process( dTime, dMeanDT );
	}
}

void F3dZEditableWnd::Render( double dTime )
{  
	if( s_bRenderBG && m_pBG )
		m_pBG->Render( dTime );

	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		pControl->Render( dTime );
	}
}

BOOL F3dZEditableWnd::SetCaptureControl( F3dZEditableControl* pControl )
{  
	if( NULL == m_pCaptureControl )
	{
		m_pCaptureControl = pControl;
		return TRUE;
	}
	return FALSE;
}

void F3dZEditableWnd::ReleaseCaptureControl( F3dZEditableControl* pControl )
{  
	assert(m_pCaptureControl);

	if( m_pCaptureControl == pControl )
		m_pCaptureControl = NULL;
}

void F3dZEditableWnd::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	ori_m_rect.right  = nWidth;
	ori_m_rect.bottom = nHeight;

	RefreshBG();
}

BOOL F3dZEditableWnd::IsInRect( const int& x, const int& y )
{  
	return m_rect.IsInRect( x, y );
}

void F3dZEditableWnd::MovePos( int x, int y )
{  
	int offsetx = x - m_rect.left;
	int offsety = y - m_rect.top;
	MovePosOffset( offsetx, offsety);
}

void F3dZEditableWnd::MovePosOffset( int offsetx, int offsety )
{  
	// Region
	m_rect.left   += offsetx;	
	m_rect.top    += offsety;
	m_rect.right  += offsetx;	
	m_rect.bottom += offsety;    

	if( m_pBG )
		m_pBG->MovePos( offsetx, offsety );

	std::vector<F3dZEditableControl*>::reverse_iterator it = m_vControlList.rbegin();
	for( ; it != m_vControlList.rend(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		pControl->MovePos( offsetx, offsety );
	}
}

int	F3dZEditableWnd::OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{  
	int nRetValue = _RET_WND_NONE;

	//맨 위에것 부터 처리
	std::vector<F3dZEditableControl*>::reverse_iterator it = m_vControlList.rbegin();
	for( ; it != m_vControlList.rend(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		nRetValue |= pControl->OnKeyMsg( uMsg, wParam, lParam );
	}

	return nRetValue;
}

int	F3dZEditableWnd::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{  
	if( nPrevRetValue & _RET_WND_USED )
		return _RET_WND_NONE;

	int nRetValue = F3dZEditableControl::_RET_CTL_NONE;

	//Mouse Move
	if( uMsg == WM_MOUSEMOVE )
	{
		if( IsMoveAble() )
		{
			int XOffset = ((x - m_ptMovingOffset.x)/s_nGrid)*s_nGrid;
			int YOffset = ((y - m_ptMovingOffset.y)/s_nGrid)*s_nGrid;

			if(XOffset >= 0 || YOffset >= 0)
			{
				MovePos( XOffset, YOffset );
			}
			nRetValue = _RET_WND_USED;
			return nRetValue;
		}
	}

	//그룹 처리
	if( !m_vGroupList.empty() ) {
		BOOL bIsIn = m_GroupRect.IsInRect( x, y );

		if( bIsIn && uMsg == WM_MOUSEMOVE ) {
			for( UINT i(0); m_vGroupList.size()>i; ++i ) {
				F3dZEditableControl* pControl = m_vGroupList[i];
				nRetValue |= pControl->OnMouseMsg( uMsg, x, y, nRetValue );
			}
		}

		return nRetValue;
	}

	if( GetCaptureControl() )
	{
		nRetValue |= GetCaptureControl()->OnMouseMsg( uMsg, x, y, nRetValue );
	}
	else
	{
		///Control
		//맨 위에것 부터 처리
		std::vector<F3dZEditableControl*>::reverse_iterator it = m_vControlList.rbegin();
		for( ; it != m_vControlList.rend(); ++it )
		{
			F3dZEditableControl* pControl = (*it);
			nRetValue |= pControl->OnMouseMsg( uMsg, x, y, nRetValue );
		}
	}

	//Window
	//하위 컨트롤에서 사용 안되었음
	if( nRetValue == F3dZEditableControl::_RET_CTL_NONE )
	{
		if( IsInRect( x, y ) )
		{
			switch( uMsg )	
			{
			case WM_LBUTTONDOWN:
				if( m_pWndManager->SetCapture(this) )
				{
					m_ptMovingOffset.x = x - m_rect.left;
					m_ptMovingOffset.y = y - m_rect.top;
					m_bMoving = TRUE;
					m_pWndManager->SetFocus(this);
				}
				break;

			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
				m_pWndManager->SetFocus(this);
				break;

			case WM_LBUTTONUP:
				m_bMoving = FALSE;
				m_pWndManager->ReleaseCapture(this);
				break;
			}

			nRetValue = _RET_WND_USED; //윈도우에서 사용됨.
		}
		else
		{
			switch( uMsg )	
			{
			case WM_LBUTTONUP:
				if( m_bMoving )
				{
					m_bMoving = FALSE;
					m_pWndManager->ReleaseCapture(this);
				}
			}

			nRetValue = _RET_WND_NONE;
		}
	}

	return nRetValue;
}

void F3dZEditableWnd::GroupSave()
{  
	const char win_base[] = "<?xml version=\"1.0\"?><window></window>";
	TiXmlDocument doc;
	doc.Parse( win_base );

	TiXmlNode* node = 0;
	TiXmlElement* todoElement = 0;

	node = doc.FirstChild( "window" );
	assert( node );
	if( !node )
		return;

	todoElement = node->ToElement();
	assert( todoElement  );
	if( !todoElement )
		return;

	std::string fileName;
	char temp[_MAX_PATH] = {0,};
	WC2AC( temp, _MAX_PATH, L"template", (int)wcslen(L"template")+1 );

	fileName = temp;
	fileName += ".xml";

	UI_RECT ui_rect;
	std::string	strFontFlag, strFontStyle;
	//컨트롤
	for( UINT i(0); m_vGroupList.size()>i; ++i )
	{
		F3dZEditableControl* pControl = m_vGroupList[i];

		TiXmlElement ctrl( "control" );

		WC2AC( temp, _MAX_PATH, pControl->GetID(), (int)wcslen(pControl->GetID())+1 );
		ctrl.SetAttribute( "id"     , temp               );

		ctrl.SetAttribute( "caption", pControl->GetCaptionID() );
		ctrl.SetAttribute( "tooltip", pControl->GetToolTipID() );

		WC2AC( temp, _MAX_PATH, pControl->GetSpriteName(), (int)wcslen(pControl->GetSpriteName())+1 );
		ctrl.SetAttribute( "ani"    , temp );

		strFontStyle = "";
		if( pControl->IsPass()    ) strFontStyle += "st_pass|";		
		if( pControl->IsCapture() ) strFontStyle += "st_capture|";
		if( pControl->IsDrag()    ) strFontStyle += "st_drag|";
		if( pControl->IsMsgSkip() ) strFontStyle += "st_msg_skip|";
		if( pControl->IsToolTip() ) strFontStyle += "st_tooltip|";
		if( pControl->IsNone()	  ) strFontStyle = "st_none|";

		ctrl.SetAttribute( "style"  , strFontStyle.c_str() );

		WC2AC( temp, _MAX_PATH, pControl->GetFontName(), (int)wcslen(pControl->GetFontName())+1 );	
		ctrl.SetAttribute( "font_name"  , temp      );
		ctrl.SetAttribute( "font_size"  , pControl->GetFontSize() );

		strFontFlag = "";
		if( pControl->GetAlignLeft()    ) strFontFlag += "al_left|";
		if( pControl->GetAlignTop()     ) strFontFlag += "al_top|";
		if( pControl->GetAlignRight()   ) strFontFlag += "al_right|";
		if( pControl->GetAlignBottom()  ) strFontFlag += "al_bottom|";
		if( pControl->GetAlignCenter()  ) strFontFlag += "al_center|";
		if( pControl->GetAlignVCenter() ) strFontFlag += "al_vcenter|";

		if( pControl->GetItalic()  ) strFontFlag += "ft_italic|"; 
		if( pControl->GetBold()    ) strFontFlag += "ft_bold|";   
		if( pControl->GetUnder()   ) strFontFlag += "ft_under|";  
		if( pControl->GetStriker() ) strFontFlag += "ft_strike|"; 
		if( pControl->GetInverse() ) strFontFlag += "ft_inverse|";
		if( pControl->GetShadow()  ) strFontFlag += "ft_shadow|"; 
		if( pControl->GetOutLine() ) strFontFlag += "ft_outline|";
		if( pControl->GetGlow()    ) strFontFlag += "ft_glow";   

		ctrl.SetAttribute( "font_flag"  , strFontFlag.c_str() );

		pControl->GetRect( ui_rect );

		ctrl.SetAttribute( "x"      , ui_rect.left        );
		ctrl.SetAttribute( "y"      , ui_rect.top         );
		ctrl.SetAttribute( "width"  , ui_rect.GetWidth()  ); 
		ctrl.SetAttribute( "height" , ui_rect.GetHeight() );

		todoElement->InsertEndChild( ctrl );
	}

	FILE* pOut = NULL;
	fopen_s( &pOut, fileName.c_str(), "wt+" );
	if( pOut )
	{
		doc.Print( pOut );
		fclose( pOut );
	}
}

void F3dZEditableWnd::Save()
{
	 
	const char win_base[] = "<?xml version=\"1.0\"?><window></window>";
	TiXmlDocument doc;
	doc.Parse( win_base );

	TiXmlNode* node = 0;
	TiXmlElement* todoElement = 0;

	node = doc.FirstChild( "window" );
	assert( node );
	if( !node )
		return;

	todoElement = node->ToElement();
	assert( todoElement );
	if( !todoElement )
		return;
	
	std::string fileName;
	char temp[_MAX_PATH] = {0,};
	WC2AC( temp, _MAX_PATH, GetID(), (int)wcslen(GetID())+1 );

	fileName = temp;
	fileName += ".xml";

	todoElement->SetAttribute( "id"     , temp );
	todoElement->SetAttribute( "style"  , "default" );
	todoElement->SetAttribute( "caption", 0 );

	TiXmlElement pos( "pos" );
	pos.SetAttribute( "x", m_rect.left );
	pos.SetAttribute( "y", m_rect.top  );

	TiXmlElement size( "size" );
	size.SetAttribute( "width" , m_rect.GetWidth()  );
	size.SetAttribute( "height", m_rect.GetHeight() );

	todoElement->InsertEndChild( pos );
	todoElement->InsertEndChild( size );

	UI_RECT ui_rect;
	std::string	strFontFlag, strFontStyle;
	//컨트롤
	for( UINT i(0); m_vControlList.size()>i; ++i )
	{
		F3dZEditableControl* pControl = m_vControlList[i];

		TiXmlElement ctrl( "control" );

		WC2AC( temp, _MAX_PATH, pControl->GetID(), (int)wcslen(pControl->GetID())+1 );
		ctrl.SetAttribute( "id"     , temp               );

		ctrl.SetAttribute( "caption", pControl->GetCaptionID() );
		ctrl.SetAttribute( "tooltip", pControl->GetToolTipID() );

		WC2AC( temp, _MAX_PATH, pControl->GetSpriteName(), (int)wcslen(pControl->GetSpriteName())+1 );
		ctrl.SetAttribute( "ani"    , temp );
		
		strFontStyle = "";
		if( pControl->IsPass()    ) strFontStyle += "st_pass|";		
		if( pControl->IsCapture() ) strFontStyle += "st_capture|";
		if( pControl->IsDrag()    ) strFontStyle += "st_drag|";
		if( pControl->IsMsgSkip() ) strFontStyle += "st_msg_skip|";
		if( pControl->IsToolTip() ) strFontStyle += "st_tooltip|";
		if( pControl->IsNone()	  ) strFontStyle = "st_none|";

		ctrl.SetAttribute( "style"  , strFontStyle.c_str() );

		WC2AC( temp, _MAX_PATH, pControl->GetFontName(), (int)wcslen(pControl->GetFontName())+1 );	
		ctrl.SetAttribute( "font_name"  , temp      );
		ctrl.SetAttribute( "font_size"  , pControl->GetFontSize() );

		strFontFlag = "";
		if( pControl->GetAlignLeft()    ) strFontFlag += "al_left|";
		if( pControl->GetAlignTop()     ) strFontFlag += "al_top|";
		if( pControl->GetAlignRight()   ) strFontFlag += "al_right|";
		if( pControl->GetAlignBottom()  ) strFontFlag += "al_bottom|";
		if( pControl->GetAlignCenter()  ) strFontFlag += "al_center|";
		if( pControl->GetAlignVCenter() ) strFontFlag += "al_vcenter|";

		if( pControl->GetItalic()  ) strFontFlag += "ft_italic|"; 
		if( pControl->GetBold()    ) strFontFlag += "ft_bold|";   
		if( pControl->GetUnder()   ) strFontFlag += "ft_under|";  
		if( pControl->GetStriker() ) strFontFlag += "ft_strike|"; 
		if( pControl->GetInverse() ) strFontFlag += "ft_inverse|";
		if( pControl->GetShadow()  ) strFontFlag += "ft_shadow|"; 
		if( pControl->GetOutLine() ) strFontFlag += "ft_outline|";
		if( pControl->GetGlow()    ) strFontFlag += "ft_glow";   

		ctrl.SetAttribute( "font_flag"  , strFontFlag.c_str() );

		pControl->GetRect( ui_rect );

		ctrl.SetAttribute( "x"      , ui_rect.left        );
		ctrl.SetAttribute( "y"      , ui_rect.top         );
		ctrl.SetAttribute( "width"  , ui_rect.GetWidth()  ); 
		ctrl.SetAttribute( "height" , ui_rect.GetHeight() );

		if( pControl->GetFontColor() != 0xFFFFFFFF ) {

			std::string strColor = MBStringUtil::StringFormat( "%x", pControl->GetFontColor() );
			ctrl.SetAttribute( "color"   , strColor.c_str() );
		}

		if( pControl->GetFontBGColor() != 0xFF000000 ) {
			std::string strColorBG = MBStringUtil::StringFormat( "%x", pControl->GetFontBGColor() );
			ctrl.SetAttribute( "bg_color", strColorBG.c_str() );
		}

		todoElement->InsertEndChild( ctrl );
	}

	FILE* pOut = NULL;
	fopen_s( &pOut, fileName.c_str(), "wt+" );
	if( pOut )
	{
		doc.Print( pOut );
		fclose( pOut );
	}
}

void F3dZEditableWnd::ChangeOrderUp( const WCHAR* pID )
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		if( !_wcsicmp( pControl->GetID(), pID ) && it != m_vControlList.begin() )
		{
			it = m_vControlList.erase( it );

			m_vControlList.insert( it-1, pControl );
			break;
		}
	}
}

void F3dZEditableWnd::ChangeOrderDn( const WCHAR* pID )
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		if( !_wcsicmp( pControl->GetID(), pID ) )
		{
			it = m_vControlList.erase( it );
			if( it != m_vControlList.end() )
				m_vControlList.insert( it+1, pControl );
			else
				m_vControlList.push_back( pControl );
			break;
		}
	}
}

F3dZEditableControl*  F3dZEditableWnd::GetCheckControl()
{  
	std::vector<F3dZEditableControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZEditableControl* pControl = (*it);
		if( pControl->IsCheck() )
			return pControl;
	}

	return NULL;
}


#include "FeatherXEnginePrecompile.h"

HWND	  F3dZEditableWndManager::s_hWnd      = NULL;
HINSTANCE F3dZEditableWndManager::s_hInstance = NULL;

F3dZEditableWndManager::F3dZEditableWndManager() : m_pUIHandler(NULL), m_pCaptureWnd(NULL), m_pFocusWnd(NULL)
{  

}

F3dZEditableWndManager::~F3dZEditableWndManager()
{  
	std::list< F3dZEditableWnd* >::iterator it;
	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		delete pWnd;
	}
}

void F3dZEditableWndManager::Process( double dTime, double dMeanDT )
{  
	std::list< F3dZEditableWnd* >::iterator it;
	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Process( dTime, dMeanDT );
	}
}

void F3dZEditableWndManager::Render( double dTime )
{  
	std::list< F3dZEditableWnd* >::iterator it;
	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Render( dTime );
	}
}

BOOL F3dZEditableWndManager::CreateWnd( const WCHAR* pFileName, F3dZEditableWnd* pWnd )
{  
	BOOL bRet = F3dZEditableUIFactory::GetSingleton()->CreateWnd( pFileName, pWnd );

	if( bRet )
	{
		boost::unordered_map< std::wstring, F3dZEditableWnd* >::iterator it = m_vWndHash.find( pWnd->GetID() );
		if( it != m_vWndHash.end() )
		{
			assert( 0 && "It exists already Window" );
			return FALSE;
		}

		m_vWndHash.insert( std::make_pair( pWnd->GetID(), pWnd ) ); 
		m_listWnd.push_back( pWnd );
	}

	return bRet;
}

BOOL F3dZEditableWndManager::AddWnd( F3dZEditableWnd* pWnd )
{  
	boost::unordered_map< std::wstring, F3dZEditableWnd* >::iterator it = m_vWndHash.find( pWnd->GetID() );
	if( it != m_vWndHash.end() )
	{
		assert( 0 && "It exists already Window" );
		return FALSE;
	}

	m_vWndHash.insert( std::make_pair( pWnd->GetID(), pWnd ) ); 
	m_listWnd.push_back( pWnd );

	return TRUE;
}

F3dZEditableWnd* F3dZEditableWndManager::GetWnd( const WCHAR* pID )
{  
	boost::unordered_map< std::wstring, F3dZEditableWnd* >::iterator it = m_vWndHash.find( pID ); 
	if( it != m_vWndHash.end() )
	{
#ifdef _DEBUG
	F3dZEditableWnd* pWnd = it->second;
#endif	
		return it->second;
	}

	return NULL;
}

void F3dZEditableWndManager::CheckGroup( int nX, int nY, UI_RECT& rect )
{  
	std::list< F3dZEditableWnd* >::iterator it;
	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		if( pWnd->IsShow() ) {
			pWnd->CheckGroup( nX, nY, rect );
			return;
		}
	}
}

BOOL  F3dZEditableWndManager::ShowWnd( const WCHAR* pID, bool bShowMode )
{  
	F3dZEditableWnd* pWnd = GetWnd( pID );
	if( pWnd )
	{
		pWnd->Show( bShowMode );
		return TRUE;
	}

	return FALSE;
}

//Msg Box : Modal
//Msg Box : Modalless

//Capture : Only One
BOOL F3dZEditableWndManager::SetCapture(F3dZEditableWnd* pWnd)
{
	if( pWnd && NULL == m_pCaptureWnd )
	{
		m_pCaptureWnd = pWnd;
		return TRUE;
	}

	return FALSE;
}

void F3dZEditableWndManager::ReleaseCapture(F3dZEditableWnd* pWnd)
{
	if( m_pCaptureWnd == pWnd )
		m_pCaptureWnd = NULL;
}

//Focus
void F3dZEditableWndManager::SetFocus(F3dZEditableWnd* pWnd)
{
	m_pFocusWnd = pWnd;

	if( NULL != pWnd )
	{	//최상위로 올려줌
		std::list< F3dZEditableWnd* >::iterator it = std::find( m_listWnd.begin(), m_listWnd.end(), pWnd );
		if( it != m_listWnd.end() )
		{
			it = m_listWnd.erase( it ); //중간에 공백이 발생됨
			m_listWnd.push_back( pWnd );
		}
	}
}

F3dZEditableWnd* F3dZEditableWndManager::GetFocusWnd()
{  
	return m_pFocusWnd;
}

BOOL F3dZEditableWndManager::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{  	
	switch( uMsg )
	{
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		::SetCapture( s_hWnd );
		break;
	}

	switch( uMsg )
	{
		case WM_LBUTTONDBLCLK: 
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:	
			{
				short x = LOWORD(lParam);
				short y = HIWORD(lParam);

					_mouseMsg( uMsg, x, y );
			}
			break;
		case WM_MOUSEWHEEL:
			{
				POINT pt; pt.x = (short)LOWORD(lParam); pt.y = (short)HIWORD(lParam);
				ScreenToClient( s_hWnd, &pt );
				short z = HIWORD(wParam);
				if(z > 0) 
					return TRUE;
				else
					return TRUE;
			}
			break;
		case WM_KEYUP:
		case WM_KEYDOWN:
			{
				_keyMsg( uMsg, wParam, lParam );
			}
			break;

		case WM_ACTIVATEAPP:
			if(wParam == TRUE)
			{
				::SetFocus( s_hWnd );
			}
			else
			{
				//KillFocus();
			}
			break;
	}

	return FALSE;
}

BOOL F3dZEditableWndManager::_mouseMsg( UINT uMsg, const int& x, const int& y )
{  
	//모든 윈도우
	int nRetVal = F3dZEditableWnd::_RET_WND_NONE;

	//캡쳐 윈도우
	if( m_pCaptureWnd )
	{
		m_pCaptureWnd->OnMouseMsg( uMsg, x, y, nRetVal );
		return TRUE;
	}

	std::list< F3dZEditableWnd* >::reverse_iterator it;
	std::list< F3dZEditableWnd* > wndList = m_listWnd; //
	for ( it = wndList.rbegin(); it != wndList.rend() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			nRetVal |= pWnd->OnMouseMsg( uMsg, x, y, nRetVal );
	}

	//
	if( WM_MOUSEMOVE != uMsg && nRetVal == F3dZEditableWnd::_RET_WND_NONE )
		SetFocus( NULL );

	return FALSE;
}

BOOL F3dZEditableWndManager::_keyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{  
	if( m_pFocusWnd )
	{
		m_pFocusWnd->OnKeyMsg( uMsg, wParam, lParam );
		return TRUE;
	}

	std::list< F3dZEditableWnd* >::reverse_iterator it;
	for ( it = m_listWnd.rbegin(); it != m_listWnd.rend() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->OnKeyMsg( uMsg, wParam, lParam );
	}

	return FALSE;
}

void F3dZEditableWndManager::OnKeyDown( int nKey )
{  
	F3dZEditableControl* pControl = NULL;

	std::list< F3dZEditableWnd* >::reverse_iterator it;
	for ( it = m_listWnd.rbegin(); it != m_listWnd.rend() ; ++it )
	{
		F3dZEditableWnd* pWnd = (*it);
		if( pWnd->IsShow() ) {
			pControl = pWnd->GetCheckControl();
			break;
		}
	}

	if( pControl == NULL ) return;

	if( nKey == 37 ) { //Left	
		pControl->MovePos( -1, 0 );
	}
	else if( nKey == 38 ) { //Up
		pControl->MovePos( 0, -1 );
	}
	else if( nKey == 39 ) { //Right
		pControl->MovePos( 1, 0 );
	}
	else if( nKey == 40 ) { //Down
		pControl->MovePos( 0, 1 );
	}
}
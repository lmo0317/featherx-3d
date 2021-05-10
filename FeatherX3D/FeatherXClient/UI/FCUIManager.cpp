#include "FeatherXClientPrecompile.h"

namespace
{
	BOOL g_bKeyOn = FALSE;
	BOOL g_bKeyProc = FALSE;
}

FCUIManager::FCUIManager():
m_zWndManger( new F3dZWndManager )
{

}

FCUIManager::~FCUIManager()
{

}

void FCUIManager::Init()
{
	m_zWndManger->Init();
	m_zWndManger->SetHandler( this );

	if( !m_pUIGameMain.get() ) {
		FCUIGameMain* pWnd = new FCUIGameMain( m_zWndManger.get() );
		if( !m_zWndManger->CreateWnd( L"ui\\window\\main_event.xml", pWnd, true ))
			delete pWnd;	
		else {
			pWnd->Init();
			pWnd->RePosition();
			m_pUIGameMain.reset( pWnd );
		}
	}

	if( !m_pUIStart.get() ) {
		FCUIStart* pWnd = new FCUIStart( m_zWndManger.get() );
		if( !m_zWndManger->CreateWnd( L"ui\\window\\start.xml", pWnd, true ))
			delete pWnd;	
		else {
			pWnd->Init();
			pWnd->RePosition();
			m_pUIStart.reset( pWnd );
		}
	}

	if( !m_pUIEnd.get() ) {
		FCUIEnd* pWnd = new FCUIEnd( m_zWndManger.get() );
		if( !m_zWndManger->CreateWnd( L"ui\\window\\end.xml", pWnd, true ))
			delete pWnd;	
		else {
			pWnd->Init();
			pWnd->RePosition();
			m_pUIEnd.reset( pWnd );
		}
	}

	FCUIManager::GetSingleton()->ShowWindow(L"main_event",TRUE);
	FCUIManager::GetSingleton()->ShowWindow(L"start",TRUE);
}

void FCUIManager::ShowWindow( const WCHAR* szName, BOOL bShow, BOOL bFocus/*=TRUE*/ )
{
	if( m_zWndManger->ShowWnd( szName, bShow, bFocus ) == FALSE ) {
	}
}

void FCUIManager::Process( double dTime, double dMeanDT )
{
	m_zWndManger->Process( dTime, dMeanDT );
}

void FCUIManager::Render( double dTime, double dMeanDT )
{
	m_zWndManger->Render( dTime, dMeanDT );
}

BOOL FCUIManager::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	UINT nChar = (UINT)wParam;
	switch( uMsg ) {
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:

		if( nChar == VK_UP || nChar == VK_DOWN || nChar == VK_LEFT || nChar == VK_RIGHT )
			break;

		if( g_bKeyOn == FALSE )
		{
			g_bKeyOn = TRUE;
			g_bKeyProc = TRUE;
			break;
		}
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		g_bKeyOn = FALSE;
		break;
	}

	BOOL bRet = m_zWndManger->MsgProc( uMsg, wParam, lParam );

	if( bRet && g_bKeyOn ) {
		g_bKeyOn = FALSE;
		g_bKeyProc = FALSE;
	}

	return bRet;
}
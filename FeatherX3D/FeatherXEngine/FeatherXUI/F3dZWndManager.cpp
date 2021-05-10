#include "FeatherXEnginePrecompile.h"
#include "MBStringUtill.h"
#include <algorithm>
#include <cctype>
#include "MagiFileSystem.h"

using namespace MBStringUtil;

HWND	  F3dZWndManager::s_hWnd      = NULL;
HINSTANCE F3dZWndManager::s_hInstance = NULL;
BOOL F3dZWndManager::s_bLeftButton  = FALSE;
BOOL F3dZWndManager::s_bRightButton = FALSE;
INT	F3dZWndManager::s_nX = 0;
INT	F3dZWndManager::s_nY = 0;

namespace 
{
	const USHORT c_nCURSOR_SIZE = 32;

	const double c_dDelaySortWnd = 0.07;
};

F3dZWndManager::F3dZWndManager() : 
m_pUIHandler(NULL), 
m_pCaptureWnd(NULL), 
m_pFocusWnd(NULL), 	
m_pDragObject(NULL), 
m_pDragRender(NULL), 
m_pLastToolTip(NULL), 
m_nActToolTip(0), 
m_bIsMsgBoxShow(FALSE),
m_dCheckDelayTime(0.),
m_dDelaySortedWnd(c_dDelaySortWnd)
{ 
	 
	m_ptDragRenderOffset.x = 0;
	m_ptDragRenderOffset.y = 0;

	for( int i(0); TOOLTIP_MAX>i; ++i )
		m_pToolTip[i] = NULL;
}

F3dZWndManager::~F3dZWndManager()
{ 
	 
}

void F3dZWndManager::Init()
{ 
	 
}

void F3dZWndManager::Finish()
{ 
	 

	for( int i(0); TOOLTIP_MAX>i; ++i )
		FBDEL(m_pToolTip[i]);
}

void F3dZWndManager::AllRePosition()
{ 
	 
	std::list< F3dZWnd* >::iterator it;

	//모달
	for ( it = m_listModal.begin(); it != m_listModal.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		pWnd->RePosition();
	}

	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		pWnd->RePosition();
	}
}	

void F3dZWndManager::ShowAllWindow( BOOL bShowMode )
{ 
	std::list< F3dZWnd* >::iterator it;
	//Sort
	for ( it = m_listSortWnd.begin(); it != m_listSortWnd.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		pWnd->Show( bShowMode );
	}

	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		pWnd->Show( bShowMode );
	}

	//ToolTip

	if( m_pToolTip[m_nActToolTip] )
		m_pToolTip[m_nActToolTip]->SetShow( bShowMode );
}

void F3dZWndManager::Process( double dTime, double dMeanDT )
{
	std::list< F3dZWnd* >::iterator it;

	//모달
	for ( it = m_listModal.begin(); it != m_listModal.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Process( dTime, dMeanDT );
	}

	std::list< F3dZWnd* > listSort = m_listSortWnd; //
	//Sort
	for ( it = listSort.begin(); it != listSort.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Process( dTime, dMeanDT );
	}

	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Process( dTime, dMeanDT );

		if( !_wcsicmp( pWnd->GetID(), L"map" ) || !_wcsicmp( pWnd->GetID(), L"minimap" ) )
			pWnd->Process( dTime, dMeanDT );
	}

	//ToolTip

	if( m_pToolTip[m_nActToolTip] )
		m_pToolTip[m_nActToolTip]->Process( dTime, dMeanDT );

	//Delay SortWnd
	if( dTime - m_dCheckDelayTime >= m_dDelaySortedWnd )
	{
		m_dCheckDelayTime = dTime;
		if( !m_qDelaySortWnd.empty() ) {
			F3dZWnd* pDelayWnd = m_qDelaySortWnd.front();
			addSortWnd( pDelayWnd );
			m_qDelaySortWnd.pop();

			boost::unordered_map< std::wstring, F3dZWnd* >::iterator it2 = m_vDelaySortWndHash.find( pDelayWnd->GetID() ); 
			if( it2 != m_vDelaySortWndHash.end() )
				m_vDelaySortWndHash.erase( it2 );
		}
	}
}

void F3dZWndManager::SetDragRenderPos( int nX, int nY )
{
	ZUIDragAndDropRender::SetPosition( nX - m_ptDragRenderOffset.x, nY - m_ptDragRenderOffset.y );
}

bool WndDataSortPredicate(F3dZWnd* p1, F3dZWnd* p2)
{
	F3dVector cameraPos = *CFeatherXCamera::GetSingleton()->GetEyeVector();

	float fLength1 = VecLength(cameraPos - p1->GetCenter());
	float fLength2 = VecLength(cameraPos - p2->GetCenter());
	return (fLength1 > fLength2);
}

const bool F3dZWndManager::IsInRectAll(const int nX,const int nY)
{
	std::list< F3dZWnd* >::iterator it;
	std::list< F3dZWnd* > listSort = m_listSortWnd; //

	for ( it = listSort.begin(); it != listSort.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() && pWnd->IsInRect(nX,nY) && pWnd->IsWndFlag(F3dZWnd::_WND_SKIP_3D_CHECK) == false)
		{
			return true;
		}
	}

	//일반
	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() && pWnd->IsInRect(nX,nY) && pWnd->IsWndFlag(F3dZWnd::_WND_SKIP_3D_CHECK) == false)
		{
			return true;
		}
	}

	//모달
	for ( it = m_listModal.begin(); it != m_listModal.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() && pWnd->IsInRect(nX,nY) && pWnd->IsWndFlag(F3dZWnd::_WND_SKIP_3D_CHECK) == false)
		{
			return true;
		}
	}

	//Drag
	/*
	if( m_pDragRender ) {
		m_pDragRender->Render( dTime );
	}
	else {
		if( !m_bIsMsgBoxShow )
		{
			if( m_pToolTip[m_nActToolTip] )
				m_pToolTip[m_nActToolTip]->Render( dTime );
		}
	}
	*/

	return false;
}

void F3dZWndManager::Render( double dTime, double dMeanDT )
{ 
	std::list< F3dZWnd* >::iterator it;
	std::list< F3dZWnd* > listSort = m_listSortWnd; //
	
	//카메라 거리 기준으로 소팅
	listSort.sort(WndDataSortPredicate);
	for ( it = listSort.begin(); it != listSort.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Render( dTime, dMeanDT );
	}

	//일반
	for ( it = m_listWnd.begin(); it != m_listWnd.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Render( dTime, dMeanDT );
	}

	//모달
	for ( it = m_listModal.begin(); it != m_listModal.end() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			pWnd->Render( dTime, dMeanDT );
	}

	//Drag
	if( m_pDragRender ) {
		m_pDragRender->Render( dTime );
	}
	else {
		if( !m_bIsMsgBoxShow )
		{
			if( m_pToolTip[m_nActToolTip] )
				m_pToolTip[m_nActToolTip]->Render( dTime );
		}
	}
}

void F3dZWndManager::OnMsgBoxNotify( const WCHAR* winID, _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* pID, const WCHAR* pExtraMsg /*= NULL*/ )
{
	if( m_pUIHandler )
		m_pUIHandler->OnMsgBoxNotify( winID, notify_msg, pID, pExtraMsg );
}

void F3dZWndManager::OpenMessageWnd( F3dZWnd* pWnd, WINDOW_ARG& window_arg, USHORT nMsgBoxID, MSG_TYPE msgType, bool bModal, std::vector<int>& captionList, float fKeepTime )
{ 
	pWnd->CreateWnd( window_arg );
	if( window_arg.vControlList.size() > 2 ) {
		if(captionList.empty())
		{
			_MBASSERT(captionList.empty());
			return;
		}
		int nPosX   = window_arg.vControlList[0].x;
		int nHalfW  = window_arg.vControlList[0].nWidth/2;//window_arg.window_info.nWidth/2;
		int nThirdW = window_arg.vControlList[0].nWidth/3;//window_arg.window_info.nWidth/3;

		window_arg.vControlList[2].nCaptionID = captionList[0];

		F3dZControl* pBG00   = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[0] );
		F3dZControl* pBG01   = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[1] );
		F3dZControl* pMsgTxt = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[2] );
		F3dZControl* pMsgTxt_1 = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[4] );
		F3dZControl* pMsgTxt_2 = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[5] );
		F3dZControl* pMsgTxt_3 = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[6] );
		F3dZControl* pMsgTxt_4 = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[7] );

		F3dZControl* pEditBG   = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[8] );
		F3dZControl* pEditMsg  = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[9] );
		F3dZControl* pEdit     = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[10] );


		if( pBG00     ) pWnd->AddControl( pBG00 );
		if( pBG01	  ) pWnd->AddControl( pBG01 );

		if( pMsgTxt ) {
			pWnd->AddControl( pMsgTxt );
		}
		if( pMsgTxt_1 ) {
			pWnd->AddControl( pMsgTxt_1 );
		}
		if( pMsgTxt_2 ) {
			pWnd->AddControl( pMsgTxt_2 );
		}
		if( pMsgTxt_3 ) {
			pWnd->AddControl( pMsgTxt_3 );
		}
		if( pMsgTxt_4 ) {
			pWnd->AddControl( pMsgTxt_4 );
		}

		if( pEditBG  ) {
			pWnd->AddControl( pEditBG );
		}

		if( pEditMsg ) {
			pWnd->AddControl( pEditMsg );
		}
		
		if( pEdit ) {
			pWnd->AddControl( pEdit );
		}

		if( msgType == TYPE_OK          )	 {
			  CONTROL_ARG ctrl_arg    = window_arg.vControlList[3];

			  ctrl_arg.ID = L"ok";
			  ctrl_arg.nCaptionID = captionList[1];
			  ctrl_arg.x    = nPosX + nHalfW-(ctrl_arg.nWidth/2);

			  F3dZControl* pBtn00 = F3dZUIFactory::GetSingleton()->CreateControl( ctrl_arg );
			  if( pBtn00 ) pWnd->AddControl( pBtn00 );
		}
		else if( msgType == TYPE_OKCANCEL    )  {
			CONTROL_ARG ctrl_arg1 = window_arg.vControlList[3];
			CONTROL_ARG ctrl_arg2 = window_arg.vControlList[3];

			ctrl_arg1.ID = L"ok";
			ctrl_arg2.ID = L"cancel";
			
			ctrl_arg1.nCaptionID = captionList[1];
			ctrl_arg2.nCaptionID = captionList[2];

			ctrl_arg1.x = nPosX + nHalfW-(ctrl_arg1.nWidth)-ctrl_arg1.nWidth/2;
			ctrl_arg2.x = nPosX + nHalfW+ctrl_arg2.nWidth/2;

			F3dZControl* pBtn00 = F3dZUIFactory::GetSingleton()->CreateControl( ctrl_arg1 );
			F3dZControl* pBtn01 = F3dZUIFactory::GetSingleton()->CreateControl( ctrl_arg2 );
			if( pBtn00 ) pWnd->AddControl( pBtn00 );
			if( pBtn01 ) pWnd->AddControl( pBtn01 );
		}
		//else if( msgType == TYPE_YESNOCANCEL )  {
		//	CONTROL_ARG ctrl_arg1 = window_arg.vControlList[9];
		//	CONTROL_ARG ctrl_arg2 = window_arg.vControlList[9];
		//	CONTROL_ARG ctrl_arg3 = window_arg.vControlList[9];

		//	ctrl_arg1.nCaptionID = captionList[1];
		//	ctrl_arg2.nCaptionID = captionList[2];
		//	ctrl_arg3.nCaptionID = captionList[3];

		//	//ctrl_arg1.nWidth = nThirdW - 20;
		//	//ctrl_arg2.nWidth = nThirdW - 20;
		//	//ctrl_arg3.nWidth = nThirdW - 20;

		//	ctrl_arg1.ID = L"yes";
		//	ctrl_arg2.ID = L"no";
		//	ctrl_arg3.ID = L"cancel";

		//	ctrl_arg1.x = nThirdW/2-(ctrl_arg1.nWidth/2);
		//	ctrl_arg2.x = (nThirdW+nThirdW/2)-(ctrl_arg2.nWidth/2);
		//	ctrl_arg3.x = (nThirdW+nThirdW+nThirdW/2)-(ctrl_arg3.nWidth/2);

		//	F3dZControl* pBtn00 = F3dZUIFactory::GetSingleton()->CreateControl( ctrl_arg1 );
		//	F3dZControl* pBtn01 = F3dZUIFactory::GetSingleton()->CreateControl( ctrl_arg2 );
		//	F3dZControl* pBtn02 = F3dZUIFactory::GetSingleton()->CreateControl( ctrl_arg3 );
		//	if( pBtn00 ) pWnd->AddControl( pBtn00 );
		//	if( pBtn01 ) pWnd->AddControl( pBtn01 );
		//	if( pBtn02 ) pWnd->AddControl( pBtn02 );
		//}
	}

	if( bModal )
		addModalWnd( pWnd );
	else
		addWnd( pWnd );
}

void F3dZWndManager::AddWnd( F3dZWnd* pWnd, bool bModal )
{ 
	 
	if( bModal )
		addModalWnd( pWnd );
	else
		addWnd( pWnd );
}

void F3dZWndManager::addModalWnd( F3dZWnd* pWnd )
{ 
	 
	m_vModalHash.insert( std::make_pair( pWnd->GetID(), pWnd ) ); 
	m_listModal.push_back( pWnd );
}

void F3dZWndManager::addWnd( F3dZWnd* pWnd )
{ 
	 
	m_vWndHash.insert( std::make_pair( pWnd->GetID(), pWnd ) ); 
	m_listWnd.push_back( pWnd );
}

void F3dZWndManager::addSortWnd( F3dZWnd* pWnd )
{ 
	 
//#ifdef _DEBUG
	F3dZWnd* pExistWnd = GetSortWnd( pWnd->GetID() );
	if( pExistWnd ) {
		return;
	}
//#endif

	m_vSortWndHash.insert( std::make_pair( pWnd->GetID(), pWnd ) ); 
	m_listSortWnd.push_back( pWnd );
}

BOOL F3dZWndManager::CreateChildWnd( const WCHAR* pFileName, F3dZWnd* pWnd )
{ 
	 

#ifdef _DEBUG
//	LOGOUT( L"최적화대상:CreateChildWnd %s", pFileName );
#endif

	std::wstring strName = pFileName;
if( MagiFileSystem::IsPackedFile() ) {
	std::transform(strName.begin() , strName.end() , strName.begin() , (int(*)(int)) std::tolower);
}

	BOOL bRet = F3dZUIFactory::GetSingleton()->CreateWnd( strName.c_str(), pWnd );

	if( bRet )	{
		boost::unordered_map< std::wstring, F3dZWnd* >::iterator it = m_vWndHash.find( pWnd->GetID() );
		if( it != m_vWndHash.end() )	{
			assert( 0 && "It exists already Window" );
			return FALSE;
		}
	}

	return bRet;
}

BOOL F3dZWndManager::CreateWnd( const WCHAR* pFileName, F3dZWnd* pWnd, bool bModal /*= false*/ )
{ 
	 
	std::wstring strName = pFileName;
if( MagiFileSystem::IsPackedFile() ) {
	std::transform(strName.begin() , strName.end() , strName.begin() , (int(*)(int)) std::tolower);
}

	BOOL bRet = CreateChildWnd( strName.c_str(), pWnd );
	if( bRet )	{
		if( bModal )
			addModalWnd( pWnd );
		else
			addWnd( pWnd );
	}

	return bRet;
}

F3dZWnd* F3dZWndManager::GetDelaySortWnd( const WCHAR* pID )
{
	 
	boost::unordered_map< std::wstring, F3dZWnd* >::iterator it = m_vDelaySortWndHash.find( pID ); 
	if( it != m_vDelaySortWndHash.end() )
	{
#ifdef _DEBUG
		F3dZWnd* pWnd = it->second;
#endif	
		return it->second;
	}

	return NULL;
}

F3dZWnd* F3dZWndManager::GetSortWnd( const WCHAR* pID )
{ 
	 
	boost::unordered_map< std::wstring, F3dZWnd* >::iterator it = m_vSortWndHash.find( pID ); 
	if( it != m_vSortWndHash.end() )
	{
#ifdef _DEBUG
		F3dZWnd* pWnd = it->second;
#endif	
		return it->second;
	}

	return NULL;
}

F3dZWnd* F3dZWndManager::GetModalWnd( const WCHAR* pID )
{ 
	 
	boost::unordered_map< std::wstring, F3dZWnd* >::iterator it = m_vModalHash.find( pID ); 
	if( it != m_vModalHash.end() )
	{
#ifdef _DEBUG
		F3dZWnd* pWnd = it->second;
#endif	
		return it->second;
	}

	return NULL;
}

F3dZWnd* F3dZWndManager::GetWnd( const WCHAR* pID )
{ 
	 
	boost::unordered_map< std::wstring, F3dZWnd* >::iterator it = m_vWndHash.find( pID ); 
	if( it != m_vWndHash.end() )
	{
#ifdef _DEBUG
	F3dZWnd* pWnd = it->second;
#endif	
		return it->second;
	}

	return NULL;
}

void F3dZWndManager::SetWindowFlag( const WCHAR* pID, DWORD dwFlag )
{
	 

	F3dZWnd* pWnd = GetModalWnd( pID );
	if( pWnd == NULL )	pWnd = GetSortWnd( pID );
	if( pWnd == NULL )	pWnd = GetWnd( pID );
	if( pWnd == NULL )	pWnd = GetDelaySortWnd( pID );

	if( pWnd ) {
		pWnd->SetWndFlag( dwFlag );
	}
}

BOOL F3dZWndManager::SetRenderTop(const WCHAR* pID,BOOL bTop)
{
	 
	F3dZWnd* pWnd = GetModalWnd( pID );
	if( pWnd == NULL )	pWnd = GetSortWnd( pID );
	if( pWnd == NULL )	pWnd = GetWnd( pID );
	if( pWnd == NULL )	pWnd = GetDelaySortWnd( pID );

	if( pWnd ) 
	{
		pWnd->SetRenderTopLevel(bTop!=0);
		return TRUE;
	}

	return FALSE;

}

BOOL  F3dZWndManager::ShowWnd( const WCHAR* pID, BOOL bShowMode, BOOL bFocus )
{ 
	 
	F3dZWnd* pWnd = GetModalWnd( pID );
	if( pWnd == NULL )	pWnd = GetSortWnd( pID );
	if( pWnd == NULL )	pWnd = GetWnd( pID );
	if( pWnd == NULL )	pWnd = GetDelaySortWnd( pID );

	if( pWnd ) {
		if( bShowMode ) pWnd->SetAlpha(1.f);
		if( bShowMode && pWnd->IsShow() ) return TRUE;

		pWnd->Show( bShowMode );
	
		if( bFocus )
			SetFocus( bShowMode ? pWnd : NULL );
		return TRUE;
	}

	return FALSE;
}

void F3dZWndManager::MoveWnd( const WCHAR* pID, int nPosX, int nPosY )
{ 
	 
	F3dZWnd* pWnd = GetModalWnd( pID );
	if( pWnd == NULL )	pWnd = GetSortWnd( pID );
	if( pWnd == NULL )	pWnd = GetWnd( pID );
	if( pWnd == NULL )	pWnd = GetDelaySortWnd( pID );

	if( pWnd ) {
		pWnd->MovePos( nPosX, nPosY );

		if( pWnd->IsWndFlag(F3dZWnd::_WND_LIMIT_MOVE) ) {
			pWnd->LimitResolution();
		}
	}
}

//Msg Box : Modal
//Msg Box : Modalless

//Capture : Only One
BOOL F3dZWndManager::SetCapture(F3dZWnd* pWnd)
{ 
	 
	if( pWnd && NULL == m_pCaptureWnd )
	{
		m_pCaptureWnd = pWnd;
		return TRUE;
	}

	return FALSE;
}

void F3dZWndManager::CheckShowToolTip( const WCHAR* pID )
{
	if( m_pToolTip[m_nActToolTip] ) {
		m_pToolTip[m_nActToolTip]->CheckOwnerWindowClose( pID );
	}
}

void F3dZWndManager::ReleaseCapture(F3dZWnd* pWnd)
{ 
	 
	if( m_pCaptureWnd == pWnd ) {
		m_pCaptureWnd = NULL;
	}
}

//Focus
void F3dZWndManager::SetFocus(F3dZWnd* pWnd)
{ 
	 

	if( m_pFocusWnd == pWnd )
		return;

	m_pFocusWnd = pWnd;

	if( NULL != pWnd )
	{	//최상위로 올려줌

#ifndef _WINDY_AUTH_CHECK_
		LOGOUT( L"SetFocus %s\n", pWnd->GetID() );
#endif

		std::list< F3dZWnd* >::iterator it = std::find( m_listWnd.begin(), m_listWnd.end(), pWnd );
		if( it != m_listWnd.end() )
		{
			it = m_listWnd.erase( it ); //중간에 공백이 발생됨
			m_listWnd.push_back( pWnd );
		}
		else {
			//modal
			it = std::find( m_listModal.begin(), m_listModal.end(), pWnd );
			if( it != m_listModal.end() ) 
			{
				it = m_listModal.erase( it );
				m_listModal.push_back( pWnd );
			}
		}
	}
}

F3dZWnd* F3dZWndManager::GetFocusWnd()
{ 
	 
	return m_pFocusWnd;
}

BOOL F3dZWndManager::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
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
				s_nX = x;
				s_nY = y;

				BOOL bRet = _mouseMsg( uMsg, x, y );

				SetDragRenderPos( x, y );

				if( uMsg == WM_LBUTTONUP )	{
					s_bLeftButton  = FALSE;

					if( m_pDragRender )	{
						if( m_pDragObject && m_pDragObject->GetParent() && !m_pDragObject->GetParent()->IsInRect( x, y ) )
							m_pDragObject->GetParent()->OnDropEmpty( m_pDragObject );
						m_pDragRender = NULL;
						m_pDragObject = NULL; 
					}
				}
				else if( uMsg == WM_RBUTTONUP )
					s_bRightButton = FALSE;
				else if( uMsg == WM_LBUTTONDOWN )
					s_bLeftButton  = TRUE;
				else if( uMsg == WM_RBUTTONDOWN )
					s_bRightButton = TRUE;
		
				if( m_pToolTip[m_nActToolTip] && m_pToolTip[m_nActToolTip]->IsShow() ) {
					m_pToolTip[m_nActToolTip]->MovePos( x, y );
					m_pToolTip[m_nActToolTip]->LimitResolution();
				}

				return bRet;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				POINT pt; pt.x = (short)LOWORD(lParam); pt.y = (short)HIWORD(lParam);
				ScreenToClient( s_hWnd, &pt );
				short z = HIWORD(wParam);
//				if(z > 0) 
//					return TRUE;
//				else
//					return TRUE;
			}
			break;
		case WM_KEYUP:
		case WM_KEYDOWN:
			{
				return _keyMsg( uMsg, wParam, lParam );
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

BOOL F3dZWndManager::IsModalShow()
{
	std::list< F3dZWnd* >::reverse_iterator it;
	//모달 윈도우
	std::list< F3dZWnd* > modalList = m_listModal; //
	for ( it = modalList.rbegin(); it != modalList.rend() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() ) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL F3dZWndManager::_mouseMsg( UINT uMsg, const int& x, const int& y )
{ 
	 
	//모든 윈도우
	int nRetVal = F3dZWnd::_RET_WND_NONE;

	//캡쳐 윈도우
	if( m_pCaptureWnd )
	{
		m_pCaptureWnd->OnMouseMsg( uMsg, x, y, nRetVal );
		return TRUE;
	}

	std::list< F3dZWnd* >::reverse_iterator it;

	//모달 윈도우
	bool bModalCheck = false;
	std::list< F3dZWnd* > modalList = m_listModal; //
	for ( it = modalList.rbegin(); it != modalList.rend() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() ) {
			nRetVal |= pWnd->OnMouseMsg( uMsg, x, y, nRetVal );
			bModalCheck = true;
			break;
		}
	}
	if( bModalCheck ) return TRUE;

	//일반 윈도우
	std::list< F3dZWnd* > wndList = m_listWnd;
	for ( it = wndList.rbegin(); it != wndList.rend() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			nRetVal |= pWnd->OnMouseMsg( uMsg, x, y, nRetVal );
	}

	//
	if( WM_MOUSEMOVE != uMsg && nRetVal == F3dZWnd::_RET_WND_NONE )
		SetFocus( NULL );

	if( nRetVal != F3dZWnd::_RET_WND_NONE )
		return TRUE;

	//ToolTip Check
	if( m_pToolTip[m_nActToolTip] && m_pToolTip[m_nActToolTip]->IsShow() )
		SetToolTip( NULL, m_nActToolTip );

	return FALSE;
}

BOOL F3dZWndManager::_keyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	 
	int nRetVal = F3dZWnd::_RET_WND_NONE;

	if( m_pFocusWnd )
	{
		if( m_pFocusWnd->IsShow() ) {
			int nKeyVal = m_pFocusWnd->OnKeyMsg( uMsg, wParam, lParam, nRetVal );
			if( F3dZWnd::_RET_WND_NONE != nKeyVal )
				return TRUE;
//				return ( F3dZWnd::_RET_WND_NONE == nKeyVal ? FALSE : TRUE );
		}
		else {
			SetFocus( NULL );
			return TRUE;
		}
	}

	std::list< F3dZWnd* >::reverse_iterator it;

	//모달
	bool bModalCheck = false;
	std::list< F3dZWnd* > modalList = m_listModal; //
	for ( it = modalList.rbegin(); it != modalList.rend() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() ) {
			pWnd->OnKeyMsg( uMsg, wParam, lParam, nRetVal );
			bModalCheck = true;
			break;
		}
	}
	if( bModalCheck ) return TRUE;

	//일반
	std::list< F3dZWnd* > normalList = m_listWnd;
	for ( it = normalList.rbegin(); it != normalList.rend() ; ++it )
	{
		F3dZWnd* pWnd = (*it);
		if( pWnd->IsShow() )
			nRetVal |= pWnd->OnKeyMsg( uMsg, wParam, lParam, nRetVal );
	}

	if( nRetVal != F3dZWnd::_RET_WND_NONE )
		return TRUE;

	return FALSE;
}

ZUIDragAndDropObject* F3dZWndManager::GetDragObject()
{ 
	 
	return m_pDragObject;
}

void F3dZWndManager::SetDragObject( ZUIDragAndDropObject* pDragObject, ZUIDragAndDropRender* pDragRender, const POINT& ptOffset )
{ 
	 
	m_pDragObject        = pDragObject;
	m_ptDragRenderOffset = ptOffset;
	m_pDragRender        = pDragRender;
	if( m_pDragRender ) {
		SetToolTip( NULL, m_nActToolTip );
	}
}

void eraseWnd( F3dZWnd* pWnd, std::list< F3dZWnd* >& wndlist, boost::unordered_map< std::wstring, F3dZWnd* >& wndHash )
{ 
	 
	std::list< F3dZWnd* >::iterator l_it = wndlist.begin();
	for( ; l_it != wndlist.end() ; ) {
		if( _wcsicmp( (*l_it)->GetID(), pWnd->GetID() ) == 0 )
			l_it = wndlist.erase( l_it );
		else
			++l_it;
	}

	boost::unordered_map< std::wstring, F3dZWnd* >::iterator it = wndHash.find( pWnd->GetID() ); 
	if( it != wndHash.end() )
		wndHash.erase( it );
}

void F3dZWndManager::EraseWndAll( F3dZWnd* pWnd )
{
	eraseWnd( pWnd, m_listModal, m_vModalHash );
	eraseWnd( pWnd, m_listWnd, m_vWndHash );
}

void F3dZWndManager::EraseWnd( F3dZWnd* pWnd, bool bModal )
{
	 
	if( bModal ) {
		eraseWnd( pWnd, m_listModal, m_vModalHash );
	}
	else {
		eraseWnd( pWnd, m_listWnd, m_vWndHash );
	}
}

void F3dZWndManager::EraseSortWnd( F3dZWnd* pWnd )
{ 
	 
	eraseWnd( pWnd, m_listSortWnd, m_vSortWndHash );
}

void F3dZWndManager::AddExistPanel( F3dZWnd* pWnd, bool bDelay/* = false*/ )
{
	 
	if( !bDelay )
		addSortWnd( pWnd );
	else {
		addDelaySortWnd( pWnd );
	}
}

void F3dZWndManager::AddPanel( F3dZWnd* pWnd, WINDOW_ARG& window_arg, bool bDelay/* = false*/ )
{ 
	 
	pWnd->CreateWnd( window_arg );

	int nControlCount = (int)window_arg.vControlList.size();
	for( int i(0); nControlCount>i; i++ )
	{
		F3dZControl* pControl = F3dZUIFactory::GetSingleton()->CreateControl( window_arg.vControlList[i] );
		if( pControl )
			pWnd->AddControl( pControl );
	}

	if( !bDelay ) 
		addSortWnd( pWnd );
	else {
		addDelaySortWnd( pWnd );
	}
}

void F3dZWndManager::addDelaySortWnd( F3dZWnd* pWnd )
{
	m_qDelaySortWnd.push( pWnd );
	m_vDelaySortWndHash.insert( std::make_pair( pWnd->GetID(), pWnd ) ); 
}

BOOL F3dZWndManager::IsExistPanel( const WCHAR* winID )
{ 
	 
	F3dZWnd* pSortWnd = GetSortWnd( winID );
	if( pSortWnd ) 
		return TRUE;

	return FALSE;
}

//Tool Tip
void F3dZWndManager::RefreshToolTip( F3dZWnd* pWndParent )
{ 
	 
	if( m_pToolTip[m_nActToolTip] && m_pToolTip[m_nActToolTip]->IsShow() && m_pLastToolTip ) {

		F3dZWnd* pTooltipParent = m_pLastToolTip->GetParent();
		if( pTooltipParent && pTooltipParent == pWndParent ) {
			m_pToolTip[m_nActToolTip]->ParseToolTipEx( m_pLastToolTip->GetToolTip(), m_pLastToolTip->GetToolTipTitle() );
			m_pToolTip[m_nActToolTip]->ParseSubToolTipEx( m_pLastToolTip->GetSubToolTip(), m_pLastToolTip->GetSubToolTipTitle() );
			m_pToolTip[m_nActToolTip]->TextMove();
		}
	}
}

void F3dZWndManager::SetToolTip( F3dZControl* pControl, int nType )
{ 
	 

	if( m_pDragRender ) //드래그 중에는 툴팁 안 보여 줌.
		return;

	if( m_bIsMsgBoxShow ) //메시지 박스 출현 중에는 툴팁 안 보여 줌.
		return;

	m_pLastToolTip = pControl;
	
	//기존에 다른 툴팁은 숨기기
	if( m_nActToolTip >= TOOLTIP_TITLE && m_nActToolTip < TOOLTIP_MAX ) {
		if( nType != m_nActToolTip && m_pToolTip[m_nActToolTip] ) {
			m_pToolTip[m_nActToolTip]->SetOwner( NULL );
			m_pToolTip[m_nActToolTip]->SetToolTipEnable( false );
		}
	}

	m_nActToolTip = nType;

	if( pControl ) {
		//활성화 하라고 알림.
		if( m_nActToolTip >= TOOLTIP_TITLE && m_nActToolTip < TOOLTIP_MAX ) {
			if( m_pToolTip[m_nActToolTip] ) {
				m_pToolTip[m_nActToolTip]->SetOwner( pControl );
				m_pToolTip[m_nActToolTip]->ParseToolTipEx( pControl->GetToolTip(), pControl->GetToolTipTitle() );
				m_pToolTip[m_nActToolTip]->ParseSubToolTipEx( pControl->GetSubToolTip(), pControl->GetSubToolTipTitle() );
				m_pToolTip[m_nActToolTip]->SetToolTipEnable( true );
			}
		}
	}
	else {
		//비활성화
		if( m_nActToolTip >= TOOLTIP_TITLE && m_nActToolTip < TOOLTIP_MAX ) {
			if( m_pToolTip[m_nActToolTip] ) {
				m_pToolTip[m_nActToolTip]->SetOwner( NULL );
				m_pToolTip[m_nActToolTip]->SetToolTipEnable( false );
			}
		}
	}		
}

void F3dZWndManager::DisableToolTip( F3dZControl* pControl, int nType )
{
	 

//	m_nActToolTip = nType;

	if( m_pToolTip[nType] && m_pToolTip[nType]->GetOwner() )
	{ 
		if( m_pToolTip[nType]->GetOwner() == pControl ) {
			m_pToolTip[nType]->SetOwner( NULL );
			m_pToolTip[nType]->SetToolTipEnable( false );
		}
	}
}

void F3dZWndManager::SetToolTipControl( F3dZControlToolTip* pToolTip, int nIndex )
{ 
	 
	//ToolTip
	FBDEL(m_pToolTip[nIndex]);
	m_pToolTip[nIndex] = pToolTip;
	if( m_pToolTip[nIndex] )
		m_pToolTip[nIndex]->SetShow( FALSE );
}

void F3dZWndManager::CloseToolTip()
{ 
	 
	for( int i(0); i < TOOLTIP_MAX; i++ )
	{
		if( m_pToolTip[i] ) {
			m_pToolTip[i]->SetShow( FALSE );
		}
	}
}	
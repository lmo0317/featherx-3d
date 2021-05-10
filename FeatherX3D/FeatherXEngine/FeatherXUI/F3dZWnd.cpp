#include "FeatherXEnginePrecompile.h"
 

namespace
{
	const float c_fAlphaTime = 0.8f;
};

F3dZWnd::F3dZWnd( F3dZWndManager* pWndManager ) : 
m_pWndManager(pWndManager), m_bShow(FALSE), m_bShowChange(FALSE), m_bMoving(FALSE), 
m_pCaptureControl(NULL), m_dwFlag(_WND_NONE), m_dCurTime(0), m_uState( ST_NONE ), 
m_dAlphaTime(0.f), m_pDragAndDropRender(NULL), m_pParent(NULL), m_fAlphaValue(-1.f),m_b3DRender(false),m_b3DCursor(false), m_bTopFocus(FALSE)
{

}

F3dZWnd::~F3dZWnd()
{ 
	m_vhashControl.clear();

	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		delete pControl;
	}
	m_vControlList.clear();

	if( m_pDragAndDropRender ) {
		delete m_pDragAndDropRender;
		m_pDragAndDropRender = NULL;
	}
}

void F3dZWnd::ClearChildWnd()
{
	for( UINT i(0); m_vChildWnd.size()>i; ++i ) 
	{
		delete m_vChildWnd[i];
	}
	m_vChildWnd.clear();
}

BOOL F3dZWnd::CreateWnd( const WINDOW_ARG& window_arg )
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
	return FALSE;
}

struct CONTROL_GREATE
{
	bool operator()( const F3dZControl* pIt1, const F3dZControl* pIt2 )
	{
		if( pIt1->GetDepth() > pIt2->GetDepth() )
			return true;

		return false;
	}
};

struct CONTROL_LESS
{
	bool operator()( const F3dZControl* pIt1, const F3dZControl* pIt2 )
	{
		if( pIt1->GetDepth() < pIt2->GetDepth() )
			return true;

		return false;
	}
};

void F3dZWnd::Show( BOOL bShowMode )
{ 
	BOOL bOld = m_bShow;
	m_bShow = bShowMode; 
	
	if( bOld != m_bShow )
		m_bShowChange = TRUE;
	else 
		m_bShowChange = FALSE;

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		m_vChildWnd[i]->Show(bShowMode);
	}

	OnShow(); 
	
	if( !m_bShow ) {
		setState(ST_NONE); 
		m_pWndManager->ReleaseCapture( this );
		m_pWndManager->CheckShowToolTip( GetID() );
	}
}

void F3dZWnd::SetRenderTopLevel( bool bFlag )
{ 
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->SetRenderTopLevel( bFlag );
	}
}

void F3dZWnd::SortControl()
{
	std::sort( m_vControlList.begin(), m_vControlList.end(), CONTROL_GREATE() );
}

void F3dZWnd::AddControl( F3dZControl* pControl )
{
	m_vhashControl.insert( std::make_pair( pControl->GetID(), pControl ) ); 

	pControl->SetParent(this);
	m_vControlList.push_back( pControl );
}

void F3dZWnd::AddChildWnd( F3dZWnd* pWnd, int nOffSetX, int nOffSetY )
{
	if( pWnd ) {
		pWnd->MovePos( m_rect.left+nOffSetX, m_rect.top+nOffSetY );
		pWnd->SetParent( this );
		m_vChildWnd.push_back( pWnd );
	}
}

void F3dZWnd::ShowChild( const WCHAR* ID, BOOL bShow )
{
	for( UINT i(0); m_vChildWnd.size()>i; ++i ) 
	{
		if( !_wcsicmp( m_vChildWnd[i]->GetID(), ID ) )
		{
			m_vChildWnd[i]->SetAlpha(1.f);
			m_vChildWnd[i]->Show( bShow );
			return;
		}
	}
}

void F3dZWnd::SetParent( F3dZWnd* pWnd )
{
	m_pParent = pWnd;
}

F3dZControl* F3dZWnd::GetControl( const WCHAR* ID )
{
	boost::unordered_map< std::wstring, F3dZControl* >::iterator hit = m_vhashControl.find( ID ); 
	if( hit != m_vhashControl.end() )
	{
		return hit->second;
	}
	
	//TODO
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		if( !_wcsicmp( pControl->GetID(), ID ) )
			return pControl;
	}

	return NULL;
}

void F3dZWnd::GetControlList( std::vector<std::wstring>& control_list )
{
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		control_list.push_back( pControl->GetID() );
	}
}

void F3dZWnd::SetAlphaOpen()
{
	setState( ST_ALPHA_INIT_OPEN );
	SetAlpha( 0.f );

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		m_vChildWnd[i]->SetAlphaOpen();
	}

	Show( TRUE );
}

void F3dZWnd::SetAlphaClose()
{
	if( IsShow() )
		setState( ST_ALPHA_INIT_CLOSE );

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		m_vChildWnd[i]->SetAlphaClose();
	}
}

void F3dZWnd::SetCenterBillBoard( const F3dVector& vtxCenter )
{
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->SetCenterBillBoard( vtxCenter );
	}
}

void F3dZWnd::SetCenter(const F3dVector& vtxCenter)
{
	m_vtxCenter = vtxCenter;
}

void F3dZWnd::SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight)
{
	m_vtxCenter = vtxCenter;
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->SetCenter3DRender( vtxCenter ,nWidth,nHeight);
	}
}

void F3dZWnd::Set3DCursor(bool bFlag)
{
	m_b3DCursor = bFlag;
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->Set3DCursor( bFlag );
	}
}

void F3dZWnd::Set3DRender(bool bFlag)
{
	m_b3DRender = bFlag;
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->Set3DRender( bFlag );
	}
}

void F3dZWnd::SetBillBoard( bool bFlag )
{
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->SetBillBoard( bFlag );
	}
}

void F3dZWnd::SetAlpha( float fValue )
{ 
	if( m_fAlphaValue == fValue )
		return;

	m_fAlphaValue = fValue;

	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		if( pControl->IsSkipAlpha() ) continue;

		pControl->SetAlpha( fValue );
	}
}

const BOOL F3dZWnd::IsShowReady() const
{ 
	return (getState() == ST_NONE); 
}

const BOOL F3dZWnd::IsAlphaClose() const
{
	return (getState() == ST_ALPHA_INIT_CLOSE || getState() == ST_ALPHA_CLOSE); 
}

void F3dZWnd::Process( double dTime, double dMeanDT )
{
	m_dCurTime = dTime;

	float fRatio = 0.f;

	switch( getState() )
	{
	case ST_NONE             :
		break;
	case ST_ALPHA_INIT_OPEN  : {
		m_dAlphaTime = m_dCurTime;
		setState( ST_ALPHA_OPEN );
		break;
		}
	case ST_ALPHA_INIT_CLOSE : {
		m_dAlphaTime = m_dCurTime;
		setState( ST_ALPHA_CLOSE );
		fRatio = 1.f;
		break; 
		}
	case ST_ALPHA_OPEN       : {
		fRatio = getClampRatioSqrt( (float)(m_dCurTime-m_dAlphaTime), c_fAlphaTime );
		if( fRatio >= 1.f ) {
			setState( ST_NONE );
			OnAlphaOpenComplete();
			fRatio = 1.f;
		}
		break;
		}
	case ST_ALPHA_CLOSE      : {
		fRatio = 1.f-getClampRatioSqrt( (float)(m_dCurTime-m_dAlphaTime), c_fAlphaTime );
		if( fRatio <= 0.f ) {
			fRatio = 0.f;
			Show( FALSE );
			setState( ST_NONE );
			OnAlphaCloseComplete();
		}
		break;
		}
	}

	BOOL bAlphaProc = ST_NONE != getState() ? TRUE : FALSE;

	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		if( pControl->IsShow() )
			pControl->Process( dTime, dMeanDT );
		if( bAlphaProc && !pControl->IsSkipAlpha() )
			pControl->SetAlpha( fRatio );
	}

	if( bAlphaProc )
		CustomSetAlpha( fRatio );

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		if( m_vChildWnd[i]->IsShow() )
			m_vChildWnd[i]->Process( dTime, dMeanDT );
	}
}

void F3dZWnd::RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer )
{
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		if( pControl->IsShow() && !pControl->IsRenderSkip() )
			pControl->RenderTarget( dTime, RenderLayer, nLayer );
	}

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		if( m_vChildWnd[i]->IsShow() )
			m_vChildWnd[i]->RenderTarget( dTime, RenderLayer, nLayer );
	}
}

void F3dZWnd::RenderControl(double dTime,double dMeanDT)
{
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		if( pControl->IsShow() && !pControl->IsRenderSkip() )
			pControl->Render( dTime );
	}

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		if( m_vChildWnd[i]->IsShow() )
			m_vChildWnd[i]->Render( dTime, dMeanDT );
	}
}

void F3dZWnd::Render( double dTime, double dMeanDT )
{	
	if(m_b3DRender | m_b3DCursor) {
		MovePos(0,0);
	}

	RenderControl(dTime,dMeanDT);
}

BOOL F3dZWnd::SetCaptureControl( F3dZControl* pControl )
{ 
	if( NULL == m_pCaptureControl )
	{
		m_pCaptureControl = pControl;
		return TRUE;
	}
	else
	{
		_MBASSERT(m_pCaptureControl);
		m_pCaptureControl = pControl;
		return TRUE;
	}
	return FALSE;
}

void F3dZWnd::ReleaseCaptureControl( F3dZControl* pControl )
{	
	if( NULL == m_pCaptureControl )
	{
	}
	else
	{
		_MBASSERT(m_pCaptureControl);
		if( m_pCaptureControl == pControl )
			m_pCaptureControl = NULL;
	}
}

BOOL F3dZWnd::IsInRect( const int& x, const int& y )
{
	return m_rect.IsInRect( x, y );
}

void F3dZWnd::SetSize( const int& nWidth, const int& nHeight )
{
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	ori_m_rect.right  = nWidth;
	ori_m_rect.bottom = nHeight;
}

void F3dZWnd::MovePosOnlyWnd( int x, int y )
{
	int offsetx = x - m_rect.left;
	int offsety = y - m_rect.top;

	// Region
	m_rect.left   += offsetx;	
	m_rect.top    += offsety;
	m_rect.right  += offsetx;	
	m_rect.bottom += offsety;    
}

void F3dZWnd::MovePos( int x, int y )
{
	int offsetx = x - m_rect.left;
	int offsety = y - m_rect.top;
	MovePosOffset( offsetx, offsety);

	OnMove();

	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		m_vChildWnd[i]->MovePosOffset( offsetx, offsety );
	}
}

void F3dZWnd::ChildWndTopCenterAlign(int nWidth,int nHeight,int nStartX,int nStartY)
{
	for( UINT i(0); m_vChildWnd.size()>i; ++i ) 
	{
		UI_RECT rt;
		m_vChildWnd[i]->GetRect(rt);
		m_vChildWnd[i]->MovePos( nStartX+nWidth/2-rt.GetWidth()/2, nStartY );
	}
}

void F3dZWnd::ChildWndCenterAlign(int nWidth,int nHeight,int nStartX,int nStartY)
{
	for( UINT i(0); m_vChildWnd.size()>i; ++i ) 
	{
		UI_RECT rt;
		m_vChildWnd[i]->GetRect(rt);
		m_vChildWnd[i]->MovePos( nStartX+nWidth/2-rt.GetWidth()/2, nStartY+nHeight/2-rt.GetHeight()/2 );
	}
}

void F3dZWnd::MovePosOffset( int offsetx, int offsety )
{
	// Region
	m_rect.left   += offsetx;	
	m_rect.top    += offsety;
	m_rect.right  += offsetx;	
	m_rect.bottom += offsety;    

	std::vector<F3dZControl*>::reverse_iterator it = m_vControlList.rbegin();
	for( ; it != m_vControlList.rend(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->ParentPosChangeNotify( offsetx, offsety );
	}
}

void F3dZWnd::SetSizeUpdate(const int& nWidth,const int& nHeight)
{	
	std::vector<F3dZControl*>::reverse_iterator it = m_vControlList.rbegin();
	for( ; it != m_vControlList.rend(); ++it )
	{
		F3dZControl* pControl = (*it);
		float fRatioX = (float)nWidth  / (float)m_rect.GetWidth();
		float fRatioY = (float)nHeight / (float)m_rect.GetHeight(); 
		int nX,nY;

		int nCtrWidth	= (int)((float)pControl->GetRect().GetWidth() * fRatioX);
		int nCtrHeight	=  (int)((float)pControl->GetRect().GetHeight() * fRatioY);
 
		//pControl->ParentSizeChangeNotify( nCtrWidth, nCtrHeight );
		pControl->GetPos(nX,nY);
		pControl->SetPos((int)(nX * fRatioX),(int)(nY * fRatioY));
	}

	SetSize(nWidth,nHeight);
}

int	F3dZWnd::OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, int& nPrevRetValue )
{
	if( !IsShow() )
		return _RET_WND_NONE;

	if( IsWndFlag( _WND_PASS ) ) 
		return _RET_WND_NONE;

	if( nPrevRetValue & _RET_WND_USED )
		return _RET_WND_NONE;

	int nRetValue = F3dZControl::_RET_CTL_NONE;

	//맨 위에것 부터 처리
	std::vector<F3dZControl*>::reverse_iterator it = m_vControlList.rbegin();
	for( ; it != m_vControlList.rend(); ++it )
	{
		F3dZControl* pControl = (*it);
		nRetValue |= pControl->OnKeyMsg( uMsg, wParam, lParam, nRetValue );
	}

	return nRetValue;
}

const bool F3dZWnd::IsLimitResolution()
{ 
	int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();

	int nX, nY;
	F3dCDevice::GetSingleton()->GetScreenStart( nX, nY );

	UI_RECT ui_rect;
	UI_RECT full_rect( nX, nY, nX+nWidth, nY+nHeight );
	GetRect( ui_rect );

	if( ui_rect.left   < full_rect.left   ) return true;
	if( ui_rect.top    < full_rect.top    ) return true;
	if( ui_rect.right  > full_rect.right  ) return true;
	if( ui_rect.bottom > full_rect.bottom ) return true;

	return false;
}

void F3dZWnd::LimitResolution( int nLeft, int nTop, int nRight, int nBottom )
{ 
	int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();

	int nX, nY;
	F3dCDevice::GetSingleton()->GetScreenStart( nX, nY );

	UI_RECT ui_rect;
	UI_RECT full_rect( nX+0+nLeft, nY+0+nTop, nX+nWidth+nRight, nY+nHeight+nBottom );
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
		
		for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
			m_vChildWnd[i]->MovePosOffset( sizeMoveOffset.x, sizeMoveOffset.y );
		}
	}

	OnLimitResolution();
}

int	F3dZWnd::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{ 
	if( !IsShow() )
		return _RET_WND_NONE;

	if( IsWndFlag( _WND_PASS ) ) 
		return _RET_WND_NONE;

	//Mouse In/Out
	if( IsInRect( x, y ) ) {
		OnControlNotify( WINDOW_IN, GetID() );
	}
	else {
		OnControlNotify( WINDOW_OUT, GetID() );
	}

	//Child Msg
	int nChildRetVal = _RET_WND_NONE;
	for( UINT i(0); m_vChildWnd.size()>i; ++i ) {
		nChildRetVal |= m_vChildWnd[i]->OnMouseMsg( uMsg, x, y, nPrevRetValue );
	}

	if( nChildRetVal != F3dZWnd::_RET_WND_NONE ) {
		return nChildRetVal;
	}

	if( nPrevRetValue & _RET_WND_USED )
		return _RET_WND_NONE;

	int nRetValue = F3dZControl::_RET_CTL_NONE;

	//Mouse Move
	if( uMsg == WM_MOUSEMOVE ) {
		if( IsWndFlag( _WND_MOVEABLE ) && IsMoveAble() ) {
			int XOffset = x - m_ptMovingOffset.x;
			int YOffset = y - m_ptMovingOffset.y;

			if(XOffset != 0 || YOffset != 0) {
				MovePos( XOffset, YOffset );
			}

			if( IsWndFlag(_WND_LIMIT_MOVE) ) {
				LimitResolution();
			}

			nRetValue = _RET_WND_USED;
			return nRetValue;
		}
	}

	if( GetCaptureControl() ) {
		GetCaptureControl()->OnMouseMsg( uMsg, x, y, nRetValue ); //무조건 한번 처리(포커스 해제등)
	}
	
	{
		///Control
		//맨 위에것 부터 처리
		std::vector<F3dZControl*>::reverse_iterator it = m_vControlList.rbegin();
		for( ; it != m_vControlList.rend(); ++it ) {
			F3dZControl* pControl = (*it);
			if( !pControl->IsShow() ) continue;
			nRetValue |= pControl->OnMouseMsg( uMsg, x, y, nRetValue );
		}

		if( !m_vTempControlList.empty() ) {
			m_vControlList = m_vTempControlList;
			m_vTempControlList.erase( m_vTempControlList.begin(), m_vTempControlList.end() );
		}
	}

#ifdef _DEBUG
	//LMO :클릭한 UI 의 이름을 알아본다
	if(uMsg == WM_MBUTTONDBLCLK)
	{
		WCHAR szTemp[128];
		swprintf_s( szTemp, 128, L" [%s] ", GetID() );
		MessageBox( 0, szTemp, L"CLICK WINDOW", MB_OK );		
	}
#endif

	//Window
	//하위 컨트롤에서 사용 안되었음
	if( nRetValue == F3dZControl::_RET_CTL_NONE ) {
		if( IsInRect( x, y ) ) {
			if( !IsWndFlag( _WND_SKIP ) ) {
				switch( uMsg )	{
				case WM_LBUTTONDOWN:
					if( m_pWndManager->SetCapture(this) ) {
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
				nRetValue = _RET_WND_NONE;
		}
		else {
			switch( uMsg )	{
			case WM_LBUTTONUP:
				if( m_bMoving ) {
					m_bMoving = FALSE;
					m_pWndManager->ReleaseCapture(this);
				}
			}

			nRetValue = _RET_WND_NONE;
		}
	}
	else {
		if( m_bMoving ) {
			m_bMoving = FALSE;
			m_pWndManager->ReleaseCapture(this);
		}
	}

	return nRetValue;
}

ZUIDragAndDropObject* F3dZWnd::GetDragObject()
{
	return m_pWndManager->GetDragObject();
}

void F3dZWnd::SetDrop( ZUIDragAndDropObject* pDragObject, F3dZControl* pRecvControl )
{
	POINT point;
	point.x = 0; point.y = 0;

	OnDropObject( pDragObject, pRecvControl );

	if( pDragObject ) pDragObject->OnDragObjectDroped();

	m_pWndManager->SetDragObject( NULL, NULL, point );
}

void F3dZWnd::MoveDrop( ZUIDragAndDropObject* pDragObject, F3dZControl* pRecvControl )
{
	OnMoveDropObject( pDragObject, pRecvControl );
}

void F3dZWnd::SetDragObject( ZUIDragAndDropObject* pDragObject, const POINT& ptOffset )
{
	ZUIDragAndDropRender* pRender = NULL;

	if( pDragObject )
		pRender = OnDragObject( pDragObject );

	if( pRender )
		m_pWndManager->SetDragObject( pDragObject, pRender, ptOffset );
	else
		m_pWndManager->SetDragObject( NULL, NULL, ptOffset );
}

void F3dZWnd::RemoveControlforTemp( std::vector< F3dZControl* >& vRemoveList )
{
	m_vTempControlList = m_vControlList;

	std::vector<F3dZControl*>::iterator it;
	for( UINT i(0); vRemoveList.size()>i; ++i ) {
		F3dZControl* pControl = vRemoveList[i];
		it = std::find( m_vTempControlList.begin(), m_vTempControlList.end(), pControl );
		if( it != m_vTempControlList.end() ) {
			m_vTempControlList.erase( it );
		}
	}
}

void F3dZWnd::AddControlforTemp( F3dZControl* pControl )
{ 
	if( pControl )
		m_vTempControlList.push_back( pControl );
}

void F3dZWnd::SetToolTip( F3dZControl* pControl, int nType )
{ 
	m_pWndManager->SetToolTip( pControl, nType );
}

void F3dZWnd::DisableToolTip( F3dZControl* pControl, int nType )
{ 
	m_pWndManager->DisableToolTip( pControl, nType );
}

void F3dZWnd::SetToolTipType( int nIndex )
{
	std::vector<F3dZControl*>::iterator it = m_vControlList.begin();
	for( ; it != m_vControlList.end(); ++it )
	{
		F3dZControl* pControl = (*it);
		pControl->SetToolTipType( nIndex );
	}
}
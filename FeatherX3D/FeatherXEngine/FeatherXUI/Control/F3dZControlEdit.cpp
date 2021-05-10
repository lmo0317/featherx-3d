#include "FeatherXEnginePrecompile.h"
#include "MBfasttime.h"

//////////////////////////////////////////////////////////////////////////

EditHandleMap* F3dZControlEdit::s_mapEdit=NULL;
HWND F3dZControlEdit::s_hWndCurrentFocus=NULL;

void F3dZControlEdit::InitEdit()
{  
	s_mapEdit = new EditHandleMap;
	CIMM::CreateSingleton();
	CIMM::GetSingleton()->Init();
}

void F3dZControlEdit::DeInitEdit()
{  
	CIMM::DestroySingleton();
	delete s_mapEdit;
}

bool F3dZControlEdit::IsAnyFocus()
{  
	return s_hWndCurrentFocus ? true : false;
}

void F3dZControlEdit::SayClear()
{
	SetCaption( L"" );
}

void F3dZControlEdit::SayOneErase()
{
	if( m_strSay.length() > 0 ) {
		m_strSay.erase( m_strSay.end()-1 );
		SetCaption( m_strSay.c_str() );
	}
}

void F3dZControlEdit::SayAddOne( const WCHAR* pStr )
{
	if( m_strSay.length() >= m_nLimitLength )
		return;

	m_strSay += pStr;
	SetCaption( m_strSay.c_str() );
}

LRESULT CALLBACK F3dZControlEdit::EditBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{  
	if( uMsg == WM_CONTEXTMENU )
	{
		return 0;
	}

	EditHandleMap::iterator it = F3dZControlEdit::s_mapEdit->find(hWnd);
	LRESULT nRet = 0;
	// Keyboard Message를 전달 
	if(it != F3dZControlEdit::s_mapEdit->end() )
	{
		F3dZControlEdit* pEdit = it->second;

		switch(uMsg)
		{
		case WM_CHAR:
			if( !pEdit->OnMessage( hWnd, uMsg, wParam, lParam ) )
				return 0L;

			/* IME Messages */ 
		case WM_INPUTLANGCHANGE:
		case WM_IME_SETCONTEXT:
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_ENDCOMPOSITION:
		case WM_KEYDOWN:
		case WM_KEYUP:
			{
				if( !pEdit->OnMessage( hWnd, uMsg, wParam, lParam ) )
					return 0L;
			}
			break;
		case WM_IME_NOTIFY:
			{
				//OnControlNotify( KEDIT_IME_CHANGE, pEdit->GetID() );
				if( !pEdit->OnMessage( hWnd, uMsg, wParam, lParam ) )
					return 0L;
			}
			break;
			/* End of IME Messages */ 
		}

		nRet = CallWindowProc(pEdit->m_oldProc, pEdit->m_hWnd, uMsg,wParam,lParam);
		int pn=0;
		switch(uMsg) 
		{
		case WM_KEYDOWN:
		case WM_KEYUP: // If you remove this line, this module acts abnormally when typing Korean language.

			/*
			case WM_CHAR:
			i->second->OnKeyMessage(uMsg, wParam);
			break;
			*/

		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			{
				//int x = LOWORD(lParam);
				//int y = HIWORD(lParam);
				//int nPrevRetValue;
				//pEdit->OnMouseMsg( uMsg, x, y, nPrevRetValue );
			}
			break;

		case WM_KILLFOCUS:
			pEdit->SetFocus( FALSE );
#ifdef _IME_DEBUG_
			MBCLog::GetSingleton()->DebugOutput( L"Edit WM_KILLFOCUS : %s\n", pEdit->GetID() );
#endif
			break;

		default:
			break;
		}
	}

	return nRet;
}

//////////////////////////////////////////////////////////////////////////

namespace
{
	const unsigned short c_nEditLimit = 84;
	const unsigned short c_nCaretOffSet_X = 2;
}

F3dZControlEdit::F3dZControlEdit() : m_hWnd(NULL), m_oldProc(NULL), m_pIMEInput(new CInput), m_bUpdate(FALSE), m_bFocus(FALSE), m_nLimitLength(c_nEditLimit),
 m_nOldLine(0),
 m_nCurLine(0),
 m_bMultiLine(FALSE),
 m_CheckCaret(0.f),
 m_bStateCaret(FALSE),
 m_bCtrl(false),
 m_bPrivateMode(false),
 m_bCaretStopMove(false),
 m_bItemLink(false),
 m_nItemLinkEndPosX(0),
 m_nItemLinkEndPosY(0)
{ 
	 
	m_dwTextAlign = F3dZUIText::ALIGN_LEFT | DT_SINGLELINE;

	m_CompositionColor = D3DCOLOR_XRGB( 255,  98,  98 );
	m_IndicatorColor   = D3DCOLOR_XRGB( 255, 255, 255 );
	m_ReadingColor     = D3DCOLOR_XRGB(  98,  98, 255 );
	m_CandidateColor   = D3DCOLOR_XRGB(   0, 128, 255 );
	m_CaretColor       = D3DCOLOR_XRGB( 255, 255, 255 );  

	m_pIMEInput->SetLimit( m_nLimitLength );
}

F3dZControlEdit::~F3dZControlEdit()
{  
	delete m_pIMEInput;
	F3dZControlEdit::s_mapEdit->erase(m_hWnd);

	if( m_hWnd )
		DestroyWindow( m_hWnd );

	SetFocus( FALSE );
}

void F3dZControlEdit::SetLimit( int nLimit )
{  
	m_nLimitLength = nLimit;
	m_pIMEInput->SetLimit( nLimit );
}

void F3dZControlEdit::SetNumberAndCapital( bool bFlag )
{
	m_pIMEInput->SetNumberAndCapital(bFlag);
}

void F3dZControlEdit::SetOnlyNumber(bool bFlag)
{  
	m_pIMEInput->SetOnlyNumber(bFlag);
}

void F3dZControlEdit::SetSkipEnterKeyClear( bool bFlag )
{
	m_pIMEInput->SetSkipEnterKeyClear(bFlag);
}

void F3dZControlEdit::SetPrivateMode( bool bFlag )
{  
	m_bPrivateMode = bFlag;
}

void F3dZControlEdit::SetCaretStopMove(bool bFlag)
{ 
	 
	m_bCaretStopMove = bFlag;
}

void F3dZControlEdit::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount > 0 ) 
	{
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}

	m_defRect = m_ori_rect;

	//
	m_hWnd = ::CreateWindow( L"EDIT",
			NULL,
			WS_CHILD|WS_TABSTOP|ES_AUTOHSCROLL|ES_LEFT,
			m_rect.left,	m_rect.top, 
			m_rect.right  - m_rect.left, 
			m_rect.bottom - m_rect.top,
			F3dZWndManager::GetHWND(),
			NULL,
			F3dZWndManager::GetInstance(),
			NULL );	

	//
	m_oldProc = (WNDPROC)( SetWindowLong(m_hWnd, GWL_WNDPROC,(LONG)(F3dZControlEdit::EditBoxProc) ) );

	m_pIMEInput->OnInputLanguageChange(m_hWnd, 0, (LPARAM)GetKeyboardLayout(0));

	//
	(*F3dZControlEdit::s_mapEdit)[m_hWnd] = this;
}

void F3dZControlEdit::Process( double dTime, double dMeanDT )
{  
	F3dZControl::Process( dTime, dMeanDT );

	if( !IsFocus() ) 
		return;

	if( dTime - m_CheckCaret > .5f ) {
		m_CheckCaret = dTime;
		if( m_bStateCaret )
		{
			SetCaret( L"_" );

			if( m_bItemLink )
			{
				if( m_Caret.get() )
					m_nItemLinkEndPosX = m_Caret->GetPosX();
				if( m_Indicator.get() )
					m_nItemLinkEndPosY = m_Indicator->GetPosY();
				m_bItemLink = FALSE;
			}
		}
		else
			SetCaret( L"" );
		
		m_bStateCaret = !m_bStateCaret;
	}
}

void F3dZControlEdit::Render( double dTime )
{  
	if( IsShow() == FALSE ) return;

	if( m_bUpdate )
	{
		std::wstring text;
		const wchar_t* indicator = m_pIMEInput->GetIndicator();
		SetIndicator( indicator );

		if(m_pIMEInput->GetImeState() == TRUE) {
			m_IndicatorColor = D3DCOLOR_XRGB( 255, 255, 255 ); //활성
		} else {
			m_IndicatorColor = D3DCOLOR_XRGB( 128, 128, 128 ); //비활성
		}

		int textLen = m_pIMEInput->GetInput(text);

		if( textLen >= 0 )
		{
			if( IsMultiLine() ) {

				if( m_cmp_text != text ) {
					m_cmp_text = text;

					m_nOldLine = m_nCurLine;

					m_vMultiLine.clear();
					
					{
//						_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
						SplitString( text.c_str(), m_vMultiLine );
					}

					{
//						_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );

						m_nCurLine = (int)m_vMultiLine.size();

						if( m_nOldLine != m_nCurLine )
							OnControlNotify( EDIT_LINE_CHANGE, GetID(), m_nOldLine, m_nCurLine );
					}

					{
//						_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
						RefreshMultiLine();
					}
				}
			}
			else {
				SetCaption( text.c_str() );
			}

			if( 0 <= m_pIMEInput->GetComp(text) )
				SetComposition( text.c_str() );

			if( 0 <= m_pIMEInput->GetReading(text) )
				SetReading( text.c_str() );

			OnControlNotify( EDIT_ENDCOMPOSITION, GetID() );
		}

		SetCaret( L"_" );

		m_bUpdate = FALSE;

		//귓속말 관련
		if( MBStringUtil::GetSplitCount( m_strSay.c_str(), L" " ) > 1 )
		{
			OnControlNotify( EDIT_SPACE, GetID() );
		}
	}

	F3dZControl::Render( dTime );

	//BG
	if( m_nSetCount > 0 )	{
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )		{
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}

	//Text
	renderCaption( dTime );

	if( IsMultiLine() )
		renderMultiLine( dTime );

	//IME
#ifdef _DEBUG
	renderIndicator  ( dTime );
#endif
	renderComposition( dTime );
	renderReading    ( dTime );
	renderCandidate  ( dTime );

	//커서
	if( IsFocus() )
		renderCaret      ( dTime );
}

void F3dZControlEdit::renderIndicator  ( double dTime ){ if( m_Indicator.get()   ) m_Indicator->Render( dTime );   }
void F3dZControlEdit::renderComposition( double dTime ){ if( m_Composition.get() ) m_Composition->Render( dTime ); }
void F3dZControlEdit::renderReading    ( double dTime ){ if( m_Reading.get()     ) m_Reading->Render( dTime );     }
void F3dZControlEdit::renderCandidate  ( double dTime ){ if( m_Candidate.get()   ) m_Candidate->Render( dTime );   }
void F3dZControlEdit::renderCaret      ( double dTime ){ if( m_Caret.get()       ) m_Caret->Render( dTime );       }
void F3dZControlEdit::renderMultiLine  ( double dTime )
{  
	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->Render( dTime );
}


void F3dZControlEdit::SetIndicator( const WCHAR* pStr )
{  
	if( m_Indicator.get() && wcslen(pStr) > 0 )
		m_Indicator->SetText( pStr, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_IndicatorColor );
	else {
		m_Indicator.reset( new F3dZUIText( pStr, m_rect.GetWidth(), m_rect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_IndicatorColor, m_bBillboard ,m_b3DRender,m_b3DCursor) );
		m_Indicator->SetRenderTopLevel( m_bRenderTopLevel );
	}

	if( IsMultiLine() ) {
		m_Indicator->SetPos( m_nPosX, m_nPosY-m_defRect.GetHeight() );
	}
	else
		m_Indicator->SetPos( m_nPosX, m_nPosY-m_defRect.GetHeight() );
}

void F3dZControlEdit::SetComposition( const WCHAR* pStr )
{  
	if( m_Composition.get() && wcslen(pStr) > 0 )
		m_Composition->SetText( pStr, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CompositionColor );
	else {
		m_Composition.reset( new F3dZUIText( pStr, m_rect.GetWidth(), m_rect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CompositionColor, m_bBillboard ,m_b3DRender ,m_b3DCursor) );
		m_Composition->SetRenderTopLevel( m_bRenderTopLevel );
	}

	if( IsMultiLine() ) {
		m_Composition->SetPos( m_nPosX+(m_Caption.get() ? m_Caption->GetSizeX() : 0), m_nPosY );

		std::vector< F3dZUITextPtr >::reverse_iterator rit = m_vTextList.rbegin();
		if( rit != m_vTextList.rend() ) {	
			int nIndex = (int)m_vTextList.size()-1;
			m_Composition->SetPos( m_nPosX+(*rit)->GetSizeX(), m_nPosY+(nIndex*m_defRect.GetHeight()) );
		}
	}
	else
		m_Composition->SetPos( m_nPosX+(m_Caption.get() ? m_Caption->GetSizeX() : 0), m_nPosY );
}

void F3dZControlEdit::SetReading( const WCHAR* pStr )
{  
	if( m_Reading.get() && wcslen(pStr) > 0 )
		m_Reading->SetText( pStr, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_ReadingColor );
	else {
		m_Reading.reset( new F3dZUIText( pStr, m_rect.GetWidth(), m_rect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_ReadingColor, m_bBillboard  ,m_b3DRender,m_b3DCursor) );
		m_Reading->SetRenderTopLevel( m_bRenderTopLevel );
	}

	if( IsMultiLine() ) {

		m_Reading->SetPos( m_nPosX+(m_Caption.get() ? m_Caption->GetSizeX():0), m_nPosY-m_defRect.GetHeight() );

		std::vector< F3dZUITextPtr >::reverse_iterator rit = m_vTextList.rbegin();
		if( rit != m_vTextList.rend() ) {	
			int nIndex = (int)m_vTextList.size()-1;
			m_Reading->SetPos( m_nPosX+(*rit)->GetSizeX(), m_nPosY-(nIndex*m_defRect.GetHeight()) );
		}
	}
	else
		m_Reading->SetPos( m_nPosX+(m_Caption.get() ? m_Caption->GetSizeX():0), m_nPosY-m_defRect.GetHeight() );
}

void F3dZControlEdit::SetCandidate( const WCHAR* pStr )
{  
	if( m_Candidate.get() && wcslen(pStr) > 0 )
		m_Candidate->SetText( pStr, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CandidateColor );
	else {
		m_Candidate.reset( new F3dZUIText( pStr, m_rect.GetWidth(), m_rect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CandidateColor, m_bBillboard ,m_b3DRender ,m_b3DCursor) );
		m_Candidate->SetRenderTopLevel( m_bRenderTopLevel );
	}

	if( IsMultiLine() ) {

		m_Candidate->SetPos( m_nPosX+(m_Caption.get()?m_Caption->GetSizeX():0), m_nPosY-m_defRect.GetHeight() );

		std::vector< F3dZUITextPtr >::reverse_iterator rit = m_vTextList.rbegin();
		if( rit != m_vTextList.rend() ) {	
			int nIndex = (int)m_vTextList.size()-1;
			m_Candidate->SetPos( m_nPosX+(*rit)->GetSizeX(), m_nPosY-(nIndex*m_defRect.GetHeight()) );
		}
	}
	else
		m_Candidate->SetPos( m_nPosX+(m_Caption.get()?m_Caption->GetSizeX():0), m_nPosY-m_defRect.GetHeight() );
}

void F3dZControlEdit::SetCaret( const WCHAR* pStr )
{ 
	 
	if( m_bCaretStopMove )
	{
		if( m_Caret.get() && wcslen(pStr) > 0 )
			m_Caret->SetText( pStr, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CaretColor );
		else {
			m_Caret.reset( new F3dZUIText( pStr, m_rect.GetWidth(), m_rect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CaretColor, m_bBillboard  ,m_b3DRender,m_b3DCursor) );
			m_Caret->SetRenderTopLevel( m_bRenderTopLevel );
		}

		m_Caret->SetPos( m_nPosX, m_nPosY );
		return;
	}

	if( m_Caret.get() && wcslen(pStr) > 0 )
		m_Caret->SetText( pStr, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CaretColor );
	else {
		m_Caret.reset( new F3dZUIText( pStr, m_rect.GetWidth(), m_rect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_CaretColor, m_bBillboard ,m_b3DRender,m_b3DCursor ) );
		m_Caret->SetRenderTopLevel( m_bRenderTopLevel );
	}

	int nCompositioX = m_Composition.get() ? m_Composition->GetSizeX() : 0 ;
	int nCaptionX    = m_Caption.get()     ? m_Caption->GetSizeX()     : 0 ;
	if( IsMultiLine() ) {

		m_Caret->SetPos( m_nPosX+nCaptionX+nCompositioX+c_nCaretOffSet_X, m_nPosY ); //초기 위치

		std::vector< F3dZUITextPtr >::reverse_iterator rit = m_vTextList.rbegin();
		if( rit != m_vTextList.rend() ) {	
			int nIndex = (int)m_vTextList.size()-1;
			m_Caret->SetPos( m_nPosX+(*rit)->GetSizeX()+nCompositioX+c_nCaretOffSet_X, m_nPosY+(nIndex*m_defRect.GetHeight()) );
		}
	}
	else
		m_Caret->SetPos( m_nPosX+nCaptionX+nCompositioX+c_nCaretOffSet_X, m_nPosY );
}

void F3dZControlEdit::SetCaption( const WCHAR* pStr, BOOL bNew/*=FALSE*/ )
{  
	if( !IsMultiLine() ) {
		if( IsPrivateMode() ) {
			std::wstring strTemp = pStr;
			if( !strTemp.empty() )
				strTemp.replace( 0, strTemp.size(), strTemp.size(), L'*' );
			F3dZControl::SetCaption( strTemp.c_str() );
		}
		else
			F3dZControl::SetCaption( pStr );
	}

	m_pIMEInput->SetInput( pStr );
	m_strSay = pStr;

	m_bUpdate = TRUE;
}

void F3dZControlEdit::RefreshMultiLine()
{  
	m_vTextList.erase( m_vTextList.begin(), m_vTextList.end() );

	for( UINT i(0); m_vMultiLine.size()>i; ++i )
	{
		F3dZUITextPtr pText( new F3dZUIText( m_vMultiLine[i].c_str(), m_defRect.GetWidth(), m_defRect.GetHeight(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor, m_bBillboard  ,m_b3DRender,m_b3DCursor) );
		pText->SetRenderTopLevel( m_bRenderTopLevel );
		pText->SetPos( m_nPosX, m_nPosY+(i*m_defRect.GetHeight()) );
		m_vTextList.push_back( pText ); 
	}
}

#include <Winnls32.h>
extern HWND g_hWnd;

void F3dZControlEdit::SetFocus( BOOL bFocus )
{  
	if( !IsEnable() )
		return;
#ifdef _IME_DEBUG_
//	MBCLog::GetSingleton()->DebugOutput( L"Edit SetFocus : %s %d\n", GetID(), bFocus );	
#endif
	m_bFocus = bFocus;

	if( bFocus )
	{
		::EnableWindow( m_hWnd, TRUE );
		::SetFocus( m_hWnd );
		s_hWndCurrentFocus = m_hWnd;
		// 포커스시 입력된 텍스트가 없으면 공백 표시
		if( m_strSay.empty() )
			SetCaption( L"" );
	}
	else
	{
		::EnableWindow(m_hWnd, FALSE);
		::SetFocus( F3dZWndManager::GetSingleton()->GetHWND() );
		s_hWndCurrentFocus = NULL;
		// 비포커스시 입력된 텍스트가 없으면 더미텍스트 표시
		if( m_strSay.empty() )
			SetDummyCaption( m_strDummyText.c_str() );
	}
}

LONG F3dZControlEdit::OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{  
	m_bUpdate = TRUE;

	switch( msg )
	{
		/* IME Messages */ 
	case WM_INPUTLANGCHANGE:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
		m_pIMEInput->OnInputLanguageChange(hWnd, wParam, lParam);
#ifdef _IME_DEBUG_
		MBCLog::GetSingleton()->DebugOutput( L"WM_INPUTLANGCHANGE %s\n", GetID() );
#endif
		}
		break;

	case WM_IME_SETCONTEXT:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
		lParam = 0;
#ifdef _IME_DEBUG_
		MBCLog::GetSingleton()->DebugOutput( L"WM_IME_SETCONTEXT %s\n", GetID() );
#endif
		}
		break;

	case WM_IME_STARTCOMPOSITION:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
#ifdef _IME_DEBUG_
		MBCLog::GetSingleton()->DebugOutput( L"WM_IME_STARTCOMPOSITION %s\n", GetID() );
#endif
		}
		return 0L;

	case WM_IME_COMPOSITION:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			if(m_pIMEInput->OnComposition(hWnd, wParam, lParam)) {
				m_pIMEInput->GetInput( m_strSay );
#ifdef _IME_DEBUG_
			MBCLog::GetSingleton()->DebugOutput( L"WM_IME_COMPOSITION %s\n", GetID() );
#endif
				return 0L;
			}
		}
		break;

	case WM_IME_ENDCOMPOSITION:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			if(m_pIMEInput->OnEndComposition(hWnd, wParam, lParam)) {
#ifdef _IME_DEBUG_
			MBCLog::GetSingleton()->DebugOutput( L"WM_IME_ENDCOMPOSITION %s\n", GetID() );
#endif
			return 0L;
			}
		}
		break;

	case WM_IME_NOTIFY:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			if(m_pIMEInput->OnNotify(hWnd, wParam, lParam)) {
#ifdef _IME_DEBUG_
			MBCLog::GetSingleton()->DebugOutput( L"WM_IME_NOTIFY %s\n", GetID() );
#endif
			return 0L;
			}
		}
		break;

	case WM_CHAR:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			if( m_pIMEInput->OnChar(hWnd, wParam, lParam, m_strSay) )	
			{
				if( wParam == '\r' || wParam == '\n' )	
				{
					if( m_pIMEInput->IsSkipEnterKeyClear() )
					{
						if( !m_strSay.empty() ) {
							if( m_strSay.length() > m_nLimitLength )
								m_strSay.resize( m_nLimitLength );

							OnControlNotify( EDIT_ENTER, GetID() );
						}
						else {
							OnControlNotify( EDIT_ENTER_EMPTY, GetID() );
						}
					}
					else
					{	//엔터 입력시, 기존 Edit의 데이타가 지워지는 경우
						if( !m_strSay.empty() && m_pIMEInput->IsEmpty() ) {
							if( m_strSay.length() > m_nLimitLength )
								m_strSay.resize( m_nLimitLength );

							OnControlNotify( EDIT_ENTER, GetID() );
						}
						else {
							OnControlNotify( EDIT_ENTER_EMPTY, GetID() );
						}
					}
				}
				else if( wParam == '\t' ) {
					OnControlNotify( EDIT_TAB, GetID() );
				}
				else if( wParam == '\b' ) {
					if( m_Caret && m_Caret->GetPosX() <= m_nItemLinkEndPosX && m_Indicator->GetPosY() == m_nItemLinkEndPosY )
						OnControlNotify( EDIT_ITEM_LINK_BACK, GetID(), wParam );
				}

	#ifdef _IME_DEBUG_
				MBCLog::GetSingleton()->DebugOutput( L"WM_CHAR %s\n", GetID() );
	#endif
				return 0L;
			}
		}
		break;
		/* End of IME Messages */ 
	
	case WM_KEYDOWN:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			switch(wParam)
			{
			case VK_SHIFT   : break;
			case VK_CONTROL : m_bCtrl = true; break;
			case VK_UP   : OnControlNotify( EDIT_ARROW_DN, GetID(), wParam ); break;
			case VK_DOWN : OnControlNotify( EDIT_ARROW_DN, GetID(), wParam ); break;
			case VK_LEFT : OnControlNotify( EDIT_ARROW_DN, GetID(), wParam ); break;
			case VK_RIGHT: OnControlNotify( EDIT_ARROW_DN, GetID(), wParam ); break;
			case VK_BACK : OnControlNotify( EDIT_BACK, GetID(), wParam ); break;

			case 0x56 : { //붙여넣기
					if( m_bCtrl ) {
						pasteClipboard();
					}
				}
				break;
			case 0x58 : {//잘라내기
					if( m_bCtrl ) {
						copyClipboard();
						m_pIMEInput->SetInput( L"" );
					}
				}
				break;
			case 0x43 : {//복사
					if( m_bCtrl ) {
						copyClipboard();
					}
				}
				break;
			}
		}
		break;

	case WM_KEYUP:
		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			switch(wParam)
			{
			case VK_SHIFT   : break;
			case VK_CONTROL :
				m_bCtrl = false;
				break;
			case VK_UP   : OnControlNotify( EDIT_ARROW_UP, GetID(), wParam ); break;
			case VK_DOWN : OnControlNotify( EDIT_ARROW_UP, GetID(), wParam ); break;
			case VK_LEFT : OnControlNotify( EDIT_ARROW_UP, GetID(), wParam ); break;
			case VK_RIGHT: OnControlNotify( EDIT_ARROW_UP, GetID(), wParam ); break;
			}
		}
		break;
	}
	return 1;
}

void F3dZControlEdit::copyClipboard()
{  
	if( m_strSay.empty() ) return;

	if (!OpenClipboard( F3dZWndManager::GetHWND() )) return; 
	EmptyClipboard(); 

	HGLOBAL hglb; 
	LPSTR   lptstr; 

	CHAR buff[1024] = {0,};
	size_t len = ::WideCharToMultiByte( m_pIMEInput->GetCodePage(), 0, m_strSay.c_str(), (int)m_strSay.length(), buff, 1024, NULL, NULL );
	std::string source = buff;

	hglb = GlobalAlloc( GMEM_MOVEABLE, (source.length()+1)*sizeof(CHAR) ); 
	if (hglb == NULL) {
		CloseClipboard(); 
		return ; 
	}

	lptstr = (LPSTR)GlobalLock(hglb); 
	if( lptstr ) {
		memcpy(lptstr, source.c_str(), source.length()*sizeof(CHAR) ); 
		lptstr[source.length()] = (CHAR)0; 
	}
	GlobalUnlock(hglb); 
	SetClipboardData(CF_TEXT, hglb); 

	CloseClipboard();
}

void F3dZControlEdit::pasteClipboard()
{  
	if( !IsClipboardFormatAvailable( CF_TEXT ) )
		return;

	if( OpenClipboard( F3dZWndManager::GetHWND() ) ) 
	{ 
		HGLOBAL hglb; 

		hglb = GetClipboardData( CF_TEXT ); 
		if( hglb ) {

			std::string strTmp;
			strTmp = (LPSTR)GlobalLock(hglb); 

			wchar_t wbuff[1024] = {0,};
			size_t len = ::MultiByteToWideChar( m_pIMEInput->GetCodePage(), 0, strTmp.c_str(), (int)strTmp.length(), wbuff, 1024 );

			if( m_pIMEInput->GetLimit() > (int)strTmp.length() ) {
				m_pIMEInput->AddInput( wbuff );

				std::wstring text;
				int textLen = m_pIMEInput->GetInput(text);
				SetCaption( text.c_str() );
			}

			GlobalUnlock(hglb); 
		}
		CloseClipboard();
	}
}

int	F3dZControlEdit::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{  
	int nRetValue = _RET_CTL_NONE;

	if( m_nState == F3dZUISprite::SET_DISABLE || !m_bEnable )
		return nRetValue;

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
	{
		switch( uMsg )
		{
		case WM_LBUTTONDBLCLK: {
			m_bLeftButton = FALSE;
			OnControlNotify( LBUTTON_DBL_CLICK, GetID() );
			SetFocus( TRUE );
		   }
		   break;
		case WM_RBUTTONDBLCLK: {
			m_bRightButton = FALSE;
			OnControlNotify( RBUTTON_DBL_CLICK, GetID() );
			SetFocus( TRUE );
		   }
		   break;
		case WM_MBUTTONDBLCLK: {
			m_bMiddleButton = FALSE;
			OnControlNotify( MBUTTON_DBL_CLICK, GetID() );
			SetFocus( TRUE );
		   }
		   break;
		case WM_LBUTTONDOWN: {
				if( GetParent() )	{
					if( GetParent()->SetCaptureControl(this) )	{
						m_bLeftButton = TRUE;
						SetFocus( TRUE );
					}
				}
			 }
			 break;
		case WM_RBUTTONDOWN:		m_bRightButton  = TRUE;			break;
		case WM_MBUTTONDOWN:		m_bMiddleButton = TRUE;			break;
		case WM_LBUTTONUP: {
			if( m_bLeftButton )		{
				OnControlNotify( LBUTTON_CLICK, GetID() );
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
		if( uMsg == WM_LBUTTONDOWN )
		{
			if( IsFocus() ) SetFocus( FALSE );
		}
		else if( uMsg == WM_LBUTTONUP )
		{
			if( m_bLeftButton && GetParent() )
				GetParent()->ReleaseCaptureControl(this);
			m_bLeftButton = FALSE;
		}
		else if( uMsg == WM_RBUTTONUP )
			m_bRightButton = FALSE;
		else if( uMsg == WM_MBUTTONUP )
			m_bMiddleButton = FALSE;

	}

	return nRetValue;
}

void F3dZControlEdit::MovePos( int x, int y )
{  
	F3dZControl::MovePos( x, y );

	if( m_Indicator.get()   ) m_Indicator->SetPos( m_nPosX, m_nPosY-m_defRect.GetHeight() );
	if( m_Composition.get() ) m_Composition->SetPos( m_nPosX, m_nPosY );
	if( m_Reading.get()     ) m_Reading->SetPos( m_nPosX, m_nPosY );
	if( m_Candidate.get()   ) m_Candidate->SetPos( m_nPosX, m_nPosY );
	if( m_Caret.get()       ) m_Caret->SetPos( m_nPosX+c_nCaretOffSet_X, m_nPosY );
	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->SetPos( m_nPosX, m_nPosY+(i*m_defRect.GetHeight()) );
}

void F3dZControlEdit::SetAlpha( float fRatio )
{  
	F3dZControl::SetAlpha( fRatio );

	if( m_Indicator.get()   ) m_Indicator->SetAlpha( fRatio );
	if( m_Composition.get() ) m_Composition->SetAlpha( fRatio );
	if( m_Reading.get()     ) m_Reading->SetAlpha( fRatio );
	if( m_Candidate.get()   ) m_Candidate->SetAlpha( fRatio );
	if( m_Caret.get()       ) m_Caret->SetAlpha( fRatio );
	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->SetAlpha( fRatio );
}

void F3dZControlEdit::SetRenderTopLevel( bool bFlag )
{  
	F3dZControl::SetRenderTopLevel( bFlag );

	if( m_Indicator.get()   ) m_Indicator->SetRenderTopLevel( bFlag );
	if( m_Composition.get() ) m_Composition->SetRenderTopLevel( bFlag );
	if( m_Reading.get()     ) m_Reading->SetRenderTopLevel( bFlag );
	if( m_Candidate.get()   ) m_Candidate->SetRenderTopLevel( bFlag );
	if( m_Caret.get()       ) m_Caret->SetRenderTopLevel( bFlag );
	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->SetRenderTopLevel( bFlag );
}

void F3dZControlEdit::SetDummyCaption( const WCHAR* pStr )
{
	if( !IsMultiLine() ) {
		if( IsPrivateMode() ) {
			std::wstring strTemp = pStr;
			if( !strTemp.empty() )
				strTemp.replace( 0, strTemp.size(), strTemp.size(), L'*' );
			F3dZControl::SetCaption( strTemp.c_str() );
		}
		else
			F3dZControl::SetCaption( pStr );
	}

	m_strDummyText = pStr;

	m_bUpdate = FALSE;
}

void F3dZControlEdit::SetItemLink( bool bFlag )
{
	m_bItemLink = bFlag;

	if( m_Caret.get() )
		m_nItemLinkEndPosX = m_Caret->GetPosX();
	if( m_Indicator.get() )
		m_nItemLinkEndPosY = m_Indicator->GetPosY();
}

void F3dZControlEdit::SetLimitWide(bool bFlag)
{
	m_pIMEInput->SetLimitWide(bFlag);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

EditableHandleMap* F3dZEditableControlEdit::s_mapEdit = NULL;

void F3dZEditableControlEdit::InitEdit()
{  
	s_mapEdit = new EditableHandleMap;
}

void F3dZEditableControlEdit::DeInitEdit()
{  
	delete s_mapEdit;
}

LRESULT CALLBACK F3dZEditableControlEdit::EditBoxProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{  
	int nRet = 0;

	return nRet; 
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


F3dZEditableControlEdit::F3dZEditableControlEdit() : m_hWnd(NULL), 	m_oldProc(NULL)
{  


}

F3dZEditableControlEdit::~F3dZEditableControlEdit()
{  
	F3dZEditableControlEdit::s_mapEdit->erase(m_hWnd);

	if( m_hWnd )
		DestroyWindow( m_hWnd );
}

void F3dZEditableControlEdit::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount < 0 ) return;

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
	     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
	else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
	else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );

	//
	m_hWnd = ::CreateWindow( L"EDIT",
		NULL,
		WS_CHILD|WS_TABSTOP|ES_AUTOHSCROLL|ES_LEFT,
		m_rect.left, m_rect.top, 
		m_rect.right  - m_rect.left, 
		m_rect.bottom - m_rect.top,
		F3dZWndManager::GetHWND(),
		NULL,
		F3dZWndManager::GetInstance(),
		NULL );	

	//
	m_oldProc = (WNDPROC)( SetWindowLong(m_hWnd, GWL_WNDPROC,(LONG)(F3dZEditableControlEdit::EditBoxProc) ) );

	//
	(*F3dZEditableControlEdit::s_mapEdit)[m_hWnd] = this;
}

void F3dZEditableControlEdit::Process( double dTime, double dMeanDT )
{  
	
}

void F3dZEditableControlEdit::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
	{
		F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	//Text
	//커서

	//IME
}

#include "FeatherXEnginePrecompile.h"

using namespace MBStringUtil;

namespace
{
	const short c_nDefComboPieceCount = 17; //기본 3조각씩 상태 1개, 버튼 1조각씩 상태 4개, 아이템 조각 9개, 선택 조각 1개( 3+4+9+1 )
	const short c_nDefSetCount      = 4;
	const short c_nDefTextCount     = 3;
	const short c_nDefItemListCount = 9;
	const short c_nDefSelectCount   = 1;
}

F3dZControlCombo::F3dZControlCombo() : m_bItemSelect(false), m_nCurrentPos(-1), m_pItemBG(NULL), m_pText(NULL), m_pBtn(NULL), m_pSelect(NULL)
{  																								 

}

F3dZControlCombo::~F3dZControlCombo()
{  

}

void F3dZControlCombo::createText( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	for( short i(0); c_nDefTextCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][i] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_Text", GetID() );

	child_control_arg.strFontName = m_strFontName;
	child_control_arg.nFontSize   = m_nFontSize;

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y;
	child_control_arg.nWidth  = control_arg.nWidth;
	child_control_arg.nHeight = control_arg.nHeight;

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_3( 1, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		pControl->SetFlag( _NONE_FLAG );
		addChildControl( pControl );

		m_pText = pControl;
	}
}

namespace
{
	const int c_nBtnOffSetX = 6;
	const int c_nBtnOffSetY = 2;
}

void F3dZControlCombo::createButton( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 3;
	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_ComboBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x+control_arg.nWidth-pTex->nWidth-c_nBtnOffSetX;
		child_control_arg.y       = control_arg.y+c_nBtnOffSetY;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pBtn = pControl;
		}
	}
}


void F3dZControlCombo::createItemBG( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 7;
	for( short i(0); c_nDefItemListCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_ItemBG", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = GetRect().left;
	child_control_arg.y       = GetRect().bottom;
	child_control_arg.nWidth  = GetRect().GetWidth();
	child_control_arg.nHeight = GetRect().GetHeight();

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_9( 1, texlist, child_control_arg );
		pControl->SetFlag( _NONE_FLAG );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );

		m_pItemBG = pControl;
	}
}

void F3dZControlCombo::ClearItem()
{  
	for( UINT i(0); m_vItem.size()>i; ++i )
	{
		delChildControl( m_vItem[i] );
	}
	m_vItem.clear();

	if( m_pItemBG )
		m_pItemBG->SetSize( GetRect().GetWidth(), GetRect().GetHeight()*(int)m_vItem.size() );

	m_bItemSelect = FALSE;
}

void F3dZControlCombo::createItemList( const WCHAR* pString )
{  
	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_%02d_Item", GetID(), (int)m_vItem.size() );

	child_control_arg.strFontName = m_strFontName;
	child_control_arg.nFontSize   = m_nFontSize;

	child_control_arg.ID      = strID;
	child_control_arg.x       = GetRect().left;
	child_control_arg.y       = GetRect().top+GetRect().GetHeight()+(GetRect().GetHeight()*(int)m_vItem.size());
	child_control_arg.nWidth  = GetRect().GetWidth();
	child_control_arg.nHeight = GetRect().GetHeight();

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_0( child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		pControl->SetFlag( _NONE_FLAG );
		addChildControl( pControl );

		pControl->SetCaption( pString );

		m_vItem.push_back( pControl );

		if( m_pItemBG )
			m_pItemBG->SetSize( GetRect().GetWidth(), GetRect().GetHeight()*(int)m_vItem.size() );
	}
}

void F3dZControlCombo::createSelect( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 16;
	for( short i(0); c_nDefSelectCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_Select", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y+control_arg.nHeight;
	child_control_arg.nWidth  = control_arg.nWidth;
	child_control_arg.nHeight = control_arg.nHeight;

	F3dZControl* pControl =	F3dZUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_1( 1, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		pControl->SetFlag( _NONE_FLAG );
		addChildControl( pControl );

		m_pSelect = pControl;
	}
}

void F3dZControlCombo::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefComboPieceCount ); 
	//사각 BG(3 조각 1셋트)	//버튼(1조작 4셋트)	//리스트 BG (9조각)	//리스트 셀렉트(1조각)
	createText( pSprite, control_arg );
	createButton( pSprite, control_arg );
	createItemBG( pSprite, control_arg );
	createSelect( pSprite, control_arg );
}

void F3dZControlCombo::Process( double dTime, double dMeanDT )
{  
	//TODO
}

void F3dZControlCombo::Render( double dTime )
{ 
	 

	if( m_pText ) m_pText->Render( dTime );
	if( m_pBtn  ) m_pBtn->Render( dTime );

	if( m_bItemSelect ) {

		if( m_pItemBG )
			m_pItemBG->Render( dTime );

		if( m_pSelect ) {
			m_pSelect->Render( dTime );
		}

		for( UINT i(0); m_vItem.size()>i; ++i ) {
			m_vItem[i]->Render( dTime );
		}
	}

	renderWire();
}

void F3dZControlCombo::AddItem( const WCHAR* pString )
{  
//	const F3dZUISprite* pSprite = F3dZUISpriteManager::GetSingleton()->GetSprite( GetSpriteName() );
	createItemList( pString );
}

int F3dZControlCombo::GetCurrentIndex()
{  
	return m_nCurrentPos;
}

int	F3dZControlCombo::SetItemIndex( int nIndex )
{  
	if( m_pText ) m_pText->SetCaption( L"" );
	if( m_vItem.empty() ) return -1;
 
	if( nIndex >= 0 && nIndex < (int)m_vItem.size() ) {
		if( m_pText ) {
			if( m_bEnable )
				m_pText->SetTextColor( m_TextColor    );
			else
				m_pText->SetTextColor( m_DisTextColor );

			m_pText->SetCaption( m_vItem[nIndex]->GetCaption() );
		}

		m_nCurrentPos = nIndex;
	}
	return m_nCurrentPos;
}

void F3dZControlCombo::OnItemButton()
{  
	m_bItemSelect = !m_bItemSelect;
	for( UINT i(0); m_vItem.size()>i; i++ ) 
	{
		m_vItem[i]->SetFlag( m_bItemSelect ? _CAPTURE_FLAG : _NONE_FLAG );
	}

	if( !m_vItem.empty() && m_pSelect )
		m_pSelect->SetPos( m_vItem[0]->GetRect().left, m_vItem[0]->GetRect().top );
}

int F3dZControlCombo::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{  
	int nRetValue = _RET_CTL_NONE;

	if(	!m_bEnable )
		return nRetValue;

	if( m_dwFlag == _NONE_FLAG )
		return nRetValue;

	if( IsFlag( _PASSED_FLAG ) )
		return nRetValue;

	// 여기에 처리
	int nOldPos = m_nCurrentPos;

	BOOL bIsOnBtn = FALSE;

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) ) {	
		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );

		if( m_pBtn )
		{
			if( m_pBtn->GetRect().IsInRect(x, y) )
			{
				//ToolTip 활성화 요청
				GetParent()->SetToolTip( IsFlag(_MSG_TOOL_TIP) ? this : NULL, GetToolTipType() );
			}
			else
			{
				//ToolTip 비활성화 요청
				if( IsFlag(_MSG_TOOL_TIP) )
					GetParent()->DisableToolTip( this, GetToolTipType() );
			}
		}
	}
	else {
		if( m_bItemSelect ) {
			switch(uMsg)
			{
			case WM_MOUSEMOVE:
				{
					for( UINT i(0); m_vItem.size()>i; i++ ) 
					{
						if( m_vItem[i]->GetRect().IsInRect(x,y) )
						{
							m_pSelect->SetPos( m_vItem[i]->GetRect().left, m_vItem[i]->GetRect().top );
							break;
						}
					}
				}
				break;
			case WM_LBUTTONUP: 
				{
					OnItemButton();

					for( UINT i(0); m_vItem.size()>i; i++ ) 
					{
						if( m_vItem[i]->GetRect().IsInRect(x,y) )
						{
							m_nCurrentPos = i;
							SetItemIndex( m_nCurrentPos );
							nRetValue |= _RET_CTL_USED;
							bIsOnBtn = TRUE;
							break;
						}
					}
				}
				break;
			}
		}

		for( UINT i(0); m_vChildList.size()>i; ++i )
			nRetValue |= m_vChildList[i]->OnMouseMsg( uMsg, x, y, nRetValue );
	}

	if( nOldPos != m_nCurrentPos )
	{
		OnControlNotify( COMBO_CHANGE, GetID(), nOldPos      , 0);
		OnControlNotify( COMBO_SELECT, GetID(), m_nCurrentPos, 0);	
	}
	else if( nOldPos == m_nCurrentPos && bIsOnBtn ) {
		OnControlNotify( COMBO_SELECT, GetID(), m_nCurrentPos, 0);	
	}

	return nRetValue;
}

void F3dZControlCombo::SetEnableComboBtn( bool bFlag )
{
	if( m_pBtn ) { bFlag ? m_pBtn->Enable() : m_pBtn->Disable(); }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZEditableControlCombo::F3dZEditableControlCombo() : m_pText(NULL), m_pBtn(NULL), m_pItem(NULL), m_pSelect(NULL) 
{													  
													  
}													  

F3dZEditableControlCombo::~F3dZEditableControlCombo()
{  

}

void F3dZEditableControlCombo::createText( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	for( short i(0); c_nDefTextCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][i] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_Text", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y;
	child_control_arg.nWidth  = control_arg.nWidth;
	child_control_arg.nHeight = control_arg.nHeight;

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_3( 1, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );

		m_pText = pControl;
	}
}

void F3dZEditableControlCombo::createButton( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 3;
	for( short i(0); c_nDefSetCount>i; ++i )
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );

	F3dZUITexture* pTex = (!texlist.empty()) ? texlist[0] : NULL;

	if( pTex ) {
		CONTROL_ARG child_control_arg;
		std::wstring strID;
		MBStringUtil::StringFormat( strID, L"%s_ComboBtn", GetID() );

		child_control_arg.ID      = strID;
		child_control_arg.x       = control_arg.x+control_arg.nWidth-pTex->nWidth-c_nBtnOffSetX;
		child_control_arg.y       = control_arg.y+c_nBtnOffSetY;
		child_control_arg.nWidth  = pTex->nWidth;
		child_control_arg.nHeight = pTex->nHeight;

		F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createButton();
		if( pControl ) {
			pControl->SetParent( GetParent() );
			pControl->createCustomControl_1( (int)texlist.size(), texlist, child_control_arg );
			pControl->SetPos( child_control_arg.x, child_control_arg.y );
			addChildControl( pControl );

			m_pBtn = pControl;
		}
	}
}

void F3dZEditableControlCombo::createItemList( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 7;
	for( short i(0); c_nDefItemListCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_Item", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y+control_arg.nHeight;
	child_control_arg.nWidth  = control_arg.nWidth;
	child_control_arg.nHeight = control_arg.nHeight;

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_9( 1, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );

		m_pItem = pControl;
	}
}

void F3dZEditableControlCombo::createSelect( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	std::vector<F3dZUITexture*> texlist;

	int nStartIdx = 16;
	for( short i(0); c_nDefSelectCount>i; ++i ) {
		texlist.push_back( pSprite->vTexList[0][i+nStartIdx] );
	}

	CONTROL_ARG child_control_arg;
	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_Select", GetID() );

	child_control_arg.ID      = strID;
	child_control_arg.x       = control_arg.x;
	child_control_arg.y       = control_arg.y;
	child_control_arg.nWidth  = control_arg.nWidth;
	child_control_arg.nHeight = control_arg.nHeight;

	F3dZEditableControl* pControl =	F3dZEditableUIFactory::GetSingleton()->createStatic();
	if( pControl ) {
		pControl->SetParent( GetParent() );
		pControl->createCustomControl_1( 1, texlist, child_control_arg );
		pControl->SetPos( child_control_arg.x, child_control_arg.y );
		addChildControl( pControl );

		m_pSelect = pControl;
	}
}

void F3dZEditableControlCombo::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount != 1 ) return;
	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	assert( nPieceCount == c_nDefComboPieceCount ); 
	//사각 BG(3 조각 1셋트)	//버튼(1조작 4셋트)	//리스트 BG (9조각)	//리스트 셀렉트(1조각)
	createText( pSprite, control_arg );
	createButton( pSprite, control_arg );
	createItemList( pSprite, control_arg );
	createSelect( pSprite, control_arg );
}

void F3dZEditableControlCombo::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	UI_RECT btn_rect, select_rect;

	std::wstring strID;
	MBStringUtil::StringFormat( strID, L"%s_Text", GetID() );
	F3dZEditableControl* pText = GetChildControl( strID.c_str() );
	if( pText ) {
		pText->SetSize( m_rect.GetWidth(), m_rect.GetHeight() );
		pText->SetPos( m_rect.left, m_rect.top );
	}

	MBStringUtil::StringFormat( strID, L"%s_ComboBtn", GetID() );
	F3dZEditableControl* pBtn = GetChildControl( strID.c_str() );
	if( pBtn ) {
		pBtn->GetRect( btn_rect );
		pBtn->SetPos( m_rect.right-btn_rect.GetWidth(), m_rect.top );
	}

	MBStringUtil::StringFormat( strID, L"%s_Item", GetID() );
	F3dZEditableControl* pItem = GetChildControl( strID.c_str() );
	if( pItem ) {
		pItem->SetSize( m_rect.GetWidth(), m_rect.GetHeight() );
		pItem->SetPos( m_rect.left, m_rect.top+m_rect.GetHeight() );
	}

	//MBStringUtil::StringFormat( strID, L"%s_Select", GetID() );
	//F3dZEditableControl* pSelect = GetChildControl( strID.c_str() );
	//if( pSelect ) {

	//}
}

void F3dZEditableControlCombo::Process( double dTime, double dMeanDT )
{  

}

void F3dZEditableControlCombo::Render( double dTime )
{  
	if( !IsShow() ) return;

	if( m_pText ) m_pText->Render( dTime );
	if( m_pBtn  ) m_pBtn->Render( dTime );

	if( m_pItem && m_bSelect ) m_pItem->Render( dTime );
}



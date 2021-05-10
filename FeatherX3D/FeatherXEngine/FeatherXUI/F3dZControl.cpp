#include "FeatherXEnginePrecompile.h"
#include "MBStringUtill.h"
 
#include "MBfasttime.h"

using namespace MBStringUtil;

//////////////////////////////////////////////////////////////////////////
namespace
{
	const short c_DefPiece1 = 1;
	const short c_DefPiece3 = 3;
	const short c_DefPiece9 = 9;
	const double c_dDownMinReact = 0.3f;
	const double c_dDownDefReact = 0.4f;
}
//////////////////////////////////////////////////////////////////////////
//static

double F3dZControl::s_dDownReact = c_dDownDefReact;

#ifdef _DEBUG
//#define _WIRE_DEBUG
#endif

void F3dZControl::GetToolTipLinkItem( int& nItemCode, int& nCount, int& nRemainTime, int& nDurability, int& nEnhance_level, std::vector< int >& vOptCode, std::vector< int >& vOptValue, int& nOptGroup, std::vector< int >& vSocketCode, std::vector< int >& vSocketValue, std::vector< BYTE >& vSocketColor, int& nSockCount, int& nbyReconState, int& nReconNum, int& nActivityValue )
{
	// 아이템 툴틉 링크 코드 얻기
	if( !m_vToolTipLinkItem.empty() )
	{
		nItemCode		= m_vToolTipLinkItem[0].nItemCode;
		nCount			= m_vToolTipLinkItem[0].nCount;
		nRemainTime		= m_vToolTipLinkItem[0].nRemainTime;
		nDurability		= m_vToolTipLinkItem[0].nDurability;
		nEnhance_level	= m_vToolTipLinkItem[0].nEnhance_level;
		vOptCode		= m_vToolTipLinkItem[0].vOptCode;
		vOptValue		= m_vToolTipLinkItem[0].vOptValue;
		nOptGroup		= m_vToolTipLinkItem[0].nOptGroup;
		vSocketCode		= m_vToolTipLinkItem[0].vSockCode;
		vSocketValue	= m_vToolTipLinkItem[0].vSockValue;
		vSocketColor	= m_vToolTipLinkItem[0].vSockColor;
		nSockCount		= m_vToolTipLinkItem[0].nSockCount;
		nbyReconState	= m_vToolTipLinkItem[0].nbyReconState;
		nReconNum		= m_vToolTipLinkItem[0].nReconNum;
		nActivityValue	= m_vToolTipLinkItem[0].nActivityValue;
	}
}

void F3dZControl::ClearToolTipLinkItem()
{
	// Item Link ToolTip 해제
	m_bClickLinkItem = FALSE;

	// 아이템 툴팁 링크 삭제
	if( !m_vToolTipLinkItem.empty() )
		m_vToolTipLinkItem.clear();
}

void F3dZControl::SearchItemToolTipLinkString( const std::wstring& strIn, int nItemCode, int nCount, int nRemainTime, int nDurability, int nEnhance_level, std::vector< int > vOptCode, std::vector< int > vOptValue, int nOptGroup, std::vector< int > vSocketCode, std::vector< int > vSocketValue, std::vector< BYTE > vSocketColor, int nSockCount, int nbyReconState, int nReconNum, int nActivityValue )
{ 

	// 아이템 툴팁 링크[] 검색 및 등록
	std::vector< std::wstring > vTextList;
	std::wstring strTmp = strIn;
	size_t pos, pos0, pos1;

	std::wstring strLine;
	while( true )
	{	
		pos0 = strTmp.find( L"[" );
		pos1 = strTmp.find( L"]" );

		if( pos0 != strTmp.npos && pos1 != strTmp.npos )
			pos = std::min( (NxU32)pos0, (NxU32)pos1 );
		else if( pos0 != strTmp.npos )
			pos = pos0;
		else if( pos1 != strTmp.npos )
			pos = pos1;
		else {
			if( !strTmp.empty() ) vTextList.push_back( strTmp );
			break;
		}

		strLine = std::wstring( strTmp.begin(), strTmp.begin() + pos );
		vTextList.push_back( strLine );
		strTmp = strTmp.substr( pos + 1, strTmp.size() );
	}

	int nLoop = (int)vTextList.size();
	DWORD dwWidth, dwHeight;
	std::vector< DWORD > vdwWidth;
	std::vector< DWORD > vdwHeight;

	for( int i(0); i < nLoop; i++ ) 
	{
		dwWidth = dwHeight = 0;
		F3dZUIText::GetStringSize( GetFontName(), GetFontSize(), IsBold(), vTextList[i].c_str(), (int)vTextList[i].length(), dwWidth, dwHeight );
		vdwWidth.push_back( dwWidth );
		vdwHeight.push_back( dwHeight );
	}

	if( !vdwWidth.empty() && vdwWidth.size() > 1 )
	{
		_ItemLink_ itemlink;
		itemlink.nWidth_Start = vdwWidth[0];
		itemlink.nWidth_End = vdwWidth[1];
		itemlink.nItemCode = nItemCode;
		itemlink.nItemCode = nItemCode;
		itemlink.nCount = nCount;
		itemlink.nRemainTime = nRemainTime;
		itemlink.nDurability = nDurability;
		itemlink.nEnhance_level = nEnhance_level;
		itemlink.vOptCode  = vOptCode;
		itemlink.vOptValue = vOptValue;
		itemlink.nOptGroup = nOptGroup;
		itemlink.vSockCode = vSocketCode;
		itemlink.vSockValue	= vSocketValue;
		itemlink.vSockColor = vSocketColor;
		itemlink.nSockCount	= nSockCount;
		itemlink.nbyReconState = nbyReconState;
		itemlink.nReconNum = nReconNum;
		itemlink.nActivityValue = nActivityValue;
		m_vToolTipLinkItem.push_back(itemlink);
	}
}

void F3dZControl::SplitStringTag( const std::wstring& strIn, std::vector<MCParser::TEXT_DATA>& outList )
{
	MCParser::DoParse( strIn.c_str(), GetFontName(), GetFontSize(), GetRect().GetWidth(), outList );
}

void F3dZControl::SplitString( const std::wstring& strIn, std::vector< std::wstring >& outList )
{ 

	//<BR>처리
	std::vector< std::wstring > vTextList;	
	std::wstring strTmp = strIn;
	size_t pos, pos0, pos1;

	{
//		_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
		std::wstring strLine;
		while( true )
		{	
			pos0 = strTmp.find( L"<BR>" );
			pos1 = strTmp.find( L"<br>" );

			if( pos0 != strTmp.npos && pos1 != strTmp.npos )
				pos = std::min( (NxU32)pos0, (NxU32)pos1 );
			else if( pos0 != strTmp.npos )
				pos = pos0;
			else if( pos1 != strTmp.npos )
				pos = pos1;
			else {
				if( !strTmp.empty() ) vTextList.push_back( strTmp );
				break;
			}

			strLine = std::wstring( strTmp.begin(), strTmp.begin() + pos );
			vTextList.push_back( strLine );
			strTmp = strTmp.substr( pos + 4, strTmp.size() );
		}
	}

	{
//		_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
		
		int nLoop = (int)vTextList.size();
		std::wstring strCur;
		DWORD dwWidth, dwHeight;
		const WCHAR *p    = NULL;

		int nLen    = 0;

		for( int i(0); nLoop>i; ++i ) {

			dwWidth = dwHeight = 0;
			strCur = L"";
			p = vTextList[i].c_str();

			{
//				_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );

				std::wstring noTagString;
				if( vTextList[i].find( L"<" ) != strTmp.npos ) {
					MCParser::RemoveTag( vTextList[i].c_str(), noTagString );
				}
				else {
					noTagString = vTextList[i];
				}

				F3dZUIText::GetStringSize( GetFontName(), GetFontSize(), IsBold(), noTagString.c_str(), (int)noTagString.length(), dwWidth, dwHeight );
				if( (int)dwWidth <= GetRect().GetWidth() ) {
					outList.push_back( vTextList[i].c_str() );
					continue;
				}
			}

			{
//				_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );

				nLen = (int)vTextList[i].length();

				for ( ; p-vTextList[i].c_str()!=nLen; p++ ) 
				{
					if( strCur.empty() && m_nCacheSplitStringSize != 0 )
					{
						int nSLoop = m_nCacheSplitStringSize - m_nCacheSplitStringSize/4;
						int nRemain = wcslen(p);
						nSLoop = std::min<int>( nSLoop, nRemain );
						if( nSLoop > 0 ) {
							for( int k(0); nSLoop>k; ++k )
							{
								strCur += *p;
								p++;
							}
							if( nSLoop >= nRemain )
							{
								break;
							}
							p--;
						}
						else {
							strCur += *p;
						}
					}
					else
					{
						strCur += *p;
					}
						
					{
//						_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
						F3dZUIText::GetStringSize( GetFontName(), GetFontSize(), IsBold(), strCur.c_str(), (int)strCur.length(), dwWidth, dwHeight );
					}

					{
//						_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
						if( (int)dwWidth == GetRect().GetWidth() )
						{
							m_nCacheSplitStringSize = strCur.length();

							outList.push_back( strCur );
							strCur = L"";
						}
						else if( (int)dwWidth > GetRect().GetWidth() )
						{
							strCur.erase( strCur.length()-1, 1 );
							outList.push_back( strCur );

							m_nCacheSplitStringSize = strCur.length();

							strCur = L"";
							--p;
						}
					}
				}

				if( !strCur.empty() ) //나머지
					outList.push_back( strCur );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

F3dZControl::F3dZControl()
: m_nDepth(0), m_pParentWnd(NULL), m_nCaptionID(0), m_nToolTipID(0), m_nSetCount(0), m_nState(F3dZUISprite::SET_NORMAL), 
m_bLeftButton(FALSE), m_bRightButton(FALSE), m_bMiddleButton(FALSE), m_bEnable(TRUE), m_vSprite(NULL), 
m_bIsVertical(FALSE), m_bWireRender(FALSE), m_bRender(TRUE), m_bRenderSkip(FALSE), m_dwFlag(_NONE_FLAG), 
m_bRenderTopLevel(false), m_dTime(0.f), m_dDownTime(0.f), m_bBillboard(false), m_bTextRoll(false), m_nToolTipType(0), m_bResizeCaption(FALSE), m_nTextureID(0),m_b3DRender(false),m_b3DCursor(false),
m_bResizeControl(false), m_bIsResizeControlStart(false), m_nResizeMin_Y(0), m_nResizeMax_Y(0),
m_bReMoveControl(false), m_bIsReMoveControlStart(false),m_bReact(true), m_bStateChange(false), m_bClickLinkItem(false), m_bSkipAlpha(false),
m_nLastLine(0), m_nLastWidth(0), m_VisibleCount(0), m_nCurScrollPos(0), m_nCacheSplitStringSize(0)
{ 
	m_nPrevMouseOffset.x = -1;
	m_nPrevMouseOffset.y = -1;

	//Caption
	m_dwTextFlag   = 0;
	m_strFontName  = L"Dotum";
	m_nFontSize    = 10;
	m_dwTextAlign  = F3dZUIText::ALIGN_HCENTER | F3dZUIText::ALIGN_VCENTER | DT_SINGLELINE;
	m_TextColor    = D3DCOLOR_XRGB( 255, 255, 255 );
	m_DisTextColor = D3DCOLOR_XRGB( 192, 192, 192 );

	m_nPosX = 0;
	m_nPosY = 0;

	SetFlag( _CAPTURE_FLAG );

	m_pDragObject = new ZUIDragAndDropObject;
}

F3dZControl::~F3dZControl()
{ 

	clearSprite();

	m_Caption.reset();

	if( m_pDragObject ) {
		delete m_pDragObject;
		m_pDragObject = NULL;
	}

	//Child
	for( UINT i(0); m_vChildList.size()>i; ++i )
		delete m_vChildList[i];
	m_vChildList.clear();
}

void F3dZControl::SetVisibleCount( const int nCount )
{
	if( nCount > 0 ) {
		m_VisibleCount = nCount;
	}
}

void F3dZControl::SetCurScrollPos( const int nPos )
{
	m_nCurScrollPos = nPos;
}

void F3dZControl::SetRotation( float fRadian )
{


	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetRotation( fRadian );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetRotation( fRadian );
}

void F3dZControl::SetClipRect( int nLeft, int nTop, int nRight, int nBottom )
{


	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetClipRect( nLeft, nTop, nRight, nBottom );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetClipRect( nLeft, nTop, nRight, nBottom );

	if( m_Caption.get() ) 
		m_Caption->SetClipRect( nLeft, nTop, nRight, nBottom );
}

void F3dZControl::SetRenderTopLevel( bool bFlag )
{ 

	m_bRenderTopLevel = bFlag;

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->bIsTop = m_bRenderTopLevel;
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetRenderTopLevel( m_bRenderTopLevel );

	if( m_Caption.get() ) m_Caption->SetRenderTopLevel( m_bRenderTopLevel );

	if( m_WireSprite.get() ) m_WireSprite->bIsTop = m_bRenderTopLevel;
}

void F3dZControl::clearSprite()
{ 

	if( m_vSprite ) {
		for( int i(0); m_nSetCount>i; i++ ) {

			//std::vector< F3dCUISpritePtr >::iterator it = m_vSprite[i].begin();
			//for( ; it != m_vSprite[i].end(); ++it )
			//	(*it).reset();

			m_vSprite[i].clear();
		}
		delete [] m_vSprite;
		m_vSprite = NULL;
	}

	m_nSetCount = 0;
}

void F3dZControl::createControl_1( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	for( int i(0); m_nSetCount>i; ++i )
	{
		const F3dZUITexture* pTex = NULL;
		if( !pSprite->vTexList[i].empty() )
		{
			pTex = pSprite->vTexList[i][0];
		}

		if( pTex ){
			create1( control_arg, pTex, i );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZControl::createControl_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	const F3dZUITexture* pTex[3];

	for( int  j(0); m_nSetCount>j; j++ )
	{
		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
			pTex[i] = pSprite->vTexList[j][i];

		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
		{
			create3( control_arg, pTex, i, j );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZControl::createControl_9( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	const F3dZUITexture* pTex[9];

	for( int j(0); m_nSetCount>j; ++j )
	{
		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
			pTex[i] = pSprite->vTexList[j][i];

		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
		{
			create9( control_arg, pTex, i, j );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}


void F3dZControl::createControl_Vertical_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;
	const F3dZUITexture* pTex[3];

	for( int  j(0); m_nSetCount>j; j++ )
	{
		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
			pTex[i] = pSprite->vTexList[j][i];

		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			if( 0 == i )
			{
				l = 0;
				t = 0;
				r = l + pTex[i]->nWidth;
				b = t + pTex[i]->nHeight;
			}
			else if( 1 == i )
			{
				l = 0;
				t = pTex[0]->nHeight;
				r = l + pTex[i]->nWidth;
				b = t + control_arg.nHeight - pTex[i-1]->nHeight - pTex[i+1]->nHeight;
			}
			else if( 2 == i )
			{
				l = 0;
				t = control_arg.nHeight - pTex[i]->nHeight; 
				r = l + pTex[i]->nWidth;
				b = t + pTex[i]->nHeight;
			}

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

			pRenderSprite->TextureID = pTex[i]->TextureID;
			pRenderSprite->b3DRender = m_b3DRender;
			pRenderSprite->b3DCursor = m_b3DCursor;
			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZControl::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
  
	SetDefValue( control_arg );

	SetTextColor( control_arg.dwTextColor );
//	SetTextBGColor( control_arg.dwTextColorBG );
	
	if( pSprite ) { 
		m_nSetCount = pSprite->nSetCount;
		m_strSprite = pSprite->strName;

		if( m_nSetCount > 0 ) {
			const F3dZUITexture* pTex = NULL;
			if( !pSprite->vTexList[0].empty() )
			{
				pTex = pSprite->vTexList[0][0];
				m_nTextureID = pTex->TextureID;
			}
		}
	}

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );

//	else {
#ifdef _WIRE_DEBUG
		m_bWireRender = TRUE;
		m_WireSprite.reset( new F3dCUISprite );

		int l, t, r, b;
		l = 0;
		t = 0;
		r = l + control_arg.nWidth;
		b = t + control_arg.nHeight;
		m_WireSprite->rtScreen.left   = l;
		m_WireSprite->rtScreen.top    = t;
		m_WireSprite->rtScreen.right  = r;
		m_WireSprite->rtScreen.bottom = b;
		m_WireSprite->b3DRender = m_b3DRender;
		m_WireSprite->b3DCursor = m_b3DCursor;
		m_WireSprite->ori_rect = UI_RECT( l, t, r, b );
#endif
//		SetPos( control_arg.x, control_arg.y );
//	}
}

void F3dZControl::ChangeSpriteAni( const F3dZUISprite* pSprite )
{ 

	clearSprite();

	m_nState = F3dZUISprite::SET_NORMAL;

	CONTROL_ARG control_arg;
	control_arg.x          = m_nPosX;
	control_arg.y          = m_nPosY;
	control_arg.ID         = m_ID;
	control_arg.nCaptionID = m_nCaptionID;
	control_arg.nWidth     = m_ori_rect.GetWidth();
	control_arg.nHeight    = m_ori_rect.GetHeight();

	CreateControl( pSprite, control_arg );

	if( m_bRenderTopLevel )
		SetRenderTopLevel( m_bRenderTopLevel );
}

//#define _WIRE_DEBUG

void F3dZControl::SetDefValue( const CONTROL_ARG& control_arg )
{ 

	m_ID         = control_arg.ID;
	m_nCaptionID = control_arg.nCaptionID;
	m_nToolTipID = control_arg.nToolTipID;

	int l, t, r, b;
	l = control_arg.x;
	t = control_arg.y;
	r = l + control_arg.nWidth;
	b = t + control_arg.nHeight;

	m_rect     = UI_RECT( l, t, r, b );
	m_ori_rect = UI_RECT( 0, 0, control_arg.nWidth, control_arg.nHeight );

	m_strFontName = control_arg.strFontName;
	m_nFontSize   = control_arg.nFontSize;

#ifdef _WIRE_DEBUG
	m_bWireRender = TRUE;
	m_WireSprite.reset( new F3dCUISprite );

	l = 0;
	t = 0;
	r = l + control_arg.nWidth;
	b = t + control_arg.nHeight;
	m_WireSprite->rtScreen.left   = l;
	m_WireSprite->rtScreen.top    = t;
	m_WireSprite->rtScreen.right  = r;
	m_WireSprite->rtScreen.bottom = b;
	m_WireSprite->b3DRender = m_b3DRender;
	m_WireSprite->b3DCursor = m_b3DCursor;

	m_WireSprite->ori_rect = UI_RECT( l, t, r, b );

	SetPos( control_arg.x, control_arg.y );
#endif
}

void F3dZControl::SetSize( const int& nWidth, const int& nHeight )
{ 

	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	if( m_bResizeCaption ) {
		if( m_Caption.get() ) m_Caption->SetSize( nWidth, nHeight );
	}

#ifdef _WIRE_DEBUG
	if( m_WireSprite.get() ) {
		int l, t, r, b;
		l = 0;
		t = 0;
		r = l + nWidth;
		b = t + nHeight;

		m_WireSprite->rtScreen.left   = l;
		m_WireSprite->rtScreen.top    = t;
		m_WireSprite->rtScreen.right  = r;
		m_WireSprite->rtScreen.bottom = b;

		m_WireSprite->ori_rect = UI_RECT( l, t, r, b );

		SetPos( m_nPosX, m_nPosY );
	}
#endif

	if( m_nSetCount <= 0 ) return;

	if( NULL != m_vSprite ) {
		int nPieceCount = (int)m_vSprite[F3dZUISprite::SET_NORMAL].size();

		if( nPieceCount == 1 )	{
			resizeControl_1( nWidth, nHeight );
		}
		else if( nPieceCount == 3 )	{
			if( IsVertical() )
				resizeControl_Vertical_3( nWidth, nHeight );
			else
				resizeControl_3( nWidth, nHeight );
		}
		else if( nPieceCount == 9 )	{
			resizeControl_9( nWidth, nHeight );
		}
	}
}

void F3dZControl::SetAlpha( float fRatio )
{ 

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetAlpha( (int)(fRatio*255.f) );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetAlpha( fRatio );

	if( m_Caption.get() ) m_Caption->SetAlpha( fRatio );
}

void F3dZControl::SetUV( const float flu, const float ftv, const float fru, const float fbv )
{
	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetUV( flu, ftv, fru, fbv );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetUV( flu, ftv, fru, fbv );
}

void F3dZControl::SetColorLeft( int r, int g, int b, int a )
{

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetColorLeft( r, g, b, a );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetColorLeft( r, g, b, a );
}

void F3dZControl::SetColorRight( int r, int g, int b, int a )
{

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetColorRight( r, g, b, a );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetColorRight( r, g, b, a );
}

void F3dZControl::SetColorTop( int r, int g, int b, int a )
{ 

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetColorTop( r, g, b, a );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetColorTop( r, g, b, a );
}

void F3dZControl::SetColorBottom( int r, int g, int b, int a )
{ 

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetColorBottom( r, g, b, a );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetColorBottom( r, g, b, a );
}

void F3dZControl::SetColor( int r, int g, int b, int a/*=255*/ )
{ 
	
	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->SetColor( r, g, b, a );
			}
		}
	}

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetColor( r, g, b, a );
}

void F3dZControl::resizeControl_1( const int& nWidth, const int& nHeight )
{ 

	int l, t, r, b;

	for( short j(0); m_nSetCount>j; ++j )
	{
		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();

			l = 0;
			t = 0;
			r = l + nWidth;
			b = t + nHeight;

			pUISprite->rtScreen.left   = l;
			pUISprite->rtScreen.top    = t;
			pUISprite->rtScreen.right  = r;
			pUISprite->rtScreen.bottom = b;

			pUISprite->ori_rect = UI_RECT( l, t, r, b );
		}
	}

	SetPos( m_nPosX, m_nPosY );
}
void F3dZControl::resizeControl_3( const int& nWidth, const int& nHeight )
{ 

	int l, t, r, b;
	const F3dCTexture* pTex[3];

	for( int  j(0); m_nSetCount>j; j++ )
	{
		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();
			pTex[i] = F3dCTextureManager::GetSingleton()->FindTexture( pUISprite->TextureID );
		}

		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();
			if(pTex[i] == NULL)
				continue;

			if( 0 == i )
			{
				l = 0;
				t = 0;
				r = l + pTex[i]->GetWidth();
				b = t + pTex[i]->GetHeight();
			}
			else if( 1 == i )
			{
				l = pTex[0]->GetWidth();
				t = 0;
				r = l + nWidth - pTex[i-1]->GetWidth() - pTex[i+1]->GetWidth();
				b = t + pTex[i]->GetHeight();
			}
			else if( 2 == i )
			{
				l = nWidth - pTex[i]->GetWidth();
				t = 0; 
				r = l + pTex[i]->GetWidth();
				b = t + pTex[i]->GetHeight();
			}

			pUISprite->rtScreen.left   = l;
			pUISprite->rtScreen.top    = t;
			pUISprite->rtScreen.right  = r;
			pUISprite->rtScreen.bottom = b;

			pUISprite->ori_rect = UI_RECT( l, t, r, b );
		}
	}

	SetPos( m_nPosX, m_nPosY );
}
void F3dZControl::resizeControl_9( const int& nWidth, const int& nHeight )
{ 

	const F3dCTexture* pTex[9];
	int nRemain = 0;
	int nIndex  = 0;
	int l, t, r, b;

	for( int j(0); m_nSetCount>j; ++j )
	{
		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();
			pTex[i] = F3dCTextureManager::GetSingleton()->FindTexture( pUISprite->TextureID );
		}

		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();

			nRemain = i/3;
			nIndex  = i%3;

			if( 0 == nIndex ) 			l = 0;
			else if( 1 == nIndex ) 		l = pTex[i-1]->GetWidth();
			else if( 2 == nIndex )		l = nWidth - pTex[i]->GetWidth();

			if( nRemain == 0 )		{
				if( 0 == nIndex ) 		t = 0; //0
				else if( 1 == nIndex ) 	t = 0; //1
				else if( 2 == nIndex )	t = 0; //2
			}
			else if( nRemain == 1 )		{
				if( 0 == nIndex )		t = pTex[0]->GetHeight(); //3
				else if( 1 == nIndex )	t = pTex[0]->GetHeight(); //4
				else if( 2 == nIndex )	t = pTex[0]->GetHeight(); //5
			}
			else if( nRemain == 2 )		{
				if( 0 == nIndex )		t = nHeight - pTex[i]->GetHeight(); //6
				else if( 1 == nIndex ) 	t = nHeight - pTex[i]->GetHeight(); //7
				else if( 2 == nIndex ) 	t = nHeight - pTex[i]->GetHeight(); //8
			}

			if( nRemain == 0 || nRemain == 2 )		{
				if( nIndex == 0 ) 			{
					r = l + pTex[i]->GetWidth();
					b = t + pTex[i]->GetHeight();
				}
				else if( nIndex == 1 ) 			{
					r = l + nWidth - pTex[i-1]->GetWidth() - pTex[i+1]->GetWidth();
					b = t + pTex[i]->GetHeight();
				}
				else if( nIndex == 2 ) 			{
					r = l + pTex[i]->GetWidth();
					b = t + pTex[i]->GetHeight();
				}
			}
			else if( nRemain == 1 )					{
				if( nIndex == 0 ) 			{
					r = l + pTex[i]->GetWidth();
					b = t + nHeight - pTex[i-3]->GetHeight() - pTex[i+3]->GetHeight();
				}
				else if( nIndex == 1 ) 			{
					r = l + nWidth  - pTex[i-1]->GetWidth()  - pTex[i+1]->GetWidth();
					b = t + nHeight - pTex[i-3]->GetHeight() - pTex[i+3]->GetHeight();
				}
				else if( nIndex == 2 ) 			{
					r = l + pTex[i]->GetWidth();
					b = t + nHeight - pTex[i-3]->GetHeight() - pTex[i+3]->GetHeight();
				}
			}

			pUISprite->rtScreen.left   = l;
			pUISprite->rtScreen.top    = t;
			pUISprite->rtScreen.right  = r;
			pUISprite->rtScreen.bottom = b;

			pUISprite->ori_rect = UI_RECT( l, t, r, b );
		}
	}

	SetPos( m_nPosX, m_nPosY );
}
void F3dZControl::resizeControl_Vertical_3( const int& nWidth, const int& nHeight )
{ 

	int l, t, r, b;
	const F3dCTexture* pTex[3];

	for( int  j(0); m_nSetCount>j; j++ )
	{
		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();
			pTex[i] = F3dCTextureManager::GetSingleton()->FindTexture( pUISprite->TextureID );
		}

		for( UINT i(0); m_vSprite[j].size()>i; ++i )
		{
			F3dCUISprite* pUISprite = m_vSprite[j][i].get();

			if( 0 == i )
			{
				l = 0;
				t = 0;
				r = l + pTex[i]->GetWidth();
				b = t + pTex[i]->GetHeight();
			}
			else if( 1 == i )
			{
				l = 0;
				t = pTex[0]->GetHeight();
				r = l + pTex[i]->GetWidth();
				b = t + nHeight - pTex[i-1]->GetHeight() - pTex[i+1]->GetHeight();
			}
			else if( 2 == i )
			{
				l = 0; 
				t = nHeight - pTex[i]->GetHeight();
				r = l + pTex[i]->GetWidth();
				b = t + pTex[i]->GetHeight();
			}

			pUISprite->rtScreen.left   = l;
			pUISprite->rtScreen.top    = t;
			pUISprite->rtScreen.right  = r;
			pUISprite->rtScreen.bottom = b;

			pUISprite->ori_rect = UI_RECT( l, t, r, b );
		}
	}

	SetPos( m_nPosX, m_nPosY );
}

void F3dZControl::createCustomControl_0( const CONTROL_ARG& control_arg )
{ 

	SetDefValue( control_arg );

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );

	m_nSetCount = 1;

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	create1( control_arg, NULL, 0 );

	SetPos( control_arg.x, control_arg.y );
}

void F3dZControl::createCustomControl_1( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{ 

	SetDefValue( control_arg );

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	for( int i(0); m_nSetCount>i; ++i ) {
		const F3dZUITexture* pTex = texList[i];
		create1( control_arg, pTex, i );
	}

	SetPos( control_arg.x, control_arg.y );
}
void F3dZControl::createCustomControl_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{ 

	SetDefValue( control_arg );

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	const F3dZUITexture* pTex[3];

	int nTexLoop = 0;

	for( int  j(0); m_nSetCount>j; j++ ) {
		for( UINT i(0); c_DefPiece3>i; ++i )
			pTex[i] = texList[nTexLoop++];

		for( UINT i(0); c_DefPiece3>i; ++i ) {
			create3( control_arg, pTex, i, j );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}
void F3dZControl::createCustomControl_Vertical_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{ 

	SetDefValue( control_arg );

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;
	const F3dZUITexture* pTex[3];

	int nTexLoop = 0;

	for( int  j(0); m_nSetCount>j; j++ ) 	{
		for( UINT i(0); c_DefPiece3>i; ++i )
			pTex[i] = texList[nTexLoop++];

		for( UINT i(0); c_DefPiece3>i; ++i ) 	{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			if( 0 == i )
			{
				l = 0;
				t = 0;
				r = l + pTex[i]->nWidth;
				b = t + pTex[i]->nHeight;
			}
			else if( 1 == i )
			{
				if( (i-1) >= 0 && (i-1) < c_DefPiece3 && 
					(i+1) >= 0 && (i+1) < c_DefPiece3 )
				{
					l = 0;
					t = pTex[0]->nHeight;
					r = l + pTex[i]->nWidth;
					b = t + control_arg.nHeight - pTex[i-1]->nHeight - pTex[i+1]->nHeight;
				}
			}
			else if( 2 == i )
			{
				l = 0;
				t = control_arg.nHeight - pTex[i]->nHeight; 
				r = l + pTex[i]->nWidth;
				b = t + pTex[i]->nHeight;
			}

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

			pRenderSprite->TextureID = pTex[i]->TextureID;
			pRenderSprite->b3DRender = m_b3DRender;
			pRenderSprite->b3DCursor = m_b3DCursor;

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZControl::create1( const CONTROL_ARG& control_arg, const F3dZUITexture* pTex, int j )
{ 

	int l, t, r, b;

	F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

	l = 0;
	t = 0;
	r = l + control_arg.nWidth;
	b = t + control_arg.nHeight;

	pRenderSprite->rtScreen.left   = l;
	pRenderSprite->rtScreen.top    = t;
	pRenderSprite->rtScreen.right  = r;
	pRenderSprite->rtScreen.bottom = b;

	pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

	if( pTex )
		pRenderSprite->TextureID   = pTex->TextureID;
	else
		pRenderSprite->TextureID   = -1;

	pRenderSprite->b3DRender = m_b3DRender;
	pRenderSprite->b3DCursor = m_b3DCursor;
	m_vSprite[j].push_back( pRenderSprite );
}

void F3dZControl::create3( const CONTROL_ARG& control_arg, const F3dZUITexture** pTex, int i, int j )
{ 

	int l, t, r, b;

	F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

	if( 0 == i )			{
		l = 0;
		t = 0;
		r = l + pTex[i]->nWidth;
		b = t + pTex[i]->nHeight;
	}
	else if( 1 == i )			{
		l = pTex[0]->nWidth;
		t = 0;
		r = l + control_arg.nWidth - pTex[i-1]->nWidth - pTex[i+1]->nWidth;
		b = t + pTex[i]->nHeight;
	}
	else if( 2 == i )			{
		l = control_arg.nWidth - pTex[i]->nWidth;
		t = 0; 
		r = l + pTex[i]->nWidth;
		b = t + pTex[i]->nHeight;
	}

	pRenderSprite->rtScreen.left   = l;
	pRenderSprite->rtScreen.top    = t;
	pRenderSprite->rtScreen.right  = r;
	pRenderSprite->rtScreen.bottom = b;

	pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

	pRenderSprite->TextureID = pTex[i]->TextureID;
	pRenderSprite->b3DRender = m_b3DRender;
	pRenderSprite->b3DCursor = m_b3DCursor;

	m_vSprite[j].push_back( pRenderSprite );
}

void F3dZControl::create9( const CONTROL_ARG& control_arg, const F3dZUITexture** pTex, int i, int j )
{ 

	int nRemain = 0;
	int nIndex  = 0;
	int l, t, r, b;

	F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

	nRemain = i/3;
	nIndex  = i%3;

	if( 0 == nIndex ) 			l = 0;
	else if( 1 == nIndex ) 		l = pTex[i-1]->nWidth;
	else if( 2 == nIndex )		l = control_arg.nWidth - pTex[i]->nWidth;

	if( nRemain == 0 )		{
		if( 0 == nIndex ) 		t = 0; //0
		else if( 1 == nIndex ) 	t = 0; //1
		else if( 2 == nIndex )	t = 0; //2
	}
	else if( nRemain == 1 )		{
		if( 0 == nIndex )		t = pTex[0]->nHeight; //3
		else if( 1 == nIndex )	t = pTex[0]->nHeight; //4
		else if( 2 == nIndex )	t = pTex[0]->nHeight; //5
	}
	else if( nRemain == 2 )		{
		if( 0 == nIndex )		t = control_arg.nHeight - pTex[i]->nHeight; //6
		else if( 1 == nIndex ) 	t = control_arg.nHeight - pTex[i]->nHeight; //7
		else if( 2 == nIndex ) 	t = control_arg.nHeight - pTex[i]->nHeight; //8
	}

	if( nRemain == 0 || nRemain == 2 )		{
		if( nIndex == 0 ) 			{
			r = l + pTex[i]->nWidth;
			b = t + pTex[i]->nHeight;
		}
		else if( nIndex == 1 ) 			{
			r = l + control_arg.nWidth - pTex[i-1]->nWidth - pTex[i+1]->nWidth;
			b = t + pTex[i]->nHeight;
		}
		else if( nIndex == 2 ) 			{
			r = l + pTex[i]->nWidth;
			b = t + pTex[i]->nHeight;
		}
	}
	else if( nRemain == 1 )					{
		if( nIndex == 0 ) 			{
			r = l + pTex[i]->nWidth;
			b = t + control_arg.nHeight - pTex[i-3]->nHeight - pTex[i+3]->nHeight;
		}
		else if( nIndex == 1 ) 			{
			r = l + control_arg.nWidth  - pTex[i-1]->nWidth  - pTex[i+1]->nWidth;
			b = t + control_arg.nHeight - pTex[i-3]->nHeight - pTex[i+3]->nHeight;
		}
		else if( nIndex == 2 ) 			{
			r = l + pTex[i]->nWidth;
			b = t + control_arg.nHeight - pTex[i-3]->nHeight - pTex[i+3]->nHeight;
		}
	}

	pRenderSprite->rtScreen.left   = l;
	pRenderSprite->rtScreen.top    = t;
	pRenderSprite->rtScreen.right  = r;
	pRenderSprite->rtScreen.bottom = b;

	pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

	pRenderSprite->TextureID = pTex[i]->TextureID;
	pRenderSprite->b3DRender = m_b3DRender;
	pRenderSprite->b3DCursor = m_b3DCursor;
	m_vSprite[j].push_back( pRenderSprite );
}

void F3dZControl::createCustomControl_9( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{ 

	SetDefValue( control_arg );

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	const F3dZUITexture* pTex[9];
	int nTexLoop = 0;

	for( int j(0); m_nSetCount>j; ++j )
	{
		for( UINT i(0); c_DefPiece9>i; ++i )
			pTex[i] = texList[nTexLoop++];

		for( UINT i(0); c_DefPiece9>i; ++i )
		{
			create9( control_arg, pTex, i, j );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZControl::CopyRect( F3dCUISprite* pSpr )
{
	if( NULL != m_vSprite )	{
		for( short j(0); m_nSetCount>j; ++j ) {
			for( UINT i(0); m_vSprite[j].size()>i; ++i ) {
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pSpr->ori_rect = pUISprite->ori_rect;
				UI_RECT::SetPosRect( pSpr->rtScreen, pUISprite->ori_rect, m_nPosX, m_nPosY );
			}
		}
	}
	else
	{
		pSpr->ori_rect = m_ori_rect;
		UI_RECT::SetPosRect( pSpr->rtScreen, m_ori_rect, m_nPosX, m_nPosY );
	}
}

void F3dZControl::OnPosChange()
{ 

	UI_RECT old_rect = m_rect;
	m_rect = m_ori_rect + UI_RECT( m_nPosX, m_nPosY, m_nPosX, m_nPosY );

	if( NULL != m_vSprite )	{
		for( short j(0); m_nSetCount>j; ++j ) {
			for( UINT i(0); m_vSprite[j].size()>i; ++i ) {
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				UI_RECT::SetPosRect( pUISprite->rtScreen, pUISprite->ori_rect, m_nPosX, m_nPosY );
			}
		}
	}
	
	OnChildPosChange( m_rect.left-old_rect.left, m_rect.top-old_rect.top );

	if( m_bWireRender && m_WireSprite.get() )
		UI_RECT::SetPosRect( m_WireSprite->rtScreen, m_WireSprite->ori_rect, m_nPosX, m_nPosY );
}

void F3dZControl::OnChildPosChange( const int nOffSetX, const int nOffSetY )
{ 

	for( UINT i(0); m_vChildList.size()>i; ++i ) {
		m_vChildList[i]->MovePosOffset( nOffSetX, nOffSetY );
		m_vChildList[i]->OnPosChange();
	}
}

void F3dZControl::Process( double dTime, double dMeanDT )
{ 

	m_dTime = dTime;

	if( m_bLeftButton || (F3dZWndManager::IsLeftBtnDown() && m_rect.IsInRect(F3dZWndManager::curX(), F3dZWndManager::curY())) ) {
		if( m_dTime-m_dDownTime >= s_dDownReact )	{
			m_dDownTime = m_dTime;
			s_dDownReact -= dMeanDT;
			if( s_dDownReact < c_dDownMinReact )
				s_dDownReact = c_dDownMinReact;
			SetDownReact( s_dDownReact );
			OnControlNotify( LBUTTON_DOWN_PRESS, GetID() );
		}
	}
	if( m_Caption.get() )
		m_Caption->Process( dTime, dMeanDT );

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->Process( dTime, dMeanDT );
}

void F3dZControl::RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer )
{

}

void F3dZControl::Render( double dTime )
{ 

}

F3dZControl* F3dZControl::GetChildControl( const WCHAR* ID )
{ 

	if( m_vChildList.empty() ) return NULL;

	for( UINT i(0); m_vChildList.size()>i; ++i ) {
		F3dZControl* pControl = m_vChildList[i];
		if( !_wcsicmp( pControl->GetID(), ID ) )
			return pControl;
	}

	return NULL;
}

void F3dZControl::delChildControl( F3dZControl* pChild )
{ 

	for( UINT i(0); m_vChildList.size()>i; ++i )
	{
		if( m_vChildList[i] == pChild )
		{
			delete m_vChildList[i];
			m_vChildList.erase( m_vChildList.begin()+i );
			return;
		}
	}
}

void F3dZControl::addChildControl( F3dZControl* pChild )
{ 

	m_vChildList.push_back( pChild );
}

void F3dZControl::renderCaption( double dTime )
{ 

	if( !m_strCaption.empty() && m_Caption.get() ) m_Caption->Render( dTime );
}

void F3dZControl::renderChild( double dTime )
{ 

	for( UINT i(0); m_vChildList.size()>i; ++i ) {
		if( m_vChildList[i]->IsShow() )
			m_vChildList[i]->Render( dTime );
	}
}

void F3dZControl::renderWire()
{ 
	//if( m_WireSprite.get() )
	//	F3dCRenderMgr::GetSingleton()->RegUIWireSprite( m_WireSprite.get() );
}

void F3dZControl::setState( F3dZUISprite::_SET_TYPE_ state )
{ 

	if( m_bStateChange )
		return;

	if( state < m_nSetCount )
	{
		m_nState = state;
	}
}

void F3dZControl::Enable()
{ 

	m_bEnable = TRUE;
	if( m_nState == F3dZUISprite::SET_DISABLE )
		setState( F3dZUISprite::SET_NORMAL );
	onEnable( m_bEnable );
}

void F3dZControl::Disable()
{ 

	m_bEnable = FALSE;
	setState( F3dZUISprite::SET_DISABLE );
	onEnable( m_bEnable );
}

void F3dZControl::ParentPosChangeNotify( int offset_x, int offset_y )
{ 

	MovePos( m_rect.left + offset_x, m_rect.top + offset_y );
}

void F3dZControl::ParentSizeChangeNotify(int nWidth,int nHeight)
{

	SetSize(nWidth,nHeight);

}

void F3dZControl::SetPos( const int& nX, const int& nY )
{ 
 
	m_nPosX = nX; m_nPosY = nY; OnPosChange(); 

	if( m_Caption.get() ) m_Caption->SetPos( m_nPosX, m_nPosY );
}

void F3dZControl::MovePosOffset( int offsetx, int offsety )
{ 

	// Region
	m_rect.left   += offsetx;	
	m_rect.top    += offsety;
	m_rect.right  += offsetx;	
	m_rect.bottom += offsety;    

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->ParentPosChangeNotify( offsetx, offsety );

	SetPos( m_rect.left, m_rect.top );

//	if( m_Caption.get() ) m_Caption->SetPos( m_nPosX, m_nPosY );
}

void F3dZControl::MovePos( int x, int y )
{ 

	int offsetx = x - m_rect.left;
	int offsety = y - m_rect.top;

	MovePosOffset( offsetx, offsety );

//	if( m_Caption.get() ) m_Caption->SetPos( m_nPosX, m_nPosY );
}

void F3dZControl::SetParent( F3dZWnd* pParentWnd )
{ 

	m_pParentWnd = pParentWnd;

	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->SetParent( pParentWnd );
}

void F3dZControl::onRefreshText( const RECT& rcTexRect )
{
	if( m_pParentWnd )
		m_pParentWnd->onRefreshText( rcTexRect );
}
void F3dZControl::SetCenter3DRender( const F3dVector& vtxCenter ,const int nWidth,const int nHeight)
{
	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();


				pUISprite->vtxCenter = vtxCenter;
				pUISprite->vtxCenter.y += nHeight;
				pUISprite->vtxCenter.x -= nWidth/2;

				pUISprite->vtxWindowScale = F3dVector((float)nWidth,(float)nHeight,0);
			}
		}
	}

	if( m_Caption.get() )
		m_Caption->SetCenter3DRender( vtxCenter ,nWidth,nHeight);
}

void F3dZControl::Set3DCursor(bool bFlag)
{
	m_b3DCursor = bFlag;

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->b3DCursor = m_b3DCursor;
			}
		}
	}

	if( m_Caption.get() )
		m_Caption->Set3DRender( bFlag );
}

void F3dZControl::Set3DRender(bool bFlag)
{
	m_b3DRender = bFlag; 

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->b3DRender = m_b3DRender;
			}
		}
	}

	if( m_Caption.get() )
		m_Caption->Set3DRender( bFlag );
}

void F3dZControl::SetBillBoard( bool bFlag )
{ 
	m_bBillboard = bFlag; 

	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->bBillboard = m_bBillboard;
			}
		}
	}

	if( m_Caption.get() )
		m_Caption->SetBillBoard( bFlag );
}

void F3dZControl::SetCenterBillBoard( const F3dVector& vtxCenter )
{
	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->vtxCenter = vtxCenter;
			}
		}
	}

	if( m_Caption.get() )
		m_Caption->SetCenter( vtxCenter ,0,0);
}

void F3dZControl::RefreshControl(BOOL bNew)
{
	int nWidth  = m_rect.GetWidth();
	int nHeight = m_rect.GetHeight();

	if( bNew ){
		m_Caption.reset( new F3dZUIText( m_strCaption.c_str(), nWidth, nHeight, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor, m_bBillboard ,m_b3DRender,m_b3DCursor, m_bTextRoll ) );
		m_Caption->SetParent( this );
		m_Caption->onRefreshText();
		m_Caption->SetRenderTopLevel( m_bRenderTopLevel );

		m_strPrevCaption = m_strCaption;
	}
	else {

		if( wcscmp( m_strCaption.c_str(), m_strPrevCaption.c_str() ) != 0 )
		{
			if( m_Caption.get() )
				m_Caption->SetText( m_strCaption.c_str(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor );
			else {
				m_Caption.reset( new F3dZUIText( m_strCaption.c_str(), nWidth, nHeight, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor, m_bBillboard ,m_b3DRender,m_b3DCursor, m_bTextRoll ) );
				m_Caption->SetParent( this );
				m_Caption->onRefreshText();
				m_Caption->SetRenderTopLevel( m_bRenderTopLevel );
			}

			m_strPrevCaption = m_strCaption;
		}
	}

	if( m_Caption.get() )
		m_Caption->SetPos( m_nPosX, m_nPosY );
}

void F3dZControl::SetCaption( const WCHAR* pStr, BOOL bNew/*=FALSE*/ )
{ 
 
	m_strCaption = pStr; 

	RefreshControl(bNew);
}

void F3dZControl::SetToolTip( const WCHAR* pStr )
{ 

	m_strToolTip = pStr; 
}

void F3dZControl::SetToolTipEx( const WCHAR* pStr, const WCHAR* pTitle )
{ 

	m_strToolTip      = pStr; 
	m_strToolTipTitle = pTitle; 
}

void F3dZControl::SetSubToolTip( const WCHAR* pStr )
{ 

	m_strSubToolTip = pStr; 
}

void F3dZControl::SetSubToolTipEx( const WCHAR* pStr, const WCHAR* pTitle )
{ 

	m_strSubToolTip      = pStr; 
	m_strSubToolTipTitle = pTitle; 
}

void F3dZControl::OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID, WPARAM wParam/*=0*/, LPARAM lParam/*=0*/ )
{ 

	if( GetParent() )
		m_pParentWnd->OnControlNotify( notify_msg, ID, wParam, lParam );
}

int F3dZControl::OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, int& nPrevRetValue )
{ 

	int nRetValue = _RET_CTL_NONE;

	if( IsEnable() == FALSE )
		return nRetValue;

	if( m_dwFlag == _NONE_FLAG )
		return nRetValue;

	if( IsFlag( _PASSED_FLAG ) )
		return nRetValue;

	return nRetValue;
}

int F3dZControl::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{ 

	int nRetValue = _RET_CTL_NONE;

	if( IsEnable() == FALSE )
		return nRetValue;

	if( m_dwFlag == _NONE_FLAG )
		return nRetValue;

	if( IsFlag( _PASSED_FLAG ) )
		return nRetValue;

	if( m_rect.IsInRect(x,y) && !( nPrevRetValue & _RET_CTL_USED ) )
	{
		switch( uMsg )
		{
		case WM_LBUTTONDBLCLK: {
				m_bLeftButton = FALSE;
				setState( F3dZUISprite::SET_OVER );
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
			
				if( IsFlag(_DRAG_FLAG) ) {

					if( GetParent() && GetParent()->GetDragObject() == NULL ) {

						// 물체의 가운데로 셋팅
						POINT ptOffset;
						ptOffset.x = (int)(GetRect().GetWidth()  * 0.5f); 
						ptOffset.y = (int)(GetRect().GetHeight() * 0.5f);
						GetParent()->SetDragObject( GetDragObject(), ptOffset );
					}
				}

				if( GetParent() )
				{
					if( IsFlag(_CAPTURE_FLAG) &&  GetParent()->SetCaptureControl(this) )
					{
						setState( F3dZUISprite::SET_DOWN );
						m_bLeftButton = TRUE;
						m_dDownTime = m_dTime;
						SetDownReact( c_dDownDefReact );
					}
				}

				if( m_bResizeControl && !m_bIsResizeControlStart && !m_bIsReMoveControlStart )
				{
					if( m_rect.IsInReSizeRect( x, y ) )
					{
						// Resize 설정
						m_bIsResizeControlStart = TRUE;
						m_nPrevMouseOffset.y = y;
					}
				}
				
				if( m_bReMoveControl && !m_bIsResizeControlStart && !m_bIsReMoveControlStart )
				{
					if( m_rect.IsInRect( x, y ) )
					{
						// Move 설정
						UI_RECT rect;
						GetParent()->GetRect( rect );
						m_nPrevMouseOffset.x = x - rect.left;
						m_nPrevMouseOffset.y = y - rect.top;
						m_bIsReMoveControlStart = TRUE;
					}
				}

				if( !m_vToolTipLinkItem.empty() )
				{
					// 아이템 링크 클릭
					for( int i(0); i < (int)m_vToolTipLinkItem.size(); i++ )
					{
						UI_RECT itemRect;
						itemRect.left = GetRect().left + m_vToolTipLinkItem[i].nWidth_Start;
						itemRect.right = itemRect.left + m_vToolTipLinkItem[i].nWidth_End;
						itemRect.top = GetRect().top;
						itemRect.bottom = GetRect().bottom;

						if( itemRect.IsInRect( x, y ) )
						{
							OnControlNotify( CLICK_ITEM_TOOLTIP, GetID() );							
							m_bIsResizeControlStart = FALSE;
							m_bIsReMoveControlStart = FALSE;
							m_bClickLinkItem		= TRUE;
						}
					}
				}

//#ifdef _DEBUG
//				LOGOUT( L"WM_LBUTTONDOWN : %s-%s", GetID(), GetParent()->GetID() );
//#endif
			}
			break;
		case WM_RBUTTONDOWN:		m_bRightButton  = TRUE;			break;
		case WM_MBUTTONDOWN:		m_bMiddleButton = TRUE;			break;
		case WM_LBUTTONUP: {
				if( m_bLeftButton )
				{
					OnControlNotify( LBUTTON_CLICK, GetID() );
					if( GetParent() )
						GetParent()->ReleaseCaptureControl(this);
				}
				m_bLeftButton = FALSE;
				// Resize 해제
				m_bIsResizeControlStart = FALSE;
				// ReMove 해제
				m_bIsReMoveControlStart = FALSE;

				if( IsFlag(_DRAG_FLAG) ) {
					if( GetParent() && GetParent()->GetDragObject()	) {
						GetParent()->SetDrop( GetParent()->GetDragObject(), this );
					}
				}
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
				//Drag and Drop
				if( m_bLeftButton  ) 
				{
				}
				else {
					if( IsFlag(_DRAG_FLAG) ) {
						if( GetParent() && GetParent()->GetDragObject()	) {
							GetParent()->MoveDrop( GetParent()->GetDragObject(), this );
						}
					}
				}

				if( m_bIsResizeControlStart )
				{
					// Resize 상태
					int nHeight = m_rect.GetHeight() + ( m_nPrevMouseOffset.y - y );

					if( m_nResizeMin_Y >= nHeight )
						nHeight = m_nResizeMin_Y;
					else if( m_nResizeMax_Y <= nHeight )
						nHeight = m_nResizeMax_Y;

					SetSize( m_rect.GetWidth(), nHeight );

					m_nPrevMouseOffset.y = y;

					GetParent()->ReSize();
				}
				else if( m_bIsReMoveControlStart )
				{
					// ReMove 상태
					int XOffset = x - m_nPrevMouseOffset.x;
					int YOffset = y - m_nPrevMouseOffset.y;

					if(XOffset != 0 || YOffset != 0) 
					{
						GetParent()->MovePos( XOffset, YOffset );
					}
					GetParent()->LimitResolution( 0, 0, 0, 0 );
				}

				if( m_bLeftButton )
					setState( F3dZUISprite::SET_DOWN );
				else {
					if( m_nState != F3dZUISprite::SET_OVER ) 
						OnControlNotify( MOUSE_OVER, GetID() );
					
					setState( F3dZUISprite::SET_OVER );
					
					if( !m_vToolTipLinkItem.empty() )
					{
						// 아이템 툴팁 링크 클릭
						for( int i(0); i < (int)m_vToolTipLinkItem.size(); i++ )
						{
							UI_RECT itemRect;
							itemRect.left = GetRect().left + m_vToolTipLinkItem[i].nWidth_Start;
							itemRect.right = itemRect.left + m_vToolTipLinkItem[i].nWidth_End;
							itemRect.top = GetRect().top;
							itemRect.bottom = GetRect().bottom;

							if( m_bClickLinkItem && itemRect.IsInRect( x, y ) )
							{
								//ToolTip 활성화 요청
								GetParent()->SetToolTip( IsFlag(_MSG_TOOL_TIP) ? this : NULL, GetToolTipType() ); 
							}
							else
							{
								// Item Link ToolTip 해제
								m_bClickLinkItem = FALSE;

								//ToolTip 비활성화 요청
								if( IsFlag(_MSG_TOOL_TIP) )
									GetParent()->DisableToolTip( this, GetToolTipType() );
							}
						}
					}
					else
					{
						// Item Link ToolTip 해제
						m_bClickLinkItem = FALSE;

						//ToolTip 활성화 요청
						GetParent()->SetToolTip( IsFlag(_MSG_TOOL_TIP) ? this : NULL, GetToolTipType() ); 
					}					
				}
			}
			break;
		}
		nRetValue = _RET_CTL_USED;
		
		//Only Drag 처리
		if( IsFlag(_MSG_SKIP_FLAG) )
			nRetValue = _RET_CTL_NONE;
	}
	else
	{
		if( uMsg == WM_LBUTTONUP )
		{
			if( m_bLeftButton && GetParent() )
				GetParent()->ReleaseCaptureControl(this);
			m_bLeftButton = FALSE;
			// Resize 해제
			m_bIsResizeControlStart = FALSE;
			// ReMove 해제
			m_bIsReMoveControlStart = FALSE;
		}
		else if( uMsg == WM_RBUTTONUP )
			m_bRightButton = FALSE;
		else if( uMsg == WM_MBUTTONUP )
			m_bMiddleButton = FALSE;
		else if( uMsg == WM_MOUSEMOVE )	{
			//ToolTip 비활성화 요청
			if( IsFlag(_MSG_TOOL_TIP) )
				GetParent()->DisableToolTip( this, GetToolTipType() );

			if( m_bIsResizeControlStart )
			{
				// Resize 상태
				int nHeight = m_rect.GetHeight() + ( m_nPrevMouseOffset.y - y );

				if( m_nResizeMin_Y >= nHeight )
					nHeight = m_nResizeMin_Y;
				else if( m_nResizeMax_Y <= nHeight )
					nHeight = m_nResizeMax_Y;

				SetSize( m_rect.GetWidth(), nHeight );

				m_nPrevMouseOffset.y = y;

				GetParent()->ReSize();
			}
			else if( m_bIsReMoveControlStart )
			{
				// ReMove 상태
				int XOffset = x - m_nPrevMouseOffset.x;
				int YOffset = y - m_nPrevMouseOffset.y;

				if(XOffset != 0 || YOffset != 0) 
				{
					GetParent()->MovePos( XOffset, YOffset );
				}
				GetParent()->LimitResolution( 0, 0, 0, 0 );
			}
		}

		if( m_nState == F3dZUISprite::SET_DOWN || m_nState == F3dZUISprite::SET_OVER )
			setState( F3dZUISprite::SET_NORMAL );
	}

	return nRetValue;
}

void F3dZControl::OnDragObjectDroped()
{

	if( m_bLeftButton ) {
		m_bLeftButton = FALSE;
		if( GetParent() )
			GetParent()->ReleaseCaptureControl(this);
	}
}

ZUIDragAndDropObject* F3dZControl::GetDragObject()
{ 

	if( m_pDragObject )
		m_pDragObject->Set( GetParent(), this );
	return m_pDragObject;
}

void F3dZControl::parseFont( const WCHAR* pStr )
{ 

	std::vector<std::wstring> datalist;
	MBStringUtil::SplitLine( datalist, pStr, L"|" );

	if( !datalist.empty() ) {
		for( UINT i(0); datalist.size()>i; ++i ) {

			if( !_wcsicmp( datalist[i].c_str(), L"default"    ) ) { 
//				m_dwTextFlag  = F3dZUIText::FLAG_SHADOW;
				m_dwTextAlign = F3dZUIText::ALIGN_HCENTER | F3dZUIText::ALIGN_VCENTER | DT_SINGLELINE;
			} //호환성 유지

			if( !_wcsicmp( datalist[i].c_str(), L"al_left"    ) ) { m_dwTextAlign  = F3dZUIText::ALIGN_LEFT;    }
			if( !_wcsicmp( datalist[i].c_str(), L"al_top"     ) ) { m_dwTextAlign |= F3dZUIText::ALIGN_TOP;     }
			if( !_wcsicmp( datalist[i].c_str(), L"al_right"   ) ) { m_dwTextAlign  = F3dZUIText::ALIGN_RIGHT;   }
			if( !_wcsicmp( datalist[i].c_str(), L"al_bottom"  ) ) { m_dwTextAlign |= F3dZUIText::ALIGN_BOTTOM;  }
			if( !_wcsicmp( datalist[i].c_str(), L"al_center"  ) ) { m_dwTextAlign  = F3dZUIText::ALIGN_HCENTER; }
			if( !_wcsicmp( datalist[i].c_str(), L"al_vcenter" ) ) { m_dwTextAlign |= F3dZUIText::ALIGN_VCENTER | DT_SINGLELINE; }

			if( !_wcsicmp( datalist[i].c_str(), L"ft_italic"  ) ) { m_dwTextFlag  = F3dZUIText::FLAG_ITALIC;  }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_bold"    ) ) { m_dwTextFlag  = F3dZUIText::FLAG_BOLD;    }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_under"   ) ) { m_dwTextFlag  = F3dZUIText::FLAG_UNDER;   }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_strike"  ) ) { m_dwTextFlag  = F3dZUIText::FLAG_STRIKE;  }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_inverse" ) ) { m_dwTextFlag  = F3dZUIText::FLAG_INVERSE; }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_shadow"  ) ) { m_dwTextFlag  = F3dZUIText::FLAG_SHADOW;  }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_outline" ) ) { m_dwTextFlag  = F3dZUIText::FLAG_OUTLINE; }
			if( !_wcsicmp( datalist[i].c_str(), L"ft_glow"    ) ) { m_dwTextFlag  = F3dZUIText::FLAG_GLOW;    }
		}
	}
}

void F3dZControl::parseStyle( const WCHAR* pStr )
{ 

	std::vector<std::wstring> datalist;
	MBStringUtil::SplitLine( datalist, pStr, L"|" );

	if( !datalist.empty() ) {
		DWORD dwFlag = _NONE_FLAG;
		for( UINT i(0); datalist.size()>i; ++i ) {

			if( !_wcsicmp( datalist[i].c_str(), L"default"    ) ) { dwFlag |= _CAPTURE_FLAG; } //호환성 유지

			if( !_wcsicmp( datalist[i].c_str(), L"st_pass"    ) ) { dwFlag |= _PASSED_FLAG;   }
			if( !_wcsicmp( datalist[i].c_str(), L"st_capture" ) ) { dwFlag |= _CAPTURE_FLAG;  }
			if( !_wcsicmp( datalist[i].c_str(), L"st_drag"    ) ) { dwFlag |= _DRAG_FLAG;     }
			if( !_wcsicmp( datalist[i].c_str(), L"st_msg_skip") ) { dwFlag |= _MSG_SKIP_FLAG; }
			if( !_wcsicmp( datalist[i].c_str(), L"st_tooltip") ) { dwFlag |= _MSG_TOOL_TIP;  }

			if( !_wcsicmp( datalist[i].c_str(), L"st_none"    ) ) { dwFlag  = _NONE_FLAG; break; }
		}
		SetFlag( dwFlag );
	}
}

void F3dZControl::SetHeightResizeControl( bool bFlag, int nMin, int nMax )
{
	// Resize 활성화
	m_bResizeControl = bFlag;
	// 최소 사이즈 설정
	m_nResizeMin_Y = nMin;
	// 최대 사이즈 설정
	m_nResizeMax_Y = nMax;
}

void F3dZControl::setStateType( int nState )
{

	if( nState < m_nSetCount )
	{
		m_nState = nState;
	}
}

void F3dZControl::setConstraintStateChange( bool bStateChange )
{
	m_bStateChange = bStateChange;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlGroup::F3dZControlGroup()
{ 

}

F3dZControlGroup::~F3dZControlGroup()
{ 

}

void F3dZControlGroup::AddControl( F3dZControl* pControl )
{ 

	if( m_rect.left   > pControl->GetRect().left   ) m_rect.left   = pControl->GetRect().left;
	if( m_rect.top    > pControl->GetRect().top    ) m_rect.top    = pControl->GetRect().top;
	if( m_rect.right  < pControl->GetRect().right  ) m_rect.right  = pControl->GetRect().right;
	if( m_rect.bottom < pControl->GetRect().bottom ) m_rect.bottom = pControl->GetRect().bottom;

	m_vFirstRectList.push_back( pControl->GetRect() );
	m_vCtrlList.push_back( pControl );
}

void F3dZControlGroup::ReFirstPos()
{ 

	m_vFirstRectList.clear();

	for( UINT i(0); m_vCtrlList.size()>i; ++i ) {
		m_vFirstRectList.push_back( m_vCtrlList[i]->GetRect() );
	}
}

void F3dZControlGroup::LastPosSave()
{ 

	m_vLastRectList.clear();

	for( UINT i(0); m_vCtrlList.size()>i; ++i ) {
		m_vLastRectList.push_back( m_vCtrlList[i]->GetRect() );
	}
}

void F3dZControlGroup::FirstPos()
{ 

	for( UINT i(0); m_vCtrlList.size()>i; ++i ) {
		m_vCtrlList[i]->SetPos( m_vFirstRectList[i].left, m_vFirstRectList[i].top );
	}
}

void F3dZControlGroup::RatioPosX( float fRatio )
{ 

	F3dVector2 vRet, vStart, vEnd;
	for( UINT i(0); m_vCtrlList.size()>i; ++i ) 
	{
		vStart.x = (float)m_vLastRectList[i].left;
		vStart.y = (float)m_vLastRectList[i].top;
		vEnd.x   = (float)m_vFirstRectList[i].left;
		vEnd.y   = vStart.y;

		Vec2Lerp( vRet, vStart, vEnd, fRatio );

		m_vCtrlList[i]->SetPos( (int)vRet.x, (int)vStart.y );
	}
}

void F3dZControlGroup::RatioPosY( float fRatio )
{ 

	F3dVector2 vRet, vStart, vEnd;
	for( UINT i(0); m_vCtrlList.size()>i; ++i ) 
	{
		vStart.x = (float)m_vLastRectList[i].left;
		vStart.y = (float)m_vLastRectList[i].top;
		vEnd.x   = vStart.x;
		vEnd.y   = (float)m_vFirstRectList[i].top;

		Vec2Lerp( vRet, vStart, vEnd, fRatio );

		m_vCtrlList[i]->SetPos( (int)vStart.x, (int)vRet.y );
	}
}

void F3dZControlGroup::RatioPos( float fRatio )
{ 

	F3dVector2 vRet, vStart, vEnd;
	for( UINT i(0); m_vCtrlList.size()>i; ++i ) 
	{
		vStart.x = (float)m_vLastRectList[i].left;
		vStart.y = (float)m_vLastRectList[i].top;
		vEnd.x   = (float)m_vFirstRectList[i].left;
		vEnd.y   = (float)m_vFirstRectList[i].top;

		Vec2Lerp( vRet, vStart, vEnd, fRatio );

		m_vCtrlList[i]->SetPos( (int)vRet.x, (int)vRet.y );
	}
}

void F3dZControlGroup::SetRect( int x, int y )
{ 

	int offsetx = x - m_rect.left;
	int offsety = y - m_rect.top;

	SetRectOffset( offsetx, offsety );
}

void F3dZControlGroup::SetRectOffset( int offsetx, int offsety )
{ 

	m_rect.left   += offsetx;	
	m_rect.top    += offsety;
	m_rect.right  += offsetx;	
	m_rect.bottom += offsety;   
}

void F3dZControlGroup::MovePos( int x, int y )
{ 

	int offsetx = x - m_rect.left;
	int offsety = y - m_rect.top;

	MovePosOffset( offsetx, offsety );
}

void F3dZControlGroup::MovePosOffset( int offsetx, int offsety )
{ 

	//m_rect.left   += offsetx;	
	//m_rect.top    += offsety;
	//m_rect.right  += offsetx;	
	//m_rect.bottom += offsety;   

	for( UINT i(0); m_vCtrlList.size()>i; ++i ) {
		m_vCtrlList[i]->MovePosOffset( offsetx, offsety );
	}
}

void F3dZControlGroup::SetShow( BOOL bShow )
{ 

	for( UINT i(0); m_vCtrlList.size()>i; ++i ) {
		m_vCtrlList[i]->SetShow( bShow );
	}
}
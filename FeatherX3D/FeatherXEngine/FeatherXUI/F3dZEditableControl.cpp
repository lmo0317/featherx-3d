#include "FeatherXEnginePrecompile.h"
#include "MBStringUtill.h"

using namespace MBStringUtil; 
namespace
{
	const short c_DefPiece1 = 1;
	const short c_DefPiece3 = 3;
	const short c_DefPiece9 = 9;
}

int F3dZEditableControl::s_nGrid = 1;

F3dZEditableControl::F3dZEditableControl()
: m_nDepth(0), m_pParentWnd(NULL), m_nSetCount(0), m_nState(F3dZUISprite::SET_NORMAL), 
  m_vSprite(NULL),
  m_bLeftButton(FALSE), 
  m_bRightButton(FALSE), 
  m_bMiddleButton(FALSE),
  m_bIsMoveAble(FALSE),
  m_bIsVertical(FALSE),
  m_bWireRender(FALSE),
  m_nPosX(0), 
  m_nPosY(0),
  m_bSelect(false),
  m_bCheck(false),
  m_bShow(true),
  m_nCaptionID(0),
  m_nToolTipID(0)
{  
	m_TextColor   = D3DCOLOR_XRGB( 255, 255, 255 );
	m_TextBGColor = D3DCOLOR_XRGB( 0, 0, 0 );

	m_bLeft    = false;
	m_bTop     = false;
	m_bRight   = false;
	m_bBottom  = false;
	m_bCenter  = false;  //default
	m_bVCenter = false;  //default

	m_bItalic  = false;
	m_bBold    = false;
	m_bUnder   = false;
	m_bStriker = false;
	m_bInverse = false;
	m_bShadow  = false;  //default
	m_bOutLine = false;
	m_bGlow    = false;

	m_bPass    = false;   
	m_bNone    = false;    
	m_bCapture = false;
	m_bDrag    = false;   
	m_bMsgSkip = false;   
	m_bToolTip = false;   

	m_dwStyle  = 0;
	m_dwFlag   = 0;
	m_dwAlign  = 0;

	m_strFontName = L"Dotum";
	m_nFontSize   = 10;
}

F3dZEditableControl::~F3dZEditableControl()
{  
	if( m_vSprite )	{
		for( int i(0); m_nSetCount>i; i++ )
			m_vSprite[i].clear();
		delete [] m_vSprite;
	}
}

void F3dZEditableControl::createControl_1( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	if( m_nSetCount <= 0 ) {
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;

	for( int i(0); m_nSetCount>i; ++i )	{
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

		if( !pSprite->vTexList[i].empty() )		{
			const F3dZUITexture* pTex = pSprite->vTexList[i][0];
			pRenderSprite->TextureID = pTex->TextureID;
		}

		m_vSprite[i].push_back( pRenderSprite );
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::createControl_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	if( m_nSetCount <= 0 ) {
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;
	const F3dZUITexture* pTex[3];

	for( int  j(0); m_nSetCount>j; j++ )	{
		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
			pTex[i] = pSprite->vTexList[j][i];

		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )		{
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

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::createControl_9( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	const F3dZUITexture* pTex[9];
	int nRemain = 0;
	int nIndex  = 0;
	int l, t, r, b;

	for( int j(0); m_nSetCount>j; ++j )	{
		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )
			pTex[i] = pSprite->vTexList[j][i];

		for( UINT i(0); pSprite->vTexList[j].size()>i; ++i )		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			nRemain = i/3;
			nIndex  = i%3;

				 if( 0 == nIndex ) 		l = 0;
			else if( 1 == nIndex ) 		l = pTex[i-1]->nWidth;
			else if( 2 == nIndex )		l = control_arg.nWidth - pTex[i]->nWidth;

				 if( nRemain == 0 )		{
					 if( 0 == nIndex ) 	t = 0; //0
				else if( 1 == nIndex ) 	t = 0; //1
				else if( 2 == nIndex )	t = 0; //2
			}
			else if( nRemain == 1 )		{
					 if( 0 == nIndex )  t = pTex[0]->nHeight; //3
				else if( 1 == nIndex )	t = pTex[0]->nHeight; //4
				else if( 2 == nIndex )	t = pTex[0]->nHeight; //5
			}
			else if( nRemain == 2 )		{
					 if( 0 == nIndex )	t = control_arg.nHeight - pTex[i]->nHeight; //6
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

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::createControl_Vertical_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
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

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::SetDefValue( const CONTROL_ARG& control_arg )
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
}

void F3dZEditableControl::createCustomControl_1( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{  
	SetDefValue( control_arg );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;

	for( int i(0); m_nSetCount>i; ++i )
	{
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

		const F3dZUITexture* pTex = texList[i];
		pRenderSprite->TextureID = pTex->TextureID;

		m_vSprite[i].push_back( pRenderSprite );
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::createCustomControl_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{
	SetDefValue( control_arg );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;
	const F3dZUITexture* pTex[3];

	int nTexLoop = 0;
	for( int j(0); m_nSetCount>j; j++ )
	{
		for( UINT i(0); c_DefPiece3>i; ++i )
			pTex[i] = texList[nTexLoop++];

		for( UINT i(0); c_DefPiece3>i; ++i )
		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			if( 0 == i ) {
				l = 0;
				t = 0;
				r = l + pTex[i]->nWidth;
				b = t + pTex[i]->nHeight;
			}
			else if( 1 == i ) {
				if( (i-1) >= 0 && (i-1) < c_DefPiece3 && 
					(i+1) >= 0 && (i+1) < c_DefPiece3 )
				{
					l = pTex[0]->nWidth;
					t = 0;
					r = l + control_arg.nWidth - pTex[i-1]->nWidth - pTex[i+1]->nWidth;
					b = t + pTex[i]->nHeight;
				}
			}
			else if( 2 == i ) {
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

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::createCustomControl_Vertical_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{  
	SetDefValue( control_arg );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b;
	const F3dZUITexture* pTex[3];

	int nTexLoop = 0;

	for( int  j(0); m_nSetCount>j; j++ )
	{
		for( UINT i(0); c_DefPiece3>i; ++i )
			pTex[i] = texList[nTexLoop++];

		for( UINT i(0); c_DefPiece3>i; ++i )
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

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::createCustomControl_9( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg )
{  
	SetDefValue( control_arg );

	m_nSetCount = nSetCount;

	if( m_nSetCount <= 0 ) 	{
		_MBASSERT(0);
		return;
	}

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	const F3dZUITexture* pTex[9];
	int nRemain = 0;
	int nIndex  = 0;
	int l, t, r, b;
	
	int nTexLoop = 0;

	for( int j(0); m_nSetCount>j; ++j )	{
		for( UINT i(0); c_DefPiece9>i; ++i )
			pTex[i] = texList[nTexLoop++];

		for( UINT i(0); c_DefPiece9>i; ++i )		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			nRemain = i/3;
			nIndex  = i%3;

			if( 0 == nIndex ) 			l = 0;
			else if( 1 == nIndex ) 		{
				if( (i-1) >= 0 && (i-1) < c_DefPiece9 )  
				{
					l = pTex[i-1]->nWidth;
				}
			}
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
					if( (i-1) >= 0 && (i-1) < c_DefPiece9 && 
						(i+1) >= 0 && (i+1) < c_DefPiece9 )
					{
						r = l + control_arg.nWidth - pTex[i-1]->nWidth - pTex[i+1]->nWidth;
						b = t + pTex[i]->nHeight;
					}
				}
				else if( nIndex == 2 ) 			{
					r = l + pTex[i]->nWidth;
					b = t + pTex[i]->nHeight;
				}
			}
			else if( nRemain == 1 )					{
				if( nIndex == 0 ) 			{
					if( (i-3) >= 0 && (i-3) < c_DefPiece9 && 
						(i+3) >= 0 && (i+3) < c_DefPiece9 )
					{
						r = l + pTex[i]->nWidth;
						b = t + control_arg.nHeight - pTex[i-3]->nHeight - pTex[i+3]->nHeight;
					}
				}
				else if( nIndex == 1 ) 			{
					if( (i-1) >= 0 && (i-1) < c_DefPiece9 && 
						(i+1) >= 0 && (i+1) < c_DefPiece9 &&
						(i-3) >= 0 && (i-3) < c_DefPiece9 && 
						(i+3) >= 0 && (i+3) < c_DefPiece9 )
					{
						r = l + control_arg.nWidth  - pTex[i-1]->nWidth  - pTex[i+1]->nWidth;
						b = t + control_arg.nHeight - pTex[i-3]->nHeight - pTex[i+3]->nHeight;
					}
				}
				else if( nIndex == 2 ) 			{
					if( (i-3) >= 0 && (i-3) < c_DefPiece9 && 
						(i+3) >= 0 && (i+3) < c_DefPiece9 )
					{
						r = l + pTex[i]->nWidth;
						b = t + control_arg.nHeight - pTex[i-3]->nHeight - pTex[i+3]->nHeight;
					}
				}
			}

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

			pRenderSprite->TextureID = pTex[i]->TextureID;

			m_vSprite[j].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );
}

void F3dZEditableControl::renderChild( double dTime )
{  
	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->Render( dTime );
}

void F3dZEditableControl::addChildControl( F3dZEditableControl* pChild )
{  
	m_vChildList.push_back( pChild );
}

F3dZEditableControl* F3dZEditableControl::GetChildControl( const WCHAR* ID )
{  
	if( m_vChildList.empty() ) return NULL;

	for( UINT i(0); m_vChildList.size()>i; ++i ) {
		F3dZEditableControl* pControl = m_vChildList[i];
		if( !_wcsicmp( pControl->GetID(), ID ) )
			return pControl;
	}

	return NULL;
}

void  F3dZEditableControl::MovePosChild( int x, int y )
{  
	for( UINT i(0); m_vChildList.size()>i; ++i )
		m_vChildList[i]->MovePos( x, y );
}

void F3dZEditableControl::SetToolTipID( int nID )
{
// 	if( nID > 0 ) {
// 		const Table* pTable = TableManager::GetSingleton()->FindTable( L"LOCAL_STRING" );
// 		if( pTable == NULL ) return;
// 
// 		const WCHAR* wszString = pTable->GetWString( nID, L"wszString" );
// 		SetToolTip( wszString );	
// 
// 		m_nToolTipID = nID;
// 	}
}

void F3dZEditableControl::SetCaptionID( int nID )
{	 
// 	if( nID > 0 ) {
// 		const Table* pTable = TableManager::GetSingleton()->FindTable( L"LOCAL_STRING" );
// 		if( pTable == NULL ) return;
// 
// 		const WCHAR* wszString = pTable->GetWString( nID, L"wszString" );
// 		SetCaption( wszString );	
// 
// 		m_nCaptionID = nID;
// 	}
}

void F3dZEditableControl::AfterCreateControl()
{  
	if( m_nCaptionID > 0 )	{
		SetCaptionID( m_nCaptionID );
	}
}

void F3dZEditableControl::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{    
	SetDefValue( control_arg );

	SetFontColor( control_arg.dwTextColor );
	SetFontBGColor( control_arg.dwTextColorBG );

	if( pSprite ) {
		m_nSetCount = pSprite->nSetCount;
		m_strSprite = pSprite->strName;
	}
	
	//Wire
	{
		m_bWireRender = TRUE;
		m_vWireSprite.reset( new F3dCUISprite );
		int l, t, r, b;
		l = 0;
		t = 0;
		r = l + control_arg.nWidth;
		b = t + control_arg.nHeight;

		m_vWireSprite->rtScreen.left   = l;
		m_vWireSprite->rtScreen.top    = t;
		m_vWireSprite->rtScreen.right  = r;
		m_vWireSprite->rtScreen.bottom = b;

		m_vWireSprite->ori_rect = UI_RECT( l, t, r, b );

		SetPos( control_arg.x, control_arg.y );
	}

	if( !control_arg.strStyle.empty() )
		parseStyle( control_arg.strStyle.c_str() );

	if( !control_arg.strFontFlag.empty() )
		parseFont( control_arg.strFontFlag.c_str() );
}

void F3dZEditableControl::OnPosChange()
{  
	m_rect = m_ori_rect + UI_RECT( m_nPosX, m_nPosY, m_nPosX, m_nPosY );

	if( NULL != m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )	{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )	{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				UI_RECT::SetPosRect( pUISprite->rtScreen, pUISprite->ori_rect, m_nPosX, m_nPosY );
			}
		}
	}

	if( m_bWireRender && m_vWireSprite.get() )
		UI_RECT::SetPosRect( m_vWireSprite->rtScreen, m_vWireSprite->ori_rect, m_nPosX, m_nPosY );
}

void F3dZEditableControl::Process( double dTime, double dMeanDT )
{  
	
}

void F3dZEditableControl::Render( double dTime )
{  
	if( m_bSelect ) {
		renderWire();
	}
}

void F3dZEditableControl::renderCaption( double dTime )
{  
	if( m_Caption.get() ) m_Caption->Render( dTime );
}

void F3dZEditableControl::renderWire()
{  
// 	if( m_vWireSprite.get() ) 
// 		F3dCRenderMgr::GetSingleton()->RegUIWireSprite( m_vWireSprite.get() );
}

void F3dZEditableControl::SetToolTip( const WCHAR* pStr )
{
	if( pStr )
		m_strToolTip = pStr; 
	else
		m_strToolTip = L"";
}

void F3dZEditableControl::SetCaption( const WCHAR* pStr ) 
{ 
	if( pStr )
		m_strCaption = pStr; 
	else
		m_strCaption = L"";

	int nWidth  = m_rect.GetWidth();
	int nHeight = m_rect.GetHeight();

	DWORD  dwFlag  = 0;
	DWORD  dwAlign = F3dZUIText::ALIGN_HCENTER|F3dZUIText::ALIGN_VCENTER|DT_SINGLELINE;

	if( m_bLeft    ) dwAlign = F3dZUIText::ALIGN_LEFT;
	if( m_bRight   ) dwAlign = F3dZUIText::ALIGN_RIGHT;
	if( m_bCenter  ) dwAlign = F3dZUIText::ALIGN_HCENTER;

	if( m_bTop     ) dwAlign |= F3dZUIText::ALIGN_TOP;
	if( m_bBottom  ) dwAlign |= F3dZUIText::ALIGN_BOTTOM;
	if( m_bVCenter ) { dwAlign |= F3dZUIText::ALIGN_VCENTER; dwAlign |= DT_SINGLELINE; }

  	if( m_bItalic  ) dwFlag |= F3dZUIText::FLAG_ITALIC;
	if( m_bBold    ) dwFlag |= F3dZUIText::FLAG_BOLD;
	if( m_bUnder   ) dwFlag |= F3dZUIText::FLAG_UNDER;
	if( m_bStriker ) dwFlag |= F3dZUIText::FLAG_STRIKE;
	if( m_bInverse ) dwFlag |= F3dZUIText::FLAG_INVERSE;
	if( m_bShadow  ) dwFlag |= F3dZUIText::FLAG_SHADOW;
	if( m_bOutLine ) dwFlag |= F3dZUIText::FLAG_OUTLINE;
	if( m_bGlow    ) dwFlag |= F3dZUIText::FLAG_GLOW;

	if( m_Caption.get() )
		m_Caption->SetText( m_strCaption.c_str(), dwFlag, m_strFontName.c_str(), m_nFontSize, dwAlign, m_TextColor );
	else
		m_Caption.reset( new F3dZUIText( m_strCaption.c_str(), nWidth, nHeight, dwFlag, m_strFontName.c_str(), m_nFontSize, dwAlign, m_TextColor, false ,false,false) );
	m_Caption->SetPos( m_nPosX, m_nPosY );
}

void F3dZEditableControl::setState( F3dZUISprite::_SET_TYPE_ state )
{  
	if( m_nSetCount > 3 ) //4가지 모두 있을때
	{
		m_nState = state;
	}
}

void F3dZEditableControl::Enable()
{  
	if( m_nState == F3dZUISprite::SET_DISABLE )
		setState( F3dZUISprite::SET_NORMAL );
}

void F3dZEditableControl::Disable()
{  
	setState( F3dZUISprite::SET_DISABLE );
}

void F3dZEditableControl::MovePos( int x, int y )
{  
	SetPos( m_rect.left + x, m_rect.top + y );

	MovePosChild( x, y );

	if( m_Caption.get() ) m_Caption->SetPos( m_nPosX, m_nPosY );
}

void F3dZEditableControl::resizeControl_1( const int& nWidth, const int& nHeight )
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
void F3dZEditableControl::resizeControl_3( const int& nWidth, const int& nHeight )
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

void F3dZEditableControl::resizeControl_Vertical_3( const int& nWidth, const int& nHeight )
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

void F3dZEditableControl::resizeControl_9( const int& nWidth, const int& nHeight )
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

				 if( 0 == nIndex ) 		l = 0;
			else if( 1 == nIndex ) 		l = pTex[i-1]->GetWidth();
			else if( 2 == nIndex )		l = nWidth - pTex[i]->GetWidth();

			if( nRemain == 0 )		{
					 if( 0 == nIndex ) 	t = 0; //0
				else if( 1 == nIndex ) 	t = 0; //1
				else if( 2 == nIndex )	t = 0; //2
			}
			else if( nRemain == 1 )		{
					 if( 0 == nIndex )  t = pTex[0]->GetHeight(); //3
				else if( 1 == nIndex )	t = pTex[0]->GetHeight(); //4
				else if( 2 == nIndex )	t = pTex[0]->GetHeight(); //5
			}
			else if( nRemain == 2 )		{
					 if( 0 == nIndex )	t = nHeight - pTex[i]->GetHeight(); //6
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

void F3dZEditableControl::SetSize( const int& nWidth, const int& nHeight )
{  
	m_rect.right  = m_rect.left + nWidth;
	m_rect.bottom = m_rect.top  + nHeight;

	m_ori_rect.right  = nWidth;
	m_ori_rect.bottom = nHeight;

	if( m_bWireRender ) {
		m_vWireSprite->rtScreen.left   = m_rect.left;
		m_vWireSprite->rtScreen.top    = m_rect.top;
		m_vWireSprite->rtScreen.right  = m_rect.right;
		m_vWireSprite->rtScreen.bottom = m_rect.bottom;
		m_vWireSprite->ori_rect        = m_ori_rect;
	}

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

void F3dZEditableControl::OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID )
{  
	if( GetParent() )
		m_pParentWnd->OnControlNotify( notify_msg, ID );
}

int F3dZEditableControl::OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{  
	int nRetValue = _RET_CTL_NONE;

	if( m_nState == F3dZUISprite::SET_DISABLE )
		return nRetValue;

	return nRetValue;
}

int F3dZEditableControl::OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue )
{  
	int nRetValue = _RET_CTL_NONE;

	if( !IsShow() ) return nRetValue;

	if( m_nState == F3dZUISprite::SET_DISABLE )
		return nRetValue;

	//Mouse Move
	if( uMsg == WM_MOUSEMOVE )
	{
		if( m_bIsMoveAble )
		{
			int XOffset = x - m_ptMovingOffset.x;
			int YOffset = y - m_ptMovingOffset.y;

			if(XOffset >= 0 || YOffset >= 0)
			{
				int offsetx = (XOffset - m_rect.left)/s_nGrid;
				int offsety = (YOffset - m_rect.top )/s_nGrid;
				MovePos( offsetx*s_nGrid, offsety*s_nGrid );
			}
			nRetValue = _RET_CTL_USED;
			return nRetValue;
		}
	}

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
				if( GetParent() )
				{
					if( GetParent()->SetCaptureControl(this) )
					{
						SetSelect( TRUE );

						setState( F3dZUISprite::SET_DOWN );
						m_bLeftButton = TRUE;
						m_bIsMoveAble = TRUE;
					}
				}

				m_ptMovingOffset.x = x - m_rect.left;
				m_ptMovingOffset.y = y - m_rect.top;
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
					m_bIsMoveAble = FALSE;
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
					setState( F3dZUISprite::SET_DOWN );
				else
					setState( F3dZUISprite::SET_OVER );

				if( m_bIsMoveAble )
				{

				}
			}
			break;
		}
		nRetValue = _RET_CTL_USED;
	}
	else
	{
		SetSelect( FALSE );

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

		m_bIsMoveAble = FALSE;

		if( m_nState == F3dZUISprite::SET_DOWN || m_nState == F3dZUISprite::SET_OVER )
			setState( F3dZUISprite::SET_NORMAL );
	}

	return nRetValue;
}

void F3dZEditableControl::parseFont( const WCHAR* pStr )
{  
	std::vector<std::wstring> datalist;
	MBStringUtil::SplitLine( datalist, pStr, L"|" );

	DWORD  dwFlag = 0;
	DWORD  dwAlign   = F3dZUIText::ALIGN_HCENTER|F3dZUIText::ALIGN_VCENTER|DT_SINGLELINE;

	for( UINT i(0); datalist.size()>i; ++i ) {
		if( !_wcsicmp( datalist[i].c_str(), L"al_left"    ) ) { m_bLeft    = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"al_top"     ) ) { m_bTop     = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"al_right"   ) ) { m_bRight   = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"al_bottom"  ) ) { m_bBottom  = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"al_center"  ) ) { m_bCenter  = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"al_vcenter" ) ) { m_bVCenter = true; }

		if( !_wcsicmp( datalist[i].c_str(), L"ft_italic"  ) ) { m_bItalic  = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"ft_bold"    ) ) { m_bBold    = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"ft_under"   ) ) { m_bUnder   = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"ft_strike"  ) ) { m_bStriker = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"ft_inverse" ) ) { m_bInverse = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"ft_shadow"  ) ) { m_bShadow  = true; } 
		if( !_wcsicmp( datalist[i].c_str(), L"ft_outline" ) ) { m_bOutLine = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"ft_glow"    ) ) { m_bGlow    = true; }
	}
}

void F3dZEditableControl::parseStyle( const WCHAR* pStr )
{  
	std::vector<std::wstring> datalist;
	MBStringUtil::SplitLine( datalist, pStr, L"|" );

	for( UINT i(0); datalist.size()>i; ++i ) {
		if( !_wcsicmp( datalist[i].c_str(), L"st_pass"    ) ) { m_bPass    = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"st_none"    ) ) { m_bNone    = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"st_capture" ) ) { m_bCapture = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"st_drag"    ) ) { m_bDrag    = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"st_msg_skip") ) { m_bMsgSkip = true; }
		if( !_wcsicmp( datalist[i].c_str(), L"st_tooltip") ) { m_bToolTip = true; }
	}
}
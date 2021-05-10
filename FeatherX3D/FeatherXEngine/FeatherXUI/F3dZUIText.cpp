#include "FeatherXEnginePrecompile.h"
#include "MBStringUtill.h"
 

//#ifdef _PACKED_FILE_
#include "MBStringUtill.h"
#include "MagiFileSystem.h"
#include "MagiStream.h"
//#endif

#include "MBfasttime.h"

using namespace MBStringUtil;

namespace 
{
	const DWORD	c_dwBlankColor    = 0x00000000;
	const DWORD	c_dwInverseColor2 = 0xFFC8C8C8;
	const DWORD	c_dwInverseColor  = 0xFFCDCDCD;

	const int	 c_nMarginX = 10; //MCGameToolTip 과 동일
	const int	 c_nMarginY = 10; //MCGameToolTip 과 동일
};

//////////////////////////////////////////////////////////////////////////
//static
HDC	F3dZUIText::s_hDC;
float F3dZUIText::s_fFreeTypeSizeRatio = 1.f;
boost::unordered_map< std::wstring, F3dZUIText::FONT_INFO* >* F3dZUIText::s_FontHash = NULL;

#ifndef TOOL_CLR
std::vector< MagiMemoryStream* > g_vTempFont;
#endif

void F3dZUIText::InitTextRender()
{	 
	s_FontHash = new boost::unordered_map< std::wstring, F3dZUIText::FONT_INFO* >;
	s_hDC = CreateCompatibleDC(NULL);
	SetMapMode( s_hDC, MM_TEXT );

	F3dZUIFreeType::CreateSingleton();
	F3dZUIFreeType::GetSingleton()->Init();
}

void F3dZUIText::DeInitTextRender()
{ 
	 
	F3dZUIFreeType::GetSingleton()->DeInit();
	F3dZUIFreeType::DestroySingleton();

	if( s_FontHash )
	{ 
		boost::unordered_map< std::wstring, F3dZUIText::FONT_INFO* >::iterator it = s_FontHash->begin();
		for(; it != s_FontHash->end(); ++it )
		{
			delete it->second;
		}
		s_FontHash->clear();

		delete s_FontHash; s_FontHash = NULL; 
	}
#ifndef TOOL_CLR
if( MagiFileSystem::IsPackedFile() ) {
	for( UINT i(0); g_vTempFont.size()>i; ++i ) {
		delete g_vTempFont[i];
	}
	g_vTempFont.clear();
}
#endif
	DeleteDC(s_hDC);
}

void F3dZUIText::GetStringSize( LPCWSTR lpszFontName, int nFontSize, BOOL bBold, LPCWSTR lpszString, int nStringLen, DWORD& dwStringWidth, DWORD& dwStringHeight )
{ 
//	_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );

	FONT_INFO* pInfo = getFont(lpszFontName, nFontSize);

//	if( pInfo && pInfo->bIsFreeType && m_bIncreaseEnglishFontSize ) nFontSize = incSizeIfNoMultiByte( lpszString, nFontSize );

	if( pInfo == NULL )
	{
		if( wcslen(lpszFontName) > 0 ) {
			addWin32Font(lpszFontName,nFontSize);
			pInfo = getFont(lpszFontName, nFontSize);
		}
		else {
			_MBASSERT(0 && "Font Invalid");
		}
		return;
	}

	if( pInfo->bIsFreeType ) {
		int nFlag = 0;
		F3dZUIFreeType::GetSingleton()->GetSize( lpszFontName, lpszString, &dwStringWidth, &dwStringHeight, nFontSize, nFlag );
		dwStringHeight = (int)(nFontSize*1.5f);
		return;
	}

	WIN32_FONT_INFO *pWin32Info = (WIN32_FONT_INFO *)pInfo;

	HFONT hFontOld;

	if(bBold)
		hFontOld = (HFONT)::SelectObject(s_hDC, pWin32Info->hBoldFont   );
	else
		hFontOld = (HFONT)::SelectObject(s_hDC, pWin32Info->hDefaultFont);

	SIZE size;
	GetTextExtentPoint32( s_hDC, lpszString, nStringLen, &size );
	dwStringWidth  = size.cx; 
	dwStringHeight = size.cy;
}

F3dZUIText::FONT_INFO* F3dZUIText::getFont( const WCHAR* pFontName, int nFontSize )
{ 
	 
	std::wstring key;
	
	//FreeType
	{
		boost::unordered_map< std::wstring, F3dZUIText::FONT_INFO* >::iterator it = s_FontHash->find( pFontName );
		if( it != s_FontHash->end() )
		{
			F3dZUIText::FONT_INFO* pInfo = it->second;
			return it->second;
		}
	}
	//BMP
	{
		MBStringUtil::StringFormat( key, L"%s_%d", pFontName, nFontSize );
		boost::unordered_map< std::wstring, F3dZUIText::FONT_INFO* >::iterator it = s_FontHash->find( key.c_str() );
		if( it != s_FontHash->end() )
		{
			F3dZUIText::FONT_INFO* pInfo = it->second;
			return it->second;
		}
	}

	return NULL;
}

F3dZUIText::FONT_INFO* F3dZUIText::addWin32Font( const WCHAR* pFontName, int nFontSize )
{ 
	 
	WIN32_FONT_INFO * pInfo = WIN32_FONT_INFO::CreateWin32Font();

	int nHeight    = -MulDiv( nFontSize, (INT)(GetDeviceCaps(s_hDC, LOGPIXELSY)), 72 );

	// 미리 폰트를 생성하자. 
	pInfo->hDefaultFont = ::CreateFont( nHeight, 0, 0, 0, FW_NORMAL, FALSE,
		FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH, pFontName );

	int nCWeight = FW_BOLD;
	////일본 비스타 버전에서 BOLD 폰트가 깨지는 문제가 발생.. 뭐냐!!!
	//if( s_bIsOsVista && ( s_strLocale.length() > 0 && _stricmp( s_strLocale.c_str(), "Shift-JIS" ) == 0 ) )
	//	nCWeight = FW_SEMIBOLD;

	pInfo->hBoldFont =      ::CreateFont( nHeight, 0, 0, 0,  nCWeight, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH, pFontName );

	pInfo->hItalicFont =    ::CreateFont( nHeight, 0, 0, 0,  nCWeight, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH, pFontName );

	pInfo->hUnderLineFont = ::CreateFont( nHeight, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH, pFontName);

	pInfo->hStrikeOutFont = ::CreateFont( nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, TRUE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH, pFontName);

	std::wstring key;
	MBStringUtil::StringFormat( key, L"%s_%d", pFontName, nFontSize );

	s_FontHash->insert( std::make_pair( key, pInfo ) );

	return pInfo;
}

F3dZUIText::FONT_INFO* F3dZUIText::AddFreeTypeFont( const WCHAR* pAlias, const WCHAR* pFontName )
{ 
	FREETYPE_FONT_INFO* pFontInfo = new FREETYPE_FONT_INFO( pAlias, pFontName );

	s_FontHash->insert( std::make_pair( pAlias, pFontInfo ) );

	return pFontInfo;
}

F3dZUIText::FREETYPE_FONT_INFO::FREETYPE_FONT_INFO( const WCHAR* pAlias, const WCHAR* pFontName )
{ 
	 
	FONT_INFO* pInfo = getFont( pAlias, -1 );
	if( pInfo )
		return;

	bIsFreeType = true;

if( MagiFileSystem::IsPackedFile() ) {

	//Second Res 우선처리
	MagiStream* pStream = MagiFileSystem::Instance(MagiFileSystem::_second)->GetStream( pFontName );

	if( pStream == NULL )
		pStream = MagiPackageFileSystem::Instance()->GetStream( pFontName );

	if( !pStream || !pStream->IsValid() )
		return;

	MagiMemoryStream* pMemStream = new MagiMemoryStream( (size_t)pStream->GetLength() );
	pStream->Read( pMemStream->GetBuf(), (size_t)pStream->GetLength() );

	if( !F3dZUIFreeType::GetSingleton()->LoadFontBuffer( pAlias, pMemStream->GetBuf(), (size_t)pStream->GetLength() ) ) {
		return;
	}
	pStream->DecRef();
#ifndef TOOL_CLR
	g_vTempFont.push_back( pMemStream );
#endif
}
else {
	char szTemp[MAX_PATH];
	WC2AC( szTemp, MAX_PATH, pFontName, (int)wcslen(pFontName)+1 );

	if( !F3dZUIFreeType::GetSingleton()->LoadFont( pAlias, szTemp ) ) {
		return;
	}
}
}

//////////////////////////////////////////////////////////////////////////
F3dZUIText::F3dZUIText( const WCHAR* pStr, int nWidth, int nHeight, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color, bool bBillboard,bool b3DRender,bool b3DCursor, bool bTextRoll )
	: m_Sprite(new F3dCUISprite), m_nRollDegree(0), m_bURoll(bTextRoll), m_pParent(NULL), m_dRollTime(0.f)
{ 
	m_editTextSize.cx = 0;
	m_editTextSize.cy = 0; 

	m_bBillboard = bBillboard;
	m_b3DRender = b3DRender;
	m_b3DCursor = b3DCursor;
	m_nPosX = 0;
	m_nPosY = 0;

	m_dwWidth  = nWidth;
	m_dwHeight = nHeight;

	int nExpX, nExpY;

	//if( m_bURoll ) {
	//	//글씨가 더 클수 있다.
	//	DWORD dwStrWidth = 0, dwStrHeight = 0;
	//	GetStringSize( lpszFontName, nFontSize, (dwFlag & FLAG_BOLD), pStr, wcslen(pStr), dwStrWidth, dwStrHeight );

	//	if( dwStrWidth > 0 && dwStrHeight > 0 )
	//		F3dCTextureManager::GetSquareSize( dwStrWidth, dwStrHeight, nExpX, nExpY );
	//	else
	//		F3dCTextureManager::GetSquareSize( m_dwWidth, m_dwHeight, nExpX, nExpY );
	//}
	//else {
		F3dCTextureManager::GetSquareSize( m_dwWidth, m_dwHeight, nExpX, nExpY );
	//}

	m_dwTexWidth  = nExpX;
	m_dwTexHeight = nExpY;   //크기

	std::wstring strText = pStr;

	//Texture
	if( 0 == m_Sprite->TextureID )
	{
		m_Sprite->TextureID = F3dCTextTextureManager::GetSingleton()->CreateTexture( nExpX, nExpY, D3DFMT_A8R8G8B8 );
		m_Sprite->bText = true;
		F3dCTexture* pTexture = F3dCTextTextureManager::GetSingleton()->FindTexture( m_Sprite->TextureID );
		pTexture->SetName( strText.c_str() );
	}

	RECT rcRect;
	SetSprite( rcRect, m_rcTexRect );
	m_Sprite->bBillboard = m_bBillboard;
	m_Sprite->b3DRender = m_b3DRender;
	m_Sprite->b3DCursor = m_b3DCursor;

	FONT_INFO * pBaseInfo = getFont(lpszFontName, nFontSize);
	if( pBaseInfo ) {
		if( pBaseInfo->bIsFreeType ) {
			drawFreeType( pStr, dwFlag, lpszFontName, nFontSize, dwAlign, color, rcRect, m_rcTexRect );
			return;
		}
	}

	drawBitmapType( pBaseInfo, pStr, lpszFontName, nFontSize, dwFlag, dwAlign, color, rcRect, m_rcTexRect );
}

F3dZUIText::~F3dZUIText()
{ 
	 
	
}

void F3dZUIText::SetBillBoard( bool bFlag )
{
	m_bBillboard = bFlag;
	m_Sprite->bBillboard = m_bBillboard;
}

void F3dZUIText::Set3DCursor(bool bFlag)
{
	m_b3DCursor = bFlag;
	m_Sprite->b3DCursor = m_b3DCursor;
}

void F3dZUIText::Set3DRender(bool bFlag)
{
	m_b3DRender = bFlag;
	m_Sprite->b3DRender = m_b3DRender;
}

void F3dZUIText::SetSprite( RECT& rcRect, RECT& rcTexRect )
{ 
	 
	rcRect.left   = 0; 
	rcRect.top    = 0; 
	rcRect.right  = m_dwWidth; 
	rcRect.bottom = m_dwHeight;

	rcTexRect.left   = 0; 
	rcTexRect.top    = 0; 
	rcTexRect.right  = m_dwWidth; 
	rcTexRect.bottom = m_dwHeight;

	m_Sprite->lu = 0.f;
	m_Sprite->tv = 0.f;
	m_Sprite->ru = (float)m_dwWidth/m_dwTexWidth;
	m_Sprite->bv = (float)m_dwHeight/m_dwTexHeight;

	m_Sprite->back_lu = m_Sprite->lu;
	m_Sprite->back_tv = m_Sprite->tv;
	m_Sprite->back_ru = m_Sprite->ru;
	m_Sprite->back_bv = m_Sprite->bv;

	m_Sprite->ori_rect.left   = 0;
	m_Sprite->ori_rect.top    = 0;
	m_Sprite->ori_rect.right  = m_dwWidth;
	m_Sprite->ori_rect.bottom = m_dwHeight;

	UI_RECT::SetPosRect( m_Sprite->rtScreen, m_Sprite->ori_rect, m_nPosX, m_nPosY );
}

void F3dZUIText::drawFreeType( const WCHAR* pStr, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color, RECT& rcRect, RECT& rcTexRect )
{ 
	 
	DWORD    dwWidth, dwHeight;   //크기
	dwWidth  = m_dwWidth;
	dwHeight = m_dwHeight;

	int nFlag = 0;
	if( dwFlag & FLAG_BOLD    ) nFlag  = F3dZUIFreeType::FT_BOLD;
	if( dwFlag & FLAG_INVERSE ) {
		F3dZUIFreeType::GetSingleton()->SetBGColor( c_dwInverseColor2 );
	}
	if( dwFlag & FLAG_SHADOW  ) {
		nFlag |= F3dZUIFreeType::FT_SHADOW;
		F3dZUIFreeType::GetSingleton()->SetBGColor( 0xff000000 );
	}
	if( dwFlag & FLAG_OUTLINE ) { 
		nFlag |= F3dZUIFreeType::FT_OUTLINE;
		F3dZUIFreeType::GetSingleton()->SetBGColor( 0xff000000 );
	}
	if( dwFlag & FLAG_GLOW    ) {
		nFlag |= F3dZUIFreeType::FT_GLOW;
		F3dZUIFreeType::GetSingleton()->SetBGColor( 0xffffffff );
	}

	F3dZUIFreeType::GetSingleton()->SetColor( color );

	DWORD nWidth, nHeight;
	nWidth = nHeight = 0;
	const ARGBBuffer *pImg = F3dZUIFreeType::GetSingleton()->Draw( lpszFontName, pStr, &nWidth, &nHeight, (int)(nFontSize*s_fFreeTypeSizeRatio), nFlag, (dwFlag & FLAG_UNDER), (dwFlag & FLAG_INVERSE) );

	{
		DWORD dwStringWidth  = nWidth;
		DWORD dwStringHeight = nHeight;
///		F3dZUIFreeType::GetSingleton()->GetSize( lpszFontName, pStr, &dwStringWidth, &dwStringHeight, nFontSize, nFlag );
		dwStringHeight = (int)(nFontSize*1.5f);

		m_editTextSize.cx = dwStringWidth; 
		m_editTextSize.cy = dwStringHeight;
	}

	if( pImg == NULL ) return;		

	DWORD dwTexWidth  = (DWORD)pImg->GetWidth();
	DWORD dwTexHeight = (DWORD)pImg->GetHeight();

	//W
	if( (dwAlign & ALIGN_LEFT) == 0 ) {
		rcTexRect.left   = 0; 
		rcTexRect.right  = dwTexWidth; 
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}
	if( dwAlign & ALIGN_RIGHT ) {
		rcTexRect.left   = dwWidth-dwTexWidth-5; 
		rcTexRect.right  = dwWidth-5; 
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}
	if( dwAlign & ALIGN_HCENTER ) {
		rcTexRect.left   = dwWidth/2-dwTexWidth/2; 
		rcTexRect.right  = dwWidth/2+dwTexWidth/2; 
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}

	//H
	if( dwAlign & ALIGN_VCENTER ) {
		rcTexRect.top    = dwHeight/2-dwTexHeight/2;
		rcTexRect.bottom = dwHeight/2+dwTexHeight/2;
	}
	if( dwAlign == ALIGN_TOP ) {
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}
	if( dwAlign & ALIGN_BOTTOM ) {
		rcTexRect.top    = dwHeight-dwTexHeight;
		rcTexRect.bottom = dwHeight;
	}

	//Copy
	int nStride;
	char* pImgBuf32 = NULL;
	F3dCTexture* pTexture = F3dCTextTextureManager::GetSingleton()->FindTexture( m_Sprite->TextureID );
	pTexture->LockRect(&rcRect, reinterpret_cast<void **>(&pImgBuf32),nStride);

	if( NULL == pImgBuf32 )
		return;

	const ARGBBuffer::Pixel *p = pImg->GetBuffer();
	int nSrcStride = dwTexWidth;

	DWORD* pDstColor;
	DWORD* pSrcColor;

	for( int y = 0; y < rcRect.bottom; y++ )
	{
		pDstColor = ( DWORD* )pImgBuf32;

		if( y >= rcTexRect.top && y < rcTexRect.bottom )
		{
			pSrcColor = ( DWORD* )p;

			for( int x = 0; x < rcRect.right; x++ )
			{
				if( x >= rcTexRect.left && x < rcTexRect.right )
					pDstColor[x] = pSrcColor[x-rcTexRect.left];
				else
					pDstColor[x] = c_dwBlankColor;
			}
			p         += nSrcStride;
		}
		else
		{
			for( int x = 0; x < rcRect.right; x++ )
			{
				pDstColor[x] = c_dwBlankColor;
			}
		}
		pImgBuf32 += nStride;
	}

	pTexture->Unlock();

#ifdef _DEBUG
	static bool bSaveFont = false;
	if( bSaveFont ) {
		pTexture->SaveTexture( L"c:\\font_test.tga" );
		bSaveFont = false;
	}
#endif
}

void F3dZUIText::drawBitmapType( FONT_INFO * pBaseInfo, const WCHAR* pStr, const WCHAR* lpszFontName, int nFontSize, DWORD dwFlag, DWORD dwAlign, const F3dColor& color, RECT& rcRect, RECT& rcTexRect )
{ 
	 
	std::wstring strText = pStr;

	DWORD    dwWidth, dwHeight;   //크기
	dwWidth  = m_dwWidth;
	dwHeight = m_dwHeight;

	BYTE *pBitmapBuffer = NULL;

	BITMAPINFO bmi;
	ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER) );
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = dwWidth;
	bmi.bmiHeader.biHeight      = -int(dwHeight);
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount    = 32;

	HBITMAP hBmBitmap = CreateDIBSection( s_hDC, &bmi, DIB_RGB_COLORS, (VOID**)&pBitmapBuffer, NULL, 0 );
	DWORD dwStride = dwWidth * 4;
	if ( dwStride % 4 )
		dwStride = ((dwStride / 4) + 1) * 4;

	ZeroMemory( pBitmapBuffer, dwStride * dwHeight );

	WIN32_FONT_INFO* pInfo = (WIN32_FONT_INFO *)pBaseInfo;
	if( !pInfo )
		pInfo = (WIN32_FONT_INFO*)addWin32Font(lpszFontName, nFontSize);

	HBITMAP hBitmapOld = reinterpret_cast<HBITMAP>( ::SelectObject(s_hDC, hBmBitmap));
	HFONT hFontOld;

	// Font Flag Setting
	if(dwFlag & FLAG_ITALIC)
		hFontOld = reinterpret_cast<HFONT>(::SelectObject(s_hDC, pInfo->hItalicFont    ) );
	else if(dwFlag & FLAG_BOLD)
		hFontOld = reinterpret_cast<HFONT>(::SelectObject(s_hDC, pInfo->hBoldFont      ) );
	else if(dwFlag & FLAG_UNDER)
		hFontOld = reinterpret_cast<HFONT>(::SelectObject(s_hDC, pInfo->hUnderLineFont ) );
	else if(dwFlag & FLAG_STRIKE)
		hFontOld = reinterpret_cast<HFONT>(::SelectObject(s_hDC, pInfo->hStrikeOutFont ) );
	else
		hFontOld = reinterpret_cast<HFONT>(::SelectObject(s_hDC, pInfo->hDefaultFont   ) );

	SetTextAlign(s_hDC, TA_TOP );

	DWORD dwColor = RGB(255,255,255);
	SetBkMode( s_hDC, TRANSPARENT );
	SetBkColor( s_hDC, c_dwBlankColor );
	SetTextColor( s_hDC, dwColor );

	int nDTHeight = ::DrawText(s_hDC, strText.c_str(), static_cast<int>(strText.size()), &rcRect, dwAlign|DT_NOPREFIX);

	GetTextExtentPoint32(s_hDC, strText.c_str(), static_cast<int>(strText.size()), &m_editTextSize);

	//Copy
	int nStride;
	unsigned int i;
	DWORD* pImgBuf32 = NULL;
	F3dCTexture* pTexture = F3dCTextTextureManager::GetSingleton()->FindTexture( m_Sprite->TextureID );
	pTexture->LockRect(&rcTexRect, reinterpret_cast<void **>(&pImgBuf32),nStride);

	if( NULL == pImgBuf32 )
		return;

	nStride /= sizeof(DWORD);

	//Text Preprocess
	int textoffset = 0;

	if ( dwFlag & FLAG_OUTLINE )	{
		int so = 1;
		textoffset = 1;

		DWORD dwShadowColor = DWORD((0xf0 << 24) | (0x40 << 16) | (0x40 << 8) | 0x40);
		for(i = 0; i < dwHeight; ++i)	{
			DWORD *pImg = (DWORD*)&pBitmapBuffer[i*dwStride];
			for(unsigned int j = 0; j < dwWidth; ++j)	{
				if( *pImg++ )	{
					if( i + so - 1 < dwHeight )		{
						if( j + so - 1 < dwWidth ) *(pImgBuf32 + (i+so-1) * nStride + (j+so-1)) = dwShadowColor;
						if( j + so     < dwWidth ) *(pImgBuf32 + (i+so-1) * nStride + (j+so))   = dwShadowColor;
						if( j + so + 1 < dwWidth ) *(pImgBuf32 + (i+so-1) * nStride + (j+so+1)) = dwShadowColor;
					}

					if( i + so < dwHeight )		{
						if( j + so - 1 < dwWidth ) *(pImgBuf32 + (i+so) * nStride + (j+so-1)) = dwShadowColor;
						if( j + so + 1 < dwWidth ) *(pImgBuf32 + (i+so) * nStride + (j+so+1)) = dwShadowColor;
					}

					if( i + so + 1 < dwHeight )		{
						if( j + so - 1 < dwWidth ) *(pImgBuf32 + (i+so+1) * nStride + (j+so-1)) = dwShadowColor;
						if( j + so     < dwWidth ) *(pImgBuf32 + (i+so+1) * nStride + (j+so))   = dwShadowColor;
						if( j + so + 1 < dwWidth ) *(pImgBuf32 + (i+so+1) * nStride + (j+so+1)) = dwShadowColor;
					}
				}
			}
		}
	}

	else if ( dwFlag & FLAG_SHADOW )	{
		DWORD dwShadowColor = DWORD((0xff << 24) | (0x40 << 16) | (0x40 << 8) | 0x40);
		for(i = 0; i < dwHeight; ++i)	{
			DWORD *pImg = (DWORD*)&pBitmapBuffer[i*dwStride];
			for(unsigned int j = 0; j < dwWidth; ++j)	{
				if( *pImg++ )	{
					if( i != dwHeight - 1 && j != dwWidth - 1 )
						*(pImgBuf32 + (i+1) * nStride + (j+1)) = dwShadowColor;
				}
			}
		}
	}

	//Text
	for(i = 0; i < dwHeight; ++i)	{
		DWORD *pImg = (DWORD*)&pBitmapBuffer[i*dwStride];
		for(unsigned int j = 0; j < dwWidth; ++j)	{
			if( *pImg++ )	{
				if( i + textoffset < dwHeight && j + textoffset < dwWidth )		{
					*(pImgBuf32 + (i+textoffset) * nStride + (j+textoffset)) = color;
				}
			}
			else if( dwFlag & FLAG_INVERSE )	{	
				if( i + textoffset + 3 < dwHeight && j + textoffset < dwWidth )		{
					*(pImgBuf32 + (i+textoffset) * nStride + (j+textoffset)) = c_dwInverseColor;
				}
			}
			else { //아무것도 없으면 투명
				*(pImgBuf32 + (i+textoffset) * nStride + (j+textoffset)) = c_dwBlankColor;
			}
		}
	}

	pTexture->Unlock();

	SelectObject(s_hDC, hFontOld);
	SelectObject(s_hDC, hBitmapOld);
	DeleteObject(hBmBitmap);
}

void F3dZUIText::onRefreshText()
{
	onRefreshText( m_rcTexRect );
}

void F3dZUIText::onRefreshText( const RECT& rcTexRect )
{
	if( m_pParent )
		m_pParent->onRefreshText( rcTexRect );
}

void F3dZUIText::SetClipRect( int nLeft, int nTop, int nRight, int nBottom )
{
	m_Sprite->SetClipRect( nLeft, nTop, nRight, nBottom );
}

//Texture 재활용
void F3dZUIText::SetText( const WCHAR* pStr, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color )
{ 
	 
	m_nPosX = 0;
	m_nPosY = 0;

	RECT rcRect;
	SetSprite( rcRect, m_rcTexRect );

	FONT_INFO * pBaseInfo = getFont(lpszFontName, nFontSize);
	bool bInit = false;
	if( pBaseInfo ) {
		if( pBaseInfo->bIsFreeType ) {
			drawFreeType( pStr, dwFlag, lpszFontName, nFontSize, dwAlign, color, rcRect, m_rcTexRect );
			bInit = true;
		}
	}

	if( !bInit ) {
		drawBitmapType( pBaseInfo, pStr, lpszFontName, nFontSize, dwFlag, dwAlign, color, rcRect, m_rcTexRect );
	}

	onRefreshText( m_rcTexRect );
}

void F3dZUIText::SetRenderTopLevel( bool bFlag )
{ 
	 
	m_Sprite->bIsTop = bFlag;
}

void F3dZUIText::SetAlpha( float fRatio )
{ 
	 
	m_Sprite->SetAlpha( (int)(fRatio*255.f) );
}

void F3dZUIText::Process( double dTime, double dMeanDT )
{ 
	 
	if( !m_bURoll ) return;

	//if( dTime-m_dRollTime >= 0.1 ) {
	//	++m_nRollDegree;
	//	m_dRollTime = dTime;
	//}
	//if( m_nRollDegree > 360 ) {
	//	m_nRollDegree = 0;
	//	m_Sprite->lu = m_Sprite->back_lu;
	//	m_Sprite->ru = m_Sprite->back_ru;
	//}

	//float diff = m_Sprite->lu;
	//m_Sprite->lu = (float)(m_Sprite->back_lu + sinf( ToRadian( (float)m_nRollDegree ) ) * (m_Sprite->back_ru - m_Sprite->back_lu));
	//m_Sprite->ru += (m_Sprite->lu - diff);
}

void F3dZUIText::Render( double dTime )
{ 
 	F3dCRenderMgr::GetSingleton()->RegUISprite( m_Sprite.get() );
}

void F3dZUIText::SetSize( const int nWidth, const int nHeight )
{
	 

	int l, t, r, b;
	l = 0;
	t = 0;
	r = l + nWidth;
	b = t + nHeight;

	m_Sprite->rtScreen.left   = l;
	m_Sprite->rtScreen.top    = t;
	m_Sprite->rtScreen.right  = r;
	m_Sprite->rtScreen.bottom = b;

	m_Sprite->ori_rect = UI_RECT( l, t, r, b );
}

void F3dZUIText::SetPos( const int nPosX, const int nPosY )
{ 
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	UI_RECT::SetPosRect( m_Sprite->rtScreen, m_Sprite->ori_rect, m_nPosX, m_nPosY );
}

void F3dZUIText::SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight)
{
	m_Sprite->vtxCenter = vtxCenter;
	m_Sprite->vtxCenter.y += nHeight;
	m_Sprite->vtxCenter.x -= nWidth/2;

	m_Sprite->vtxWindowScale = F3dVector( (float)nWidth, (float)nHeight, 0.f );
}

void F3dZUIText::SetCenter( const F3dVector& vtxCenter ,const int nWidth,const int nHeight)
{
	m_Sprite->vtxCenter = vtxCenter;
	m_Sprite->vtxCenter.y += nHeight;
	m_Sprite->vtxCenter.x -= nWidth/2;

	m_Sprite->vtxWindowScale = F3dVector( (float)nWidth, (float)nHeight, 0.f );
}

void F3dZUIText::drawFreeTypeToTexture( const WCHAR* pStr, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color, RECT& rcRect, RECT& rcTexRect,F3dCTexturePtr pInTexture )
{
	DWORD    dwWidth, dwHeight;   //크기
	dwWidth  = 0;
	dwHeight = 0;

	int nFlag = 0;
	if( dwFlag & FLAG_BOLD    ) nFlag  = F3dZUIFreeType::FT_BOLD;
	if( dwFlag & FLAG_INVERSE ) {
		F3dZUIFreeType::GetSingleton()->SetBGColor( c_dwInverseColor2 );
	}
	if( dwFlag & FLAG_SHADOW  ) {
		nFlag |= F3dZUIFreeType::FT_SHADOW;
		F3dZUIFreeType::GetSingleton()->SetBGColor( 0xff000000 );
	}
	if( dwFlag & FLAG_OUTLINE ) {
		nFlag |= F3dZUIFreeType::FT_OUTLINE;
		F3dZUIFreeType::GetSingleton()->SetBGColor( 0xff000000 );
	}
	if( dwFlag & FLAG_GLOW    ) {
		nFlag |= F3dZUIFreeType::FT_GLOW;
		F3dZUIFreeType::GetSingleton()->SetBGColor( 0xffffffff );
	}

	F3dZUIFreeType::GetSingleton()->SetColor( color );

	DWORD nWidth, nHeight;
	const ARGBBuffer *pImg = F3dZUIFreeType::GetSingleton()->Draw( lpszFontName, pStr, &nWidth, &nHeight, (int)(nFontSize*s_fFreeTypeSizeRatio), nFlag, (dwFlag & FLAG_UNDER), (dwFlag & FLAG_INVERSE) );

	{
		DWORD dwStringWidth  = nWidth;
		DWORD dwStringHeight = nHeight;
		//F3dZUIFreeType::GetSingleton()->GetSize( lpszFontName, pStr, &dwStringWidth, &dwStringHeight, nFontSize, nFlag );
		dwStringHeight = (int)(nFontSize*1.5f);
		
		//m_editTextSize.cx = dwStringWidth; 
		//m_editTextSize.cy = dwStringHeight;
	}

	if( pImg == NULL ) return;		

	DWORD dwTexWidth  = (DWORD)pImg->GetWidth();
	DWORD dwTexHeight = (DWORD)pImg->GetHeight();

	//W
	if( (dwAlign & ALIGN_LEFT) == 0 ) {
		rcTexRect.left   = 0; 
		rcTexRect.right  = dwTexWidth; 
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}
	if( dwAlign & ALIGN_RIGHT ) {
		rcTexRect.left   = dwWidth-dwTexWidth-5; 
		rcTexRect.right  = dwWidth-5; 
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}
	if( dwAlign & ALIGN_HCENTER ) {
		rcTexRect.left   = dwWidth/2-dwTexWidth/2; 
		rcTexRect.right  = dwWidth/2+dwTexWidth/2; 
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}

	//H
	if( dwAlign & ALIGN_VCENTER ) {
		rcTexRect.top    = dwHeight/2-dwTexHeight/2;
		rcTexRect.bottom = dwHeight/2+dwTexHeight/2;
	}
	if( dwAlign == ALIGN_TOP ) {
		rcTexRect.top    = 0;
		rcTexRect.bottom = dwTexHeight;
	}
	if( dwAlign & ALIGN_BOTTOM ) {
		rcTexRect.top    = dwHeight-dwTexHeight;
		rcTexRect.bottom = dwHeight;
	}

	//Copy
	int nStride;
	char* pImgBuf32 = NULL;
	F3dCTexture* pTexture = pInTexture.get();

	pTexture->LockRect(&rcRect, reinterpret_cast<void **>(&pImgBuf32),nStride);

	if( NULL == pImgBuf32 )
		return;

	const ARGBBuffer::Pixel *p = pImg->GetBuffer();
	int nSrcStride = dwTexWidth;

	DWORD* pDstColor;
	DWORD* pSrcColor;

	for( int y = 0; y < rcRect.bottom; y++ )
	{
		pDstColor = ( DWORD* )pImgBuf32;

		if( y >= rcTexRect.top && y < rcTexRect.bottom )
		{
			pSrcColor = ( DWORD* )p;

			for( int x = 0; x < rcRect.right; x++ )
			{
				if( x >= rcTexRect.left && x < rcTexRect.right )
					pDstColor[x] = pSrcColor[x-rcTexRect.left];
				else
					pDstColor[x] = c_dwBlankColor;
			}
			p         += nSrcStride;
		}
		else
		{
			for( int x = 0; x < rcRect.right; x++ )
			{
				pDstColor[x] = c_dwBlankColor;
			}
		}
		pImgBuf32 += nStride;
	}

	pTexture->Unlock();

#ifdef _DEBUG
	static bool bSaveFont = false;
	if( bSaveFont ) {
		pTexture->SaveTexture( L"c:\\font_test.tga" );
		bSaveFont = false;
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Parser

struct  BTOKEN
{
	int           val;
	const WCHAR * str;
} dict_tokens[] = {
	MCParser::COLOR       , L"COLOR",
	MCParser::COLOR_DEF   , L"/COLOR",
	MCParser::LINE_BREAK  , L"BR",
	MCParser::OFFSET_POS_X, L"POSX",
	MCParser::BOLD_ON     , L"B",
	MCParser::BOLD_OFF    , L"/B",
	MCParser::ITALIC_ON   , L"I",
	MCParser::ITALIC_OFF  , L"/I",
	MCParser::INVERSE_ON  , L"INV",
	MCParser::INVERSE_OFF , L"/INV",
	MCParser::STRIKE_ON   , L"STRIKE",
	MCParser::STRIKE_OFF  , L"/STRIKE",
	MCParser::SHADOW_ON   , L"SHADOW",
	MCParser::SHADOW_OFF  , L"/SHADOW",
	MCParser::OUTLINE_ON  , L"OUT",
	MCParser::OUTLINE_OFF , L"/OUT",
	MCParser::UNDER_ON    , L"UNDER",
	MCParser::UNDER_OFF   , L"/UNDER",
	MCParser::AL_RIGHT    , L"RIGHT",
	MCParser::FONT        , L"FONT",
	MCParser::FONT_DEF    , L"/FONT",

	MCParser::MAP_NPC_LNK_ON  , L"MAP_NPC_LNK",
	MCParser::MAP_NPC_LNK_OFF , L"/MAP_NPC_LNK",
};

static inline int hextoint( const WCHAR *str )
{ 
	 
	int result = 0;
	for ( int i=0 ; i< 2 ; i++ )
	{
		if ( str[i] >= L'0' && str[i] <= L'9' )
			result += (str[i] - L'0') * (i==0 ? 16 : 1);
		else if ( str[i] >= L'A' && str[i] <= L'F' )
			result += (str[i] - L'A' + 10 ) * (i==0 ? 16 : 1);
		else if ( str[i] >= L'a' && str[i] <= L'f' )
			result += (str[i] - L'a' + 10 ) * (i==0 ? 16 : 1);
		else return 0;
	}
	return result;
}

bool MCParser::s_bWordWrap = false;

void MCParser::checkToken( const WCHAR* pText, TEXT_TOKEN* pToken, const WCHAR* pOnlyText )
{ 
	 
	for ( int i=0 ; i < sizeof(dict_tokens) / sizeof(BTOKEN) ; i++ )
	{
		if( _wcsnicmp( dict_tokens[i].str, pText, wcslen(dict_tokens[i].str) ) == 0 ) 
		{
			switch( i+1 )
			{
			case COLOR        : 
				{
					const WCHAR* pStr = pText+6;
					int len = (int)wcslen(pStr);
					if( len < 6 ) {
						pToken->dwType = INVALID;
						return;
					}
					len = ((len/2)>4) ? 4 : len/2;
					unsigned char colors[4] = { 255, 255, 255, 255, };
					for ( int j=0 ; j<len ; j++ ) {
						colors[j] = (unsigned char)hextoint( &pStr[j*2] );
					}
					pToken->dwType = COLOR;
					// a,r,g,b
					pToken->dwColor = (colors[3] << 24) | (colors[0] << 16) | (colors[1] << 8) | colors[2];
				}
				return;
			case OFFSET_POS_X : 
				{
					pToken->dwType   = OFFSET_POS_X;
					pToken->dwOffset = _wtoi(pText + wcslen(dict_tokens[i].str)+1);
				}
				return;
			case FONT		  :
				pToken->strFont	= (pText + wcslen(dict_tokens[i].str)+1);
				pToken->dwType = FONT;
				return;
			case FONT_DEF		  :
				pToken->strFont	= L"font"; //Def Font
				pToken->dwType  = FONT_DEF;
				return;
			case MAP_NPC_LNK_ON   :
				pToken->strMapNpcLnk = (pText + wcslen(dict_tokens[i].str)+1);
				pToken->dwType  = MAP_NPC_LNK_ON;
				pToken->dwColor = 0xFF00A2E8;
				return;

			case MAP_NPC_LNK_OFF  :
			case COLOR_DEF    :
			case LINE_BREAK   : 
			case BOLD_ON      : 
			case BOLD_OFF     : 
			case ITALIC_ON    :
			case ITALIC_OFF   :
			case INVERSE_ON   : 
			case INVERSE_OFF  : 
			case STRIKE_ON    : 
			case STRIKE_OFF   : 
			case SHADOW_ON    : 
			case SHADOW_OFF   : 
			case OUTLINE_ON   : 
			case OUTLINE_OFF  : 
			case UNDER_ON     : 
			case UNDER_OFF    : 
			case AL_RIGHT     :
				pToken->dwType = (i+1);
				return;
			default:
				return;
			}//switch
		}//if
	}//for

	//예외 <ddd> 이런 것 처리
	pToken->dwType  = TEXT;
	pToken->strText = L'<';
	pToken->strText += pOnlyText;
	pToken->strText += L'>';
}

void MCParser::getNextToken( TEXT_TOKEN* pToken, const WCHAR* pSrc, int& nCount )
{ 
	 
	WCHAR a = getWCharToken( pSrc, nCount );
	if ( a == L'\0' ) {
		pToken->dwType = ENDOFSTRING;
		return;
	}
	if( a == L'<' ) {
		bool success  = false;
		int tempCount = nCount;
		std::wstring str, str_space;
		while( true ) {
			WCHAR c = getWCharToken( pSrc, tempCount );

			if( c == L' ' ) {
				str_space += c;
				continue;
			}
			else if( c == L'>' ) {
				success = true;
				break;
			}
			else if( c == L'<' || c == '\0' )
				break;
			str += c;
			str_space += c;
		}
		if( success == true ) {
			nCount = tempCount;
			checkToken( str.c_str(), pToken, str_space.c_str() );
			return;
		}
	}

	std::wstring str;
	while( true ) {
		str += a;
		a = getWCharToken( pSrc, nCount );
		if( a == L'<' ) {
			nCount--;
			break;
		}
		else if( a == L'\0' ) {
			nCount--;
			break;
		}
	}
	pToken->dwType  = TEXT;
	pToken->strText = str;
}

std::wstring SplitWidth( const WCHAR* pFontName, const int nFontSize, const BOOL bBold, const DWORD dwEmptyWidth, std::wstring& strText, bool& bWordWrap )
{ 
	 
	DWORD dwWidth = 0, dwHeight = 0;

	std::wstring s;
	WCHAR c;
	INT uLastSpace = -1;
	for( UINT i=0; strText.size()>i; ++i ) {
		c = strText.at(i);
		s += c;		

		if( c == L' ' ) { //WordWrap
			uLastSpace = i;
		}

		F3dZUIText::GetStringSize( pFontName, nFontSize, bBold, s.c_str(), (int)s.size(), dwWidth, dwHeight );

		if( dwWidth > dwEmptyWidth ) { 

			if( MCParser::IsWordWrap() )
			{
				if( uLastSpace > 0 ) {
					i = uLastSpace;
					bWordWrap = true;
				}
			}

			s = strText.substr( 0, i );
			strText.erase( 0, i );
			return s;
		}
	}
	strText.erase( strText.begin(), strText.end() );
	return s;
}

//Color 가 들어간 일부 String의 문자 크기가 필요할 경우, Tag를 제거하는 용도. 멀티라인은 지원하지 않음.
void MCParser::RemoveTag( const WCHAR* pSrc, std::wstring& strText )
{
	 

	BOOL bLoop  = TRUE;
	int  nCount = 0;
	while( bLoop ) {

		TEXT_TOKEN token;
		getNextToken(&token, pSrc, nCount);

		switch(token.dwType)
		{			
		case COLOR        : 
		case COLOR_DEF    :
		case LINE_BREAK   : 
		case OFFSET_POS_X : 
		case BOLD_ON      :
		case BOLD_OFF     :
		case ITALIC_ON    :
		case ITALIC_OFF   :
		case INVERSE_ON   :
		case INVERSE_OFF  :
		case STRIKE_ON    :
		case STRIKE_OFF   :
		case SHADOW_ON    :
		case SHADOW_OFF   :
		case OUTLINE_ON   :
		case OUTLINE_OFF  :
		case UNDER_ON     :
		case UNDER_OFF    :
		case AL_RIGHT     : 
		case FONT         :
		case FONT_DEF     : 
		case MAP_NPC_LNK_ON  :
		case MAP_NPC_LNK_OFF :
			break;

		case TEXT         :
			{
				if( !token.strText.empty() ) {
					strText += token.strText;
					token.Init();
				}
			}
			break;
		case ENDOFSTRING  : 
			{
				bLoop = FALSE;
			}
			break;
		case INVALID      : break;
		default:
			break;
		}
	}
}

void MCParser::DoParse( const WCHAR* pSrc, const WCHAR* pFontName, const int nFontSize, const DWORD dwMaxWidth, std::vector<TEXT_DATA>& vTextDataList )
{ 
	 
	const DWORD c_dwDefColor = 0xFFFFFFFF;

	F3dColor	 color     = c_dwDefColor;
	int			 line      = 0;
	DWORD        curWidth  = 0;
	DWORD		 dwFlag    = 0;
	DWORD		 dwOffSetX = 0;
	BOOL         bRight    = FALSE;

	std::wstring strCurFontName = pFontName;

	BOOL bLoop  = TRUE;
	int  nCount = 0;
	while( bLoop ) {

		TEXT_TOKEN token;
		getNextToken(&token, pSrc, nCount);

		switch(token.dwType)
		{			
		case COLOR        : 
			color = token.dwColor;
			break;
		case COLOR_DEF    :
			color = c_dwDefColor;
			break;
		case LINE_BREAK   : 
			curWidth = 0;
			++line;
			break;
		case OFFSET_POS_X : 
			dwOffSetX = token.dwOffset;
			break;
		case BOLD_ON      : { dwFlag |=  F3dZUIText::FLAG_BOLD;		}	break;
		case BOLD_OFF     : { dwFlag &= ~F3dZUIText::FLAG_BOLD;		}	break;
		case ITALIC_ON    : { dwFlag |=  F3dZUIText::FLAG_ITALIC;	}	break;
		case ITALIC_OFF   : { dwFlag &= ~F3dZUIText::FLAG_ITALIC;	}	break;
		case INVERSE_ON   : { dwFlag |=  F3dZUIText::FLAG_INVERSE;	}	break;
		case INVERSE_OFF  : { dwFlag &= ~F3dZUIText::FLAG_INVERSE;	}	break;
		case STRIKE_ON    : { dwFlag |=  F3dZUIText::FLAG_STRIKE;	}	break;
		case STRIKE_OFF   : { dwFlag &= ~F3dZUIText::FLAG_STRIKE;	}	break;
		case SHADOW_ON    : { dwFlag |=  F3dZUIText::FLAG_SHADOW;	}	break;
		case SHADOW_OFF   : { dwFlag &= ~F3dZUIText::FLAG_SHADOW;	}	break;
		case OUTLINE_ON   : { dwFlag |=  F3dZUIText::FLAG_OUTLINE;	}	break;
		case OUTLINE_OFF  : { dwFlag &= ~F3dZUIText::FLAG_OUTLINE;	}	break;
		case UNDER_ON     : { dwFlag |=  F3dZUIText::FLAG_UNDER;	}	break;
		case UNDER_OFF    : { dwFlag &= ~F3dZUIText::FLAG_UNDER;	}	break;
		case AL_RIGHT     : 
			bRight    = TRUE;
			break;

		case FONT        : 
			strCurFontName = token.strFont.c_str();
			break;

		case FONT_DEF    : 
			strCurFontName = L"font";
			break;

		case MAP_NPC_LNK_ON  :
			color = token.dwColor;
			dwFlag |=  F3dZUIText::FLAG_UNDER;
			break;

		case MAP_NPC_LNK_OFF   :
			color = c_dwDefColor;
			dwFlag &= ~F3dZUIText::FLAG_UNDER;
			break;

		case TEXT         :
			{
				if( !token.strText.empty() ) {

					//Text가 현재 줄에 들어 갈 수 있나?
					BOOL bBold = dwFlag & F3dZUIText::FLAG_BOLD;
					DWORD dwStrWidth = 0, dwStrHeight = 0;
					F3dZUIText::GetStringSize( strCurFontName.c_str(), nFontSize, bBold, token.strText.c_str(), (int)token.strText.size(), dwStrWidth, dwStrHeight );

					curWidth += dwStrWidth;
					curWidth += dwOffSetX;

					//줄 넘어가면, 잘라야 됨.
					if( curWidth > dwMaxWidth ) {

						curWidth -= dwStrWidth;
						int nEmptyWidth = dwMaxWidth - curWidth;

						std::wstring s;
						while( true ) {

							bool bWordWrap = false;
							s = SplitWidth( strCurFontName.c_str(), nFontSize, bBold, nEmptyWidth, token.strText, bWordWrap );
							if( s.empty() ) {
								if( token.strText.empty() )
									break;

								//New Line
								++line;
								curWidth = 0;
								nEmptyWidth = dwMaxWidth;
								continue;
							}

							F3dZUIText::GetStringSize( strCurFontName.c_str(), nFontSize, bBold, s.c_str(), (int)s.size(), dwStrWidth, dwStrHeight );
							curWidth += dwStrWidth;

							TEXT_DATA data;
							data.color      = color;
							data.dwOffSetX  = dwOffSetX;
							data.dwFlag     = dwFlag;
							data.text       = s;
							data.nLine      = line;
							data.font       = strCurFontName.c_str();
							data.dwStrWidth = dwStrWidth;

							vTextDataList.push_back( data );
							
							if( curWidth <= dwMaxWidth ) {
								nEmptyWidth -= dwStrWidth;
							}
							
							if( dwOffSetX != 0 ) 
								dwOffSetX = 0; //기존 OffSet Reset

							if( bWordWrap ) 
							{
								++line;
								curWidth = 0;
								nEmptyWidth = dwMaxWidth;
							}
						}

					}//if
					else {
						if( bRight ) {
							dwOffSetX = dwMaxWidth-(curWidth/*+c_nMarginX*/);
							curWidth +=dwOffSetX;
							bRight=FALSE;
						}

						TEXT_DATA data;
						data.color      = color;
						data.dwOffSetX  = dwOffSetX;
						data.dwFlag     = dwFlag;
						data.text       = token.strText;
						data.font       = strCurFontName.c_str();
						data.nLine      = line;
						data.dwStrWidth = dwStrWidth;

						vTextDataList.push_back( data );
						dwOffSetX = 0;
					}

					token.Init();
				}
			}
			break;
		case ENDOFSTRING  : 
			{
				bLoop = FALSE;
			}
			break;
		case INVALID      : break;
		default:
			break;
		}
	}
}

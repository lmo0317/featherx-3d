#include "FeatherXEnginePrecompile.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include "MBfasttime.h"

namespace
{
	const float c_fRatioHeight  = 0.9f;
	const float c_fRatioYMargin = 0.25f;
	const int   c_nMinYMargin   = 3;
};

class F3dZUIFreeType::MFreeType
{
public:
	MFreeType()
	{
		m_library = NULL;

		m_nRed   = 255;
		m_nGreen = 255;
		m_nBlue  = 255;

		m_nRedBG   = 0;
		m_nGreenBG = 0;
		m_nBlueBG  = 0;
	};

	bool IsInitialized() const { return m_library != NULL; }

	bool Init()
	{
		if( FT_Init_FreeType(&m_library) ) 
			return false;

		return true;
	}

	bool DeInit()
	{
		if( !m_library ) 
			return false;
		
		for( std::vector< ARGBBuffer* >::iterator it =  m_vImageList.begin(); it != m_vImageList.end(); ++it )
		{
			delete (*it);
		}

		for(std::vector< FaceInfo* >::iterator it = m_vFaceList.begin(); it != m_vFaceList.end(); ++it )
		{
			FT_Done_Face( (*it)->face );
			delete (*it);
		}

		if( FT_Done_FreeType(m_library) )
			return false;

		m_library = NULL;

		return true;
	}

	bool selectCharmap( FT_Face face, FT_Encoding ft_encoding )
	{
		for( int i = 0; i < face->num_charmaps; ++i )
		{
			if( face->charmaps[i]->encoding == ft_encoding )
			{
				FT_Error ft_error = FT_Select_Charmap( face, face->charmaps[i]->encoding );

				if( ft_error ) {
					return false;
				}
				return true;
			}
		}
		return false;
	}

	bool LoadFontBuffer( const WCHAR* pAlias, const void* pBuf, size_t nBufSize )
	{	
		FT_Face face;
		if( FT_New_Memory_Face( m_library, (FT_Byte*)pBuf, (FT_Long)nBufSize, 0, &face ) ) {
			return false;
		}

		//cache
		FaceInfo *pInfo = new FaceInfo;
		pInfo->bUniCode = selectCharmap( face, FT_ENCODING_UNICODE );
		pInfo->face     = face;
		pInfo->strAlias = pAlias;
		m_vFaceList.push_back( pInfo );

		return true;
	}

	bool LoadFont( const WCHAR* pAlias, const char* pFullPath )
	{
		FT_Face face;
		if( FT_New_Face( m_library, pFullPath, 0, &face ) ) {
			return false;
		}

		//cache
		FaceInfo *pInfo = new FaceInfo;
		pInfo->bUniCode = selectCharmap( face, FT_ENCODING_UNICODE );
		pInfo->face     = face;
		pInfo->strAlias = pAlias;
		m_vFaceList.push_back( pInfo );

		return true;
	}

	int get_x_margin( int nFontSize, int ft )
	{
		int nXAdd = 0;
		return nXAdd;
	}

	int get_y_margin( int nFontSize, int ft )
	{
		int nYAdd = (int)(nFontSize*c_fRatioYMargin);
		if( nYAdd < c_nMinYMargin ) nYAdd = c_nMinYMargin;
		return nYAdd;
	}

	void SetColor( int r, int g, int b   ) { m_nRed   = r; m_nGreen   = g; m_nBlue   = b; }
	void SetBGColor( int r, int g, int b ) { m_nRedBG = r; m_nGreenBG = g; m_nBlueBG = b; }

	const ARGBBuffer * Draw( const WCHAR *szAlias, const WCHAR* wszString, size_t len, DWORD* pWidth, DWORD* pHeight, int nFontSize = 11, int ft = FT_PLAIN, BOOL bUnderline = FALSE, BOOL bBackGround = FALSE )
	{
		FaceInfo* pInfo = getFace( szAlias );
		if( !pInfo ) 
			return NULL;

		if( FT_Set_Char_Size( pInfo->face, 0, nFontSize*96, 64, 64 ) )
			return NULL;

		std::vector< FT_UInt > vGlyphIndex;

		getGlyphList( pInfo, wszString, len, &vGlyphIndex );

		getTextInfo( pInfo, nFontSize, ft, vGlyphIndex, pWidth, pHeight );

		ARGBBuffer *pImage = getSurface( *pWidth, *pHeight );
		if( !pImage )
			return NULL;

		drawGlyphList( pImage, pInfo, nFontSize, ft, vGlyphIndex );

		applyEffect( pImage, nFontSize, ft, bUnderline, bBackGround );

		return pImage;
	}

	const BOOL GetSize( const WCHAR *szAlias, const WCHAR* wszString, size_t len, DWORD* pWidth, DWORD* pHeight, int nFontSize, int ft )
	{
		FaceInfo* pInfo = getFace( szAlias );
		if( !pInfo ) return FALSE;

		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			if( FT_Set_Char_Size( pInfo->face, 0, 96*nFontSize, 64, 64 ) ) return FALSE;
			//if( FT_Select_Charmap( pTag->face, pTag->face->charmaps[0]->encoding ) ) return NULL;
		}

		std::vector< FT_UInt >	vGlyphIndex;

		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			// glyph index 리스트를 얻는다
			getGlyphList( pInfo, wszString, len, &vGlyphIndex );
		}

		{
//			_TIME_CHECK_ timeCheck( 1, __FILE__, __LINE__ );
			// 크기정보를 얻음
			getTextInfo( pInfo, nFontSize, ft, vGlyphIndex, pWidth, pHeight );
		}

		return TRUE;
	}

	FT_Library* GetLib() { return &m_library; }

private:

	struct	FaceInfo
	{
		FT_Face		 face;
		bool		 bUniCode;
		std::wstring strAlias;
	};
	
	int get_pen_start_x( int nFontSize, int ft ) { return get_x_margin( nFontSize, ft ); }
	int get_pen_start_y( int nFontSize, int ft ) { return (int)(nFontSize*c_fRatioHeight ) + get_y_margin( nFontSize, ft ); }

	int get_font_height( int nFontSize, int ft )
	{	
		return (int)(nFontSize*c_fRatioHeight + get_y_margin( nFontSize, ft )*2);
	}

	void applyEffect( ARGBBuffer *pImage, int nFontSize, int ft, BOOL bUnderline, BOOL bBackground = false )
	{
		ARGBBuffer *pBuffer     = NULL;
		ARGBBuffer *pBackGround = NULL;

		if( bBackground ) {
			pBackGround = getSurface( pImage->GetWidth(), pImage->GetHeight(), pImage );
			pBackGround->ForceFillColor( m_nRedBG, m_nGreenBG, m_nBlueBG, 150 );
		}

		pImage->FillColor( m_nRed, m_nGreen, m_nBlue );

		if( bUnderline ) {
			int nXMargin = get_x_margin( nFontSize, ft );
			int nYMargin = get_y_margin( nFontSize, ft );

			for( UINT i = nXMargin; i+nXMargin < pImage->GetWidth(); ++i )
			{
				pImage->SetColor( i, pImage->GetHeight() - nYMargin , m_nRed, m_nGreen, m_nBlue );
				pImage->SetAlpha( i, pImage->GetHeight() - nYMargin , ARGBBuffer::MAX_VALUE );				
			}
		}

		if( ft & FT_GLOW || ft & FT_SHADOW || ft & FT_OUTLINE ) {
			pBuffer = getSurface( pImage->GetWidth(), pImage->GetHeight(), pImage, pBackGround );
			*pBuffer = *pImage;
		}

		if( ft & FT_BOLD    ) {
			pImage->ModulateAlpha( 1.8f );
		}
		if( pBuffer ) {
			if( ft & FT_GLOW    ) {
				pBuffer->IncAlpha( 10 );
				pBuffer->IncAlpha( 20 );
				pBuffer->FillColor( m_nRedBG, m_nGreenBG, m_nBlueBG );
				pImage->Append( *pBuffer, 0, 0, ARGBBuffer::OP_GLOW );
			}
			if( ft & FT_OUTLINE ) {
				pBuffer->IncAlpha( 10 );
				pBuffer->IncAlpha( 20 );
				pBuffer->FillColor( m_nRedBG, m_nGreenBG, m_nBlueBG );
				pImage->Append( *pBuffer, 0, 0, ARGBBuffer::OP_GLOW );
			}
			if( ft & FT_SHADOW	) {
				pBuffer->FillColor( m_nRedBG, m_nGreenBG, m_nBlueBG );
				pImage->Append( *pBuffer, -1, -1, ARGBBuffer::OP_SHADOW );
			}
		}

		if( bBackground ) {
			pImage->Append( *pBackGround, 0, 0, ARGBBuffer::OP_GLOW );
		}
	}

	void drawGlyphList( ARGBBuffer *pImage, FaceInfo* pInfo, int nFontSize, int ft, const std::vector< FT_UInt > & vGlyphIndex )
	{
		FT_GlyphSlot			slot = pInfo->face->glyph;  /* a small shortcut */

		int pen_x = get_pen_start_x( nFontSize, ft );
		int pen_y = get_pen_start_y( nFontSize, ft );

		for ( size_t n = 0; n < vGlyphIndex.size(); n++ )
		{	
			///* load glyph image into the slot (erase previous one) */ 
			if( FT_Load_Glyph( pInfo->face, vGlyphIndex[n], FT_LOAD_DEFAULT ) ) continue;

			/* convert to an anti-aliased bitmap */
			if( FT_Render_Glyph( pInfo->face->glyph, ft_render_mode_normal ) ) continue;

			/* now, draw to our target surface */
			draw_bitmap( pImage, &slot->bitmap,
				pen_x + slot->bitmap_left,
				pen_y - slot->bitmap_top );

			/* increment pen position */
			pen_x += slot->advance.x >> 6;
			pen_y += slot->advance.y >> 6; /* not useful for now */
		}
	}

	void draw_bitmap( ARGBBuffer *pImg, FT_Bitmap*  bitmap, FT_Int x, FT_Int y )
	{
		FT_Int  i, j, p, q;
		FT_Int  x_max = x + bitmap->width;
		FT_Int  y_max = y + bitmap->rows;

		FT_Int	image_width  = (FT_Int)pImg->GetWidth();
		FT_Int	iamge_height = (FT_Int)pImg->GetHeight();

		for ( i = x, p = 0; i < x_max; i++, p++ )
		{
			for ( j = y, q = 0; j < y_max; j++, q++ )
			{
				if ( i >= image_width || j >= iamge_height ) continue;

				if( i < 0 || j < 0 || bitmap->buffer[q * bitmap->width + p] == 0 ) continue;

				pImg->SetAlpha( i, j, bitmap->buffer[q * bitmap->width + p] );
			}
		}
	}

	ARGBBuffer* getSurface( size_t nWidth, size_t nHeight, ARGBBuffer *pExcept1 = NULL, ARGBBuffer *pExcept2 = NULL)
	{
		for( std::vector< ARGBBuffer* >::iterator it =  m_vImageList.begin(); it != m_vImageList.end(); ++it )
		{
			if( (*it)->GetWidth() < nWidth || (*it)->GetHeight() < nHeight || (*it) == pExcept1 || (*it) == pExcept2 ) continue;
			(*it)->Clear();
			(*it)->SetSize( nWidth, nHeight );
			return (*it);
		}

		ARGBBuffer *pImg = NULL;

		pImg = new ARGBBuffer( nWidth, nHeight );

		m_vImageList.push_back( pImg );
		return pImg;
	}

	FaceInfo* getFace( const WCHAR *szAlias )
	{
		for(std::vector< FaceInfo* >::iterator it = m_vFaceList.begin(); it != m_vFaceList.end(); ++it )
		{
			if( _wcsicmp( szAlias, (*it)->strAlias.c_str() ) ) continue;
			return (*it);
		}

		if( m_vFaceList.empty() ) return NULL;

		return m_vFaceList.front();
	}

	void getGlyphList( FaceInfo* pInfo, const WCHAR* wszString, size_t len, std::vector< FT_UInt > *pList )
	{
		// 폰트의 코드페이지를 얻어낸다
		int nCodePage = 0;
		if( pInfo->bUniCode == false ) {
			for( int i = 0; i < pInfo->face->num_charmaps; ++i ) {
				if( pInfo->face->charmaps[i]->encoding == FT_ENCODING_SJIS	  )	{ nCodePage =  932; FT_Select_Charmap( pInfo->face, pInfo->face->charmaps[i]->encoding ); break; }
				if( pInfo->face->charmaps[i]->encoding == FT_ENCODING_GB2312  )	{ nCodePage =  936; FT_Select_Charmap( pInfo->face, pInfo->face->charmaps[i]->encoding ); break; }
				if( pInfo->face->charmaps[i]->encoding == FT_ENCODING_BIG5	  )	{ nCodePage =  950; FT_Select_Charmap( pInfo->face, pInfo->face->charmaps[i]->encoding ); break; }
				if( pInfo->face->charmaps[i]->encoding == FT_ENCODING_WANSUNG )	{ nCodePage =  949; FT_Select_Charmap( pInfo->face, pInfo->face->charmaps[i]->encoding ); break; }
//				{ nCodePage =  1256; FT_Select_Charmap( pInfo->face, pInfo->face->charmaps[i]->encoding ); break; } //아랍어
				
				if( pInfo->face->charmaps[i]->encoding == FT_ENCODING_JOHAB	  )	{ nCodePage = 1361; FT_Select_Charmap( pInfo->face, pInfo->face->charmaps[i]->encoding ); break; }
			}
		}

		for ( size_t n = 0; n < len; n++ ) {
			int code_point = wszString[n];

			// UTF16-BE 로 변환
			if( nCodePage )
			{
				if( ( code_point >= 0xAC00 && code_point <= 0xD7A3 ) || ( code_point >= 0x3130 && code_point <= 0x318F ) )
				{
					int nResult = WideCharToMultiByte( nCodePage, 0, &wszString[n], 1, (LPSTR)&code_point, sizeof(code_point), "*", NULL ); 
					std::swap( *((char*)( &code_point ) + 0), *((char*)( &code_point ) + 1) );
				}
			}

			/* retrieve glyph index from character code */
			pList->push_back( FT_Get_Char_Index( pInfo->face, code_point ) );
		}
	}

	void getTextInfo( FaceInfo* pTag, int nFontSize, int ft, const std::vector< FT_UInt >& vGlyphList, DWORD* pWidth, DWORD* pHeight )
	{
		int nXMargin = get_x_margin( nFontSize, ft );
		int nYMargin = get_y_margin( nFontSize, ft );

		int nPenStartX = get_pen_start_x( nFontSize, nXMargin );
		int nCurrentX  = nPenStartX*2;

		FT_GlyphSlot  slot = pTag->face->glyph;

		for( size_t i = 0; i < vGlyphList.size(); ++i )
		{
			/*FT_LOAD_NO_HINTING*/
			if( FT_Load_Glyph( pTag->face, vGlyphList[i], FT_LOAD_DEFAULT ) ) continue;

			/* increment pen position */
			nCurrentX += slot->advance.x >> 6;
		}

		int nWidth  = nCurrentX;
		int nHeight = get_font_height( nFontSize, ft );

		nWidth++;

		if( nWidth % 2  ) nWidth++;
		if( nHeight % 2 ) nHeight++;

		if( pWidth  ) *pWidth  = nWidth;
		if( pHeight ) *pHeight = nHeight;
	}

	FT_Library	m_library;	
	std::vector<FaceInfo*>		m_vFaceList;
	std::vector<ARGBBuffer*>	m_vImageList;

	int		m_nRed;
	int		m_nGreen;
	int		m_nBlue;

	int		m_nRedBG;
	int		m_nGreenBG;
	int		m_nBlueBG;
};

F3dZUIFreeType::F3dZUIFreeType()
{  
	m_pImplement = new MFreeType();
}

F3dZUIFreeType::~F3dZUIFreeType()
{  
	delete m_pImplement;
}

void* F3dZUIFreeType::GetFreeTypeLib()
{
	return m_pImplement->GetLib();
}

bool F3dZUIFreeType::IsInitialized() const
{
	return m_pImplement->IsInitialized();
}

bool F3dZUIFreeType::Init()
{  
	return m_pImplement->Init();
}

bool F3dZUIFreeType::DeInit()
{  
	return m_pImplement->DeInit();
}

bool F3dZUIFreeType::LoadFontBuffer( const WCHAR *szAlias, const void *szBuffer, size_t nBufferSize )
{  
	return m_pImplement->LoadFontBuffer( szAlias, szBuffer, nBufferSize );
}

bool F3dZUIFreeType::LoadFont( const WCHAR *szAlias, const char *szFilePath )
{  
	return m_pImplement->LoadFont( szAlias, szFilePath );
}


int F3dZUIFreeType::get_x_margin( int nFontSize, int ft )
{  
	return m_pImplement->get_x_margin( nFontSize, ft );
}

int F3dZUIFreeType::get_y_margin( int nFontSize, int ft )
{  
	return m_pImplement->get_y_margin( nFontSize, ft );
}

const ARGBBuffer* F3dZUIFreeType::Draw( const WCHAR *szAlias, const WCHAR* wszString, size_t len, DWORD* pWidth, DWORD* pHeight, int nFontSize, int ft, BOOL bUnderline, BOOL bBackGround )
{  
	return m_pImplement->Draw( szAlias, wszString, len, pWidth, pHeight, nFontSize, ft, bUnderline, bBackGround );
}

const BOOL F3dZUIFreeType::GetSize( const WCHAR *szAlias, const WCHAR* wszString, DWORD* pWidth, DWORD* pHeight, int nFontSize, int ft )
{  
	size_t len = wcslen( wszString );
	if( !len ) {
		*pWidth  = 0;
		*pHeight = 0;
		return TRUE;
	}

	return m_pImplement->GetSize( szAlias, wszString, len, pWidth, pHeight, nFontSize, ft );
}

void F3dZUIFreeType::SetColor( DWORD dwColor )
{  
	int r =	((dwColor&0x00ff0000) >> 16);
	int g = ((dwColor&0x0000ff00) >>  8);
	int b =  (dwColor&0x000000ff);

	m_pImplement->SetColor( r, g, b );
}

void F3dZUIFreeType::SetBGColor( DWORD dwColor )
{  
	int r =	((dwColor&0x00ff0000) >> 16);
	int g = ((dwColor&0x0000ff00) >>  8);
	int b =  (dwColor&0x000000ff);

	m_pImplement->SetBGColor( r, g, b );
}
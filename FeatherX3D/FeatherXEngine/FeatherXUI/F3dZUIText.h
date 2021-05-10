

#pragma once

class F3dZControl;

class F3dZUIText
{
public:
	F3dZUIText( const WCHAR* pStr, int nWidth, int nHeight, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color, bool bBillboard,bool b3DRender, bool b3DCursor,bool bTextRoll=false );
	~F3dZUIText();

	virtual void	Render( double dTime );
	void	SetPos( const int nPosX, const int nPosY );
	void	SetCenter( const F3dVector& vtxCenter,const int nWidth,const int nHeight );
	void	SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight);

	void	SetSize( const int nWidth, const int nHeight );

	void	SetBillBoard( bool bFlag );
	void	Set3DRender(bool bFlag);	
	void	Set3DCursor(bool bFlag);


	static void InitTextRender();
	static void DeInitTextRender();
	static void	GetStringSize( LPCWSTR lpszFontName, int nFontSize, BOOL bBold, LPCWSTR lpszString, int nStringLen, DWORD& dwStringWidth, DWORD& dwStringHeight );
	static void drawFreeTypeToTexture( const WCHAR* pStr, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color, RECT& rcRect, RECT& rcTexRect,F3dCTexturePtr pInTexture );
	const int GetSizeX() { return m_editTextSize.cx; }
	const int GetSizeY() { return m_editTextSize.cy; }

	const int GetPosX() { return m_nPosX; }
	const int GetPosY() { return m_nPosY; }

	//Texture 재활용, Size 변경이 있으면 안됨.
	void SetText( const WCHAR* pStr, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color );

	void SetAlpha( float fRatio );
	void SetRenderTopLevel( bool bFlag );

	void Process( double dTime, double dMeanDT );

	void SetURoll( bool bFlag ) { m_bURoll = bFlag; }

	void SetParent( F3dZControl* pParent ) { m_pParent = pParent; }
	void onRefreshText();

	void	SetClipRect( int nLeft, int nTop, int nRight, int nBottom );

	enum
	{
		FLAG_UNDER   = 1<<0,
		FLAG_STRIKE  = 1<<1,
		FLAG_INVERSE = 1<<2,
		FLAG_ITALIC  = 1<<3,
		FLAG_BOLD    = 1<<4,
		FLAG_SHADOW  = 1<<5,
		FLAG_OUTLINE = 1<<6,
		FLAG_GLOW    = 1<<7,
	} TEXT_FLAG;

	enum
	{
		ALIGN_LEFT    = DT_LEFT,
		ALIGN_RIGHT   = DT_RIGHT,
		ALIGN_HCENTER = DT_CENTER,
		ALIGN_VCENTER = DT_VCENTER,
		ALIGN_TOP     = DT_TOP,
		ALIGN_BOTTOM  = DT_BOTTOM,
	} TEXT_ALIGN;

	struct FONT_INFO
	{
		FONT_INFO() { bIsFreeType = false; }
		virtual ~FONT_INFO() {}

		bool bIsFreeType;
	};

	static FONT_INFO* AddFreeTypeFont( const WCHAR* pAlias, const WCHAR* pFontName );

protected:
	void SetSprite( RECT& rcRect, RECT& rcTexRect );
	void drawFreeType( const WCHAR* pStr, DWORD dwFlag, const WCHAR* lpszFontName, int nFontSize, DWORD dwAlign, const F3dColor& color, RECT& rcRect, RECT& rcTexRect );
	void drawBitmapType( FONT_INFO * pBaseInfo, const WCHAR* pStr, const WCHAR* lpszFontName, int nFontSize, DWORD dwFlag, DWORD dwAlign, const F3dColor& color, RECT& rcRect, RECT& rcTexRect );

	void onRefreshText( const RECT& rcTexRect );

	struct FREETYPE_FONT_INFO : public FONT_INFO
	{
		FREETYPE_FONT_INFO(const WCHAR* pAlias, const WCHAR* pFontName );

		virtual ~FREETYPE_FONT_INFO()
		{

		}
	};

	struct WIN32_FONT_INFO : public FONT_INFO
	{
		WIN32_FONT_INFO() : hDefaultFont(NULL), hItalicFont(NULL), hBoldFont(NULL), hUnderLineFont(NULL), hStrikeOutFont(NULL)
		{
			bIsFreeType = false;
		}

		virtual ~WIN32_FONT_INFO()
		{
			if( hDefaultFont   ) DeleteObject(hDefaultFont  );
			if( hItalicFont    ) DeleteObject(hItalicFont   );
			if( hBoldFont      ) DeleteObject(hBoldFont     );
			if( hUnderLineFont ) DeleteObject(hUnderLineFont);
			if( hStrikeOutFont ) DeleteObject(hStrikeOutFont);
		}
		HFONT 		hDefaultFont;
		HFONT		hItalicFont;
		HFONT		hBoldFont;
		HFONT		hUnderLineFont;
		HFONT		hStrikeOutFont;

		static WIN32_FONT_INFO * CreateWin32Font() 
		{
			return new WIN32_FONT_INFO;
		}
	};

	static FONT_INFO* getFont( const WCHAR* pFontName, int nFontSize );
	static FONT_INFO* addWin32Font( const WCHAR* pFontName, int nFontSize );

	static HDC		  s_hDC;
	
	static boost::unordered_map< std::wstring, F3dZUIText::FONT_INFO* >* s_FontHash;
	static float		s_fFreeTypeSizeRatio;

	//
	F3dCUISpritePtr		m_Sprite;
	int					m_nPosX;
	int					m_nPosY;

	SIZE				m_editTextSize;

	bool				m_bURoll;
	int					m_nRollDegree;
	double				m_dRollTime;

	DWORD				m_dwWidth; 
	DWORD				m_dwHeight;   //크기

	DWORD				m_dwTexWidth; 
	DWORD				m_dwTexHeight;   //크기

	F3dZControl*		m_pParent;
	RECT				m_rcTexRect;
	bool				m_bBillboard;
	bool				m_b3DRender;
	bool				m_b3DCursor;

private:

};



typedef boost::shared_ptr< F3dZUIText > F3dZUITextPtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
class MCParser
{
	//	<COLOR:RRGGBB[AA]>
	//	</COLOR>
	//	<BR>
	//	<POSX>
	//	<B></B>
	//	<I></I>
	//	<INV></INV>
	//	<STRIKE></STRIKE>
	//	<SHADOW></SHADOW>
	//	<OUT></OUT>
	//	<UNDER></UNDER>
	//	<RIGHT>

	static bool s_bWordWrap;

public:

	static void SetWordWrap( bool bFlag ) { s_bWordWrap = bFlag; }
	static bool IsWordWrap() { return s_bWordWrap; }

	enum 
	{
		COLOR      = 1,
		COLOR_DEF   ,
		LINE_BREAK  ,
		OFFSET_POS_X,
		BOLD_ON     ,
		BOLD_OFF    ,
		ITALIC_ON   ,
		ITALIC_OFF  ,
		INVERSE_ON  ,
		INVERSE_OFF ,
		STRIKE_ON   ,
		STRIKE_OFF  ,
		SHADOW_ON   ,
		SHADOW_OFF  ,
		OUTLINE_ON  ,
		OUTLINE_OFF ,
		UNDER_ON    ,
		UNDER_OFF   ,
		AL_RIGHT    ,
		FONT        ,
		FONT_DEF    ,

		MAP_NPC_LNK_ON  ,
		MAP_NPC_LNK_OFF ,

		TEXT        ,
		ENDOFSTRING ,
		INVALID     ,
	};

	MCParser() {}
	~MCParser() {}

	struct TEXT_DATA
	{
		F3dColor	 color;
		DWORD		 dwFlag;
		DWORD		 dwOffSetX;
		int			 nLine;
		DWORD		 dwStrWidth;
		std::wstring text;
		std::wstring font;
	};

	struct TEXT_TOKEN
	{
		DWORD  		 dwType;
		DWORD  		 dwColor;
		DWORD  		 dwOffset;
		std::wstring strMapNpcLnk;
		std::wstring strFont;
		std::wstring strText;

		TEXT_TOKEN() {
			Init();
		}

		void Init()	{
			dwColor  = 0xFFFFFFFF;
			dwOffset = 0;
			dwType   = 0;
			//strText.erase( strText.begin(), strText.end() );
			strFont  = L"font";
		}
	};

	static void DoParse( const WCHAR* pSrc, const WCHAR* pFontName, const int nFontSize, const DWORD dwMaxWidth, std::vector<TEXT_DATA>& vTextDataList );
	static void RemoveTag( const WCHAR* pSrc, std::wstring& strText );

	static void getNextToken( TEXT_TOKEN* pToken, const WCHAR* pSrc, int& nCount );
protected:

	static inline WCHAR getWCharToken( const WCHAR *str, int &count )
	{
		WCHAR a;
		while( true )
		{
			a = str[count++];
			if ( a == L'\n' || a == L'\r' || a == L'\t' )
				continue;
			break;
		}
		return a;
	}

	static void checkToken( const WCHAR* pText, TEXT_TOKEN* pToken, const WCHAR* pOnlyText );

private:
};


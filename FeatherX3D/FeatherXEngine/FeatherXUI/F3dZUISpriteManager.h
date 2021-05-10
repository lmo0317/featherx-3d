

#pragma once

//랜더 단위
struct F3dCUISprite
{
	enum
	{
		TRIANGLE_STRIP = 0,
		TRIANGLE_FAN,
	};

	F3dCUISprite()
		: lu(0.f), tv(0.f), ru(1.f), bv(1.f), color_lu(0xFFFFFFFF), color_tv(0xFFFFFFFF), color_ru(0xFFFFFFFF), color_bv(0xFFFFFFFF), bShow(true), bText(false), TextureID(0), bIsTop(false),
		back_lu(0.f), back_tv(0.f), back_ru(1.f), back_bv(1.f), nDrawType(TRIANGLE_STRIP), nDegree(0), bReverse(false), bUseClip(false), m_fRadian(0.f), bSkipRelease(false), bBillboard(false), vtxCenter(0,0,0),b3DRender(false),b3DCursor(false)
	{
		ori_rect.left  = ori_rect.top    = 0;
		ori_rect.right = ori_rect.bottom = 0;
		rtScreen.left  = rtScreen.top    = 0;
		rtScreen.right = rtScreen.bottom = 0;
		rtClip.left   = 0;
		rtClip.top    = 0;
		rtClip.right  = 0;
		rtClip.bottom = 0;
	}
	~F3dCUISprite();

	void SetSkipRelease( bool bFlag ) {
		bSkipRelease = bFlag;
	}

	void SetAlpha( int a ) {
		F3dColor color = (a<<24)+(0x00FFFFFF & color_lu );
		color_lu = color_tv = color_ru = color_bv = color;
	}

	void SetColorTop( int r, int g, int b, int a=255 ) {
		F3dColor color = (a<<24)+(r<<16)+(g<<8)+b;
		color_lu = color_tv = color;
	}
	void SetColorBottom( int r, int g, int b, int a=255 ) {
		F3dColor color = (a<<24)+(r<<16)+(g<<8)+b;
		color_ru = color_bv = color;
	}

	void SetColorLeft( int r, int g, int b, int a=255 ) {
		F3dColor color = (a<<24)+(r<<16)+(g<<8)+b;
		color_lu = color_ru = color;
	}
	void SetColorRight( int r, int g, int b, int a=255 ) {
		F3dColor color = (a<<24)+(r<<16)+(g<<8)+b;
		color_tv = color_bv = color;
	}

	void SetColor( int r, int g, int b, int a=255 ) {
		F3dColor color = (a<<24)+(r<<16)+(g<<8)+b;
		color_lu = color_tv = color_ru = color_bv = color;
	}

	void SetLeftRatio( const UI_RECT& ui_rect, const double fRatio )	{
		rtScreen.left = (int)(ui_rect.left+ui_rect.GetWidth()*fRatio);
	}

	void SetRightRatio( const UI_RECT& ui_rect, const double fRatio )	{
		rtScreen.right = (int)(ui_rect.left+ui_rect.GetWidth()*fRatio);
	}

	void SetWidthRatio( const double fRatio ) {
		rtScreen.right = (int)(rtScreen.left + ori_rect.GetWidth()*fRatio); 
	}

	void SetHeightRatio( const double fRatio ) {
		rtScreen.bottom = (int)(rtScreen.top + ori_rect.GetHeight()*fRatio); 
	}

	void SetLeftU_Ratio( const double fRatio ) {
		lu = (float)fRatio;
	}
	void SetRightU_Ratio( const double fRatio ) {
		ru = (float)fRatio;
	}
	void SetTopV_Ratio( const double fRatio ) {
		tv = (float)fRatio;
	}
	void SetBottomV_Ratio( const double fRatio ) {
		bv = (float)fRatio;
	}
	void SetUV( const float flu, const float ftv, const float fru, const float fbv ) {
		lu = flu;
		tv = ftv;
		ru = fru;
		bv = fbv;
	}
	void SetPosition( int nX, int nY ) {
		UI_RECT val_rect = ori_rect + UI_RECT( nX, nY, nX, nY );
		rtScreen.left   = val_rect.left;
		rtScreen.top    = val_rect.top;
		rtScreen.right  = val_rect.right;
		rtScreen.bottom = val_rect.bottom;	
	}

	void SetClipRect( int left, int top, int right, int bottom )
	{
		rtClip.left   = left;
		rtClip.top    = top;
		rtClip.right  = right;
		rtClip.bottom = bottom;
		bUseClip = true;
	}

	void SetRotation( float fRadian ) { m_fRadian = fRadian; }

	const bool IsShow()    const { return bShow;    }
	const bool IsUseClip() const { return bUseClip; }

	const int GetWidth()  const { return rtScreen.right -rtScreen.left; }
	const int GetHeight() const { return rtScreen.bottom-rtScreen.top;  }

	float	lu, tv, ru, bv;
	float	back_lu, back_tv, back_ru, back_bv;
	F3dColor		color_lu, color_tv, color_ru, color_bv;
	UI_RECT			ori_rect;
	RECT			rtScreen;
	UINT64			TextureID;
	bool			bText;
	bool			bShow;
	bool			bIsTop;

	int				nDrawType;
	short			nDegree;
	bool			bReverse;

	UI_RECT			rtClip;
	bool			bUseClip;

	float			m_fRadian;
	bool			bSkipRelease;
	bool			bBillboard;
	bool			b3DRender;
	bool			b3DCursor;
	F3dVector		vtxCenter;
	F3dVector		vtxWindowScale;
};

typedef boost::shared_ptr< F3dCUISprite > F3dCUISpritePtr;

//스프라이트 로딩 객체
struct F3dZUITexture
{
	F3dZUITexture()
	{
		nWidth    = 0;
		nHeight   = 0;
		TextureID = 0;
	}

	~F3dZUITexture();

	std::wstring	strName;
	int				nWidth;
	int				nHeight;
	UINT64			TextureID;
};

//스프라이트
struct F3dZUISprite
{
	enum _SET_TYPE_
	{
		SET_NORMAL  = 0,
		SET_OVER    = 1,
		SET_DOWN    = 2,
		SET_DISABLE = 3,

		SET_CHECK_OFF      = 0,
		SET_CHECK_OFF_OVER = 1,
		SET_CHECK_ON       = 2,
		SET_CHECK_ON_OVER  = 3,
		SET_CHECK_DISABLE  = 4,
	};

	F3dZUISprite()
	{
		bLoaded   = false;
		nSetCount = 0;
	}

	~F3dZUISprite()
	{
		for( int  j(0); nSetCount>j; ++j )
		{
			for( UINT i(0); vTexList[j].size()>i; ++i )
				delete vTexList[j][i];
			vTexList[j].clear();
		}
		delete [] vTexList;
	}

	bool		 bLoaded;
	int          nSetCount;
	std::wstring strType;
	std::wstring strName;
	std::vector<F3dZUITexture*>* vTexList;
};

class F3dZUISpriteManager : public FBSingleton< F3dZUISpriteManager >
{
public:
	F3dZUISpriteManager();
	virtual ~F3dZUISpriteManager();

	bool	Load( SPRITE_ARG& sprite_arg, bool bTextureLoad=false );
	const F3dZUISprite* GetSprite( const WCHAR* pAniName );

	const F3dZUISprite* GetSpriteInfo( const WCHAR* pAniName );

	//Guild Icon 등, User Contents 용
	const F3dZUISprite* GetDynamicSprite( const WCHAR* pFullPath, const WCHAR* pFileName );

	void Init();
	void Finish();

	//for Tool
	void AddSprite( F3dZUISprite* pNewSpr );
	void DelSprite( const WCHAR* pAniName );
	void GetSpriteList( std::vector<const F3dZUISprite*>& sprList );

protected:

	void AddDynamicSprite( const WCHAR* pFullPath, const WCHAR* pFileName );

	boost::unordered_map< std::wstring, F3dZUISprite* > m_vSpriteHash;

	boost::unordered_map< std::wstring, F3dZUISprite* > m_vDynamicSpriteHash;

private:
};
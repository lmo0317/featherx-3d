
#pragma once

class F3dZWnd; 

class F3dZControl
{
public:

	enum _RET_CTL_FLAG
	{
		_RET_CTL_NONE = 1 << 1,
		_RET_CTL_USED = 1 << 2,
	};

	enum _STYLE_FLAG_
	{
		_NONE_FLAG     = 0,
		_CAPTURE_FLAG  = 1 << 1,
		_PASSED_FLAG   = 1 << 2,
		_DRAG_FLAG     = 1 << 3,
		_MSG_SKIP_FLAG = 1 << 4,
		_MSG_TOOL_TIP  = 1 << 5,
	};

	F3dZControl();
	virtual ~F3dZControl();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer );

	virtual int		OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, int& nPrevRetValue );
	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	
	virtual void	OnPosChange();
	virtual void	SetRenderTopLevel( bool bFlag );

	virtual void	SetClipRect( int nLeft, int nTop, int nRight, int nBottom );

	virtual void	SetRotation( float fRadian );

	void			onRefreshText( const RECT& rcTexRect );

	void			SetSkipAlpha( bool bFlag )	{ m_bSkipAlpha = bFlag; }
	const bool      IsSkipAlpha() const			{ return m_bSkipAlpha; }

	virtual void onEnable( const BOOL bFlag ) { }
	void		Enable();
	void		Disable();
	const BOOL	IsEnable() const { return m_bEnable; }

	void        SetParent( F3dZWnd* pParentWnd );
	F3dZWnd*    GetParent()                      { return m_pParentWnd; }

	virtual void		SetSize( const int& nWidth, const int& nHeight );
	void		SetPos( const int& nX, const int& nY );
	void		GetPos( int& nX, int& nY )				{ nX = m_nPosX; nY = m_nPosY; }

	void		SetDepth( int nDepth )	{ m_nDepth = nDepth; }
	const int	GetDepth() const		{ return m_nDepth;	 }
	const WCHAR* GetID()				{ return m_ID.c_str(); }

	virtual void	SetCaption( const WCHAR* pStr, BOOL bNew=FALSE );
	virtual void	SetToolTip( const WCHAR* pStr );
	virtual void	SetToolTipEx( const WCHAR* pStr, const WCHAR* pTitle );

	virtual void	SetSubToolTip( const WCHAR* pStr );
	virtual void	SetSubToolTipEx( const WCHAR* pStr, const WCHAR* pTitle );

	void SetResizeCaption( const BOOL bFlag ) { m_bResizeCaption = bFlag; }

	const int    GetCaptionID() const { return m_nCaptionID; }
	const WCHAR* GetCaption()		{ return m_strCaption.c_str(); }
	const WCHAR* GetToolTip()       { return m_strToolTip.c_str(); }
	const WCHAR* GetToolTipTitle()  { return m_strToolTipTitle.c_str(); }

	const WCHAR* GetSubToolTip()       { return m_strSubToolTip.c_str(); }
	const WCHAR* GetSubToolTipTitle()  { return m_strSubToolTipTitle.c_str(); }


	const int    GetToolTipID() const { return m_nToolTipID; }

	const WCHAR* GetSpriteName()	{ return m_strSprite.c_str();  }

	void		ChangeSpriteAni( const F3dZUISprite* pSprite );
	
	void		SetRenderSkip( BOOL bFlag ) { m_bRenderSkip = bFlag; }
	const BOOL	IsRenderSkip() const { return m_bRenderSkip; }

	void		SetShow( BOOL bShow ) { m_bRender = bShow; OnShow(); }
	bool		IsShow() const { return m_bRender != 0; }

	virtual void OnShow() { setState( F3dZUISprite::SET_NORMAL ); }

	void		ParentPosChangeNotify( int offset_x, int offset_y );
	void		ParentSizeChangeNotify(int nWidth,int nHeight);

	virtual void		MovePos( int x, int y );
	void		MovePosOffset( int offsetx, int offsety );

	void		GetRect( UI_RECT& rect ) const	{	rect = m_rect; 	}
	UI_RECT&	GetRect()    { return m_rect; }
	UI_RECT&	GetOriRect() { return m_ori_rect; }

	void		AddTextAlign( DWORD dwAlign ) { m_dwTextAlign |= dwAlign; }
	void		SubTextAlign( DWORD dwAlign ) { m_dwTextAlign ^= dwAlign; }

	void	createCustomControl_0( const CONTROL_ARG& control_arg );
	void	createCustomControl_1( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );
	void	createCustomControl_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );
	void	createCustomControl_Vertical_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );
	void	createCustomControl_9( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );

	void    SetVertical( BOOL bValue )	{ m_bIsVertical = bValue; }
	const BOOL    IsVertical() const { return m_bIsVertical; }

	void		SetFlag( DWORD dwFlag ) { m_dwFlag  = dwFlag; }
	void		AddFlag( DWORD dwFlag ) { m_dwFlag |= dwFlag; }

	void		SetTextFlag( DWORD dwTextFlag ) { m_dwTextFlag  = dwTextFlag; }
	void		AddTextFlag( DWORD dwTextFlag ) { m_dwTextFlag |= dwTextFlag; }
	void		SubTextFlag( DWORD dwTextFlag ) { m_dwTextFlag ^= dwTextFlag; }

	const BOOL  IsFlag( DWORD dwFlag )  { return ( m_dwFlag & dwFlag ) ? TRUE : FALSE; }
	const BOOL  IsBold() { return m_dwTextFlag & F3dZUIText::FLAG_BOLD; }

	//Text
	void		SetFontName ( const WCHAR* pName   ) { m_strFontName = pName;     }
	void		SetFontSize ( const int nFontSize  ) { m_nFontSize   = nFontSize; }
	void		SetTextColor( const F3dColor color ) { m_TextColor   = color;     }

	const WCHAR* GetFontName() { return m_strFontName.c_str(); }
	const int    GetFontSize() { return m_nFontSize; }

	void		SplitStringTag( const std::wstring& strIn, std::vector<MCParser::TEXT_DATA>& outList );
	void		SplitString( const std::wstring& strIn, std::vector< std::wstring >& outList );
	void		SearchItemToolTipLinkString( const std::wstring& strIn, int nItemCode, int nCount, int nRemainTime, int nDurability, int nEnhance_level, std::vector< int > vOptCode, std::vector< int > vOptValue, int nOptGroup, std::vector< int > vSocketCode, std::vector< int > vSocketValue, std::vector< BYTE > vSocketColor, int nSockCount, int nbyReconState, int nReconNum, int nActivityValue );
	void		GetToolTipLinkItem( int& nItemCode, int& nCount, int& nRemainTime, int& nDurability, int& nEnhance_level, std::vector< int >& vOptCode, std::vector< int >& vOptValue, int& nOptGroup, std::vector< int >& vSocketCode, std::vector< int >& vSocketValue, std::vector< BYTE >& vSocketColor, int& nSockCount, int& nbyReconState, int& nReconNum, int& nActivityValue );
	void		ClearToolTipLinkItem();

	virtual ZUIDragAndDropObject* GetDragObject();
	void		OnDragObjectDroped();

	void		CopyRect( F3dCUISprite* pSpr );

	virtual void SetAlpha( float fRatio );
	virtual void SetColor( int r, int g, int b, int a=255 );
	virtual void SetColorTop( int r, int g, int b, int a );
	virtual void SetColorBottom( int r, int g, int b, int a );

	virtual void SetColorLeft( int r, int g, int b, int a );
	virtual void SetColorRight( int r, int g, int b, int a );

	virtual void SetUV( const float flu, const float ftv, const float fru, const float fbv );

	static	void SetDownReact( double dTime ) { s_dDownReact = dTime; }

	void SetTextRoll( bool bFlag ) { m_bTextRoll = bFlag; }
	void SetBillBoard( bool bFlag );
	void Set3DRender(bool bFlag);
	void Set3DCursor(bool bFlag);

	void SetCenterBillBoard( const F3dVector& vtxCenter );
	void SetToolTipType( int nIndex ) { m_nToolTipType = nIndex; }
	const int GetToolTipType() { return m_nToolTipType; }

	const UINT64 GetTextureID() const { return m_nTextureID; }

	void SetHeightResizeControl( bool bFlag, int nMin, int nMax );
	const BOOL IsHeightResizeControl() { return m_bIsResizeControlStart; }

	void SetReMoveControl( bool bFlag ) { m_bReMoveControl = bFlag; }

	void SetReact(bool bReact) {m_bReact = bReact;}

	void RefreshControl(BOOL bNew);

	void setStateType( int nState );
	void setConstraintStateChange( bool bStateChange );

	void GetLastSizeInfo( int& nWidth, int& nLine )
	{
		nWidth  = m_nLastWidth;
		nLine   = m_nLastLine;
	}

	F3dZUITextPtr GetCaptionControl() {return m_Caption;}

	virtual void SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight);

	virtual int  GetLineCount() { return 1; } 

	void SetVisibleCount( const int nCount );
	void SetCurScrollPos( const int nPos );

protected:

	void	clearSprite();

	void	OnChildPosChange( const int nOffSetX, const int nOffSetY );

	void	SetDefValue( const CONTROL_ARG& control_arg );

	void	OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID, WPARAM wParam=0, LPARAM lParam=0 );

	void	setState( F3dZUISprite::_SET_TYPE_ state );


	void	createControl_1( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	//Horizon
	void	createControl_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void	createControl_9( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	//Vertical
	void	createControl_Vertical_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	void	create1( const CONTROL_ARG& control_arg, const F3dZUITexture* pTex, int j );
	void	create3( const CONTROL_ARG& control_arg, const F3dZUITexture** pTex, int i, int j );
	void	create9( const CONTROL_ARG& control_arg, const F3dZUITexture** pTex, int i, int j );

	void	resizeControl_1( const int& nWidth, const int& nHeight );
	void	resizeControl_3( const int& nWidth, const int& nHeight );
	void	resizeControl_9( const int& nWidth, const int& nHeight );

	void	resizeControl_Vertical_3( const int& nWidth, const int& nHeight );

	void		addChildControl( F3dZControl* pChild );
	void		delChildControl( F3dZControl* pChild );
	F3dZControl* GetChildControl( const WCHAR* ID );

	void	refreshControl();

	void	renderCaption( double dTime );
	void    renderChild( double dTime );
	void    renderWire();

	void    parseFont( const WCHAR* pStr );
	void    parseStyle( const WCHAR* pStr );

	std::wstring	m_ID;
	int				m_nDepth;
	F3dZWnd*		m_pParentWnd;

	ZUIDragAndDropObject* m_pDragObject;

	BOOL			m_bEnable;

	int				m_nPosX;
	int				m_nPosY;
	UI_RECT			m_ori_rect;
	UI_RECT			m_rect;

	short			m_nSetCount;
	short			m_nState;
	BOOL			m_bLeftButton;
	BOOL			m_bRightButton;
	BOOL			m_bMiddleButton;

	bool			m_bRenderTopLevel;

	int				m_nCaptionID;
	int				m_nToolTipID;

	std::wstring	m_strPrevCaption;

	std::wstring	m_strCaption;
	std::wstring	m_strToolTip;
	std::wstring	m_strToolTipTitle;

	std::wstring	m_strSubToolTip;
	std::wstring	m_strSubToolTipTitle;

	std::wstring	m_strSprite;

	std::vector< F3dCUISpritePtr >*	m_vSprite;

	double			m_dTime;
	double			m_dDownTime;

	F3dZUITextPtr	m_Caption;
	DWORD			m_dwTextFlag;
	std::wstring	m_strFontName;
	int				m_nFontSize;
	DWORD			m_dwTextAlign;
	F3dColor		m_TextColor;
	F3dColor		m_DisTextColor;

	BOOL			m_bResizeCaption;
	BOOL			m_bRender;
	BOOL			m_bRenderSkip;

	BOOL			m_bIsVertical;
	//Wire
	BOOL			m_bWireRender;
	F3dCUISpritePtr	m_WireSprite;
	//Child
	std::vector< F3dZControl* > m_vChildList;

	DWORD			m_dwFlag;

	static double	s_dDownReact;

	bool			m_bBillboard;
	bool			m_b3DRender;
	bool			m_b3DCursor;
	bool			m_bTextRoll;

	int				m_nToolTipType;

	UINT64			m_nTextureID;

	//Resize
	bool			m_bResizeControl;
	bool			m_bIsResizeControlStart;
	int				m_nResizeMin_Y;
	int				m_nResizeMax_Y;
	POINT			m_nPrevMouseOffset;

	//Move
	bool			m_bReMoveControl;
	bool			m_bIsReMoveControlStart;

	bool			m_bReact;

	//State
	bool			m_bStateChange;

	//Pick Up Item ToolTip
	struct _ItemLink_
	{
		int nWidth_Start;
		int nWidth_End;
		int	nItemCode;
		int	nCount;
		int	nRemainTime;
		int	nDurability;
		int	nEnhance_level;
		int	nOptGroup;
		std::vector< int > vOptCode;
		std::vector< int > vOptValue;
		int	nSockCount;
		std::vector< int > vSockCode;
		std::vector< int > vSockValue;
		std::vector< BYTE > vSockColor;
		int	nbyReconState;
		int	nReconNum;
		int nActivityValue;
	};
	std::vector< _ItemLink_ >	m_vToolTipLinkItem;
	bool						m_bClickLinkItem;

	int				m_VisibleCount;
	int				m_nCurScrollPos;

	bool m_bSkipAlpha;

	int m_nLastLine;
	int m_nLastWidth;

	int		m_nCacheSplitStringSize;

private:

};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class F3dZControlGroup
{
public:
	F3dZControlGroup();
	~F3dZControlGroup();
	
	void AddControl( F3dZControl* pControl );

	void MovePos( int x, int y );
	void MovePosOffset( int offsetx, int offsety );

	void FirstPos();
	void LastPosSave();

	void SetShow( BOOL bShow );

	void SetRect( int x, int y );
	void SetRectOffset( int offsetx, int offsety );

	void RatioPos( float fRatio ); 

	void RatioPosX( float fRatio ); 
	void RatioPosY( float fRatio ); 

	void ReFirstPos();

protected:

	UI_RECT					  m_rect;
	std::vector<F3dZControl*> m_vCtrlList;
	std::vector<UI_RECT>      m_vFirstRectList;
	std::vector<UI_RECT>      m_vLastRectList;

private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include <algorithm>
inline float getClampRatio(float curTime, float totalTime)
{  
	return std::min( curTime/ totalTime, 1.f);
}
inline float getClampRatioPow(float curTime, float totalTime)
{  
	return std::min( (curTime * curTime) / (totalTime * totalTime),1.f);
}
inline float getClampRatioSqrt(float curTime, float totalTime)
{  
	return std::min( ::sqrtf(getClampRatio(curTime,totalTime)) , 1.f);
}

#pragma once

struct F3dZUISprite;
class F3dZEditableWnd; 

class F3dZEditableControl
{
public:

	enum _RET_CTL_FLAG
	{
		_RET_CTL_NONE = 1 << 1,
		_RET_CTL_USED = 1 << 2,
	};

	F3dZEditableControl();
	virtual ~F3dZEditableControl();

	virtual void		CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void		AfterCreateControl();
	virtual void		Process( double dTime, double dMeanDT );
	virtual void		Render( double dTime );
	virtual int			OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual int			OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	virtual void		OnPosChange();

	void				Enable();
	void				Disable();

	void				SetFontColor( DWORD dwValue )   { m_TextColor = dwValue; }
	void				SetFontBGColor( DWORD dwValue ) { m_TextBGColor = dwValue; }

	const DWORD 		GetFontColor()   const { return m_TextColor;   }
	const DWORD 		GetFontBGColor() const { return m_TextBGColor; }


	void        		SetParent( F3dZEditableWnd* pParentWnd ) { m_pParentWnd = pParentWnd; }
	F3dZEditableWnd*    GetParent()                      { return m_pParentWnd; }

	virtual void		SetSize( const int& nWidth, const int& nHeight );
	void				SetPos( const int& nX, const int& nY )	{ m_nPosX = nX; m_nPosY = nY; OnPosChange(); }
	void				GetPos( int& nX, int& nY )				{ nX = m_nPosX; nY = m_nPosY; }

	void				SetDepth( int nDepth )	{ m_nDepth = nDepth; }
	const int			GetDepth() const		{ return m_nDepth;	 }
	const WCHAR*		GetID()				{ return m_ID.c_str(); }

	void				SetCaptionID( int nID );
	const int			GetCaptionID() const { return m_nCaptionID; }
	void				SetCaption( const WCHAR* pStr );
	const WCHAR*		GetCaption()		{ return m_strCaption.c_str(); }
	const WCHAR* 		GetSpriteName()	{ return m_strSprite.c_str();  }

	void				SetToolTipID( int nID );
	const int			GetToolTipID() const { return m_nToolTipID; }

	void				SetToolTip( const WCHAR* pStr );
	const WCHAR*		GetToolTip()		{ return m_strToolTip.c_str(); }

	void				MovePos( int x, int y );

	void				SetSelect( bool bSelect ) { m_bSelect = bSelect; }
	void				SetCheck( bool bCheck ) { m_bCheck = bCheck; }
	bool				IsCheck()				{ return m_bCheck; }

//for Tool
	static void SetMoveGrid( const int nGrid ) { s_nGrid = nGrid; }
	void		GetRect( UI_RECT& rect )	{	rect = m_rect; 	}

	void	createCustomControl_1( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );
	void	createCustomControl_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );
	void	createCustomControl_Vertical_3( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );
	void	createCustomControl_9( const int& nSetCount, std::vector<F3dZUITexture*>& texList, const CONTROL_ARG& control_arg );

	void    SetVertical( BOOL bValue )	{ m_bIsVertical = bValue; }
	const BOOL    IsVertical() const { return m_bIsVertical; }

	void	SetFontName( const WCHAR* pName ) { m_strFontName = pName;     }
	void	SetFontSize( int nFontSize )	  { m_nFontSize   = nFontSize; }
	
	//Align
	void	SetAlignLeft( bool bValue )    { m_bLeft    = bValue; }
	void	SetAlignTop( bool bValue )	   { m_bTop     = bValue; }		
	void	SetAlignRight( bool bValue  )  { m_bRight   = bValue; }				
	void	SetAlignBottom( bool bValue )  { m_bBottom  = bValue; }			
	void	SetAlignCenter( bool bValue )  { m_bCenter  = bValue; }			
	void	SetAlignVCenter( bool bValue ) { m_bVCenter = bValue; }			
	
	//Font Flag
	void    SetItalic( bool bValue )   { m_bItalic  = bValue; }
	void    SetBold( bool bValue )     { m_bBold    = bValue; }
	void    SetUnder( bool bValue )    { m_bUnder   = bValue; }
	void    SetStriker( bool bValue )  { m_bStriker = bValue; }
	void    SetInverse( bool bValue )  { m_bInverse = bValue; }
	void    SetShadow( bool bValue )   { m_bShadow  = bValue; }
	void    SetOutLine( bool bValue )  { m_bOutLine = bValue; }
	void    SetGlow( bool bValue )     { m_bGlow    = bValue; }

	//Style
	void    SetPass( bool bValue )     { m_bPass    = bValue; }
	void    SetNone( bool bValue )     { m_bNone    = bValue; }
	void    SetCapture( bool bValue )  { m_bCapture = bValue; }
	void    SetDrag( bool bValue )     { m_bDrag    = bValue; }
	void    SetToolTip( bool bValue )  { m_bToolTip = bValue; }

	const WCHAR* GetFontName() { return m_strFontName.c_str(); }
	const int    GetFontSize() { return m_nFontSize;           }

	const bool    GetAlignLeft() const    { return m_bLeft;    }
	const bool    GetAlignTop() const     { return m_bTop;     }			
	const bool    GetAlignRight() const   { return m_bRight;   }				
	const bool    GetAlignBottom() const  { return m_bBottom;  }			
	const bool    GetAlignCenter() const  { return m_bCenter;  }			
	const bool    GetAlignVCenter() const { return m_bVCenter; }			

	const bool    GetItalic() const  { return m_bItalic;  }
	const bool    GetBold() const    { return m_bBold;    }
	const bool    GetUnder() const   { return m_bUnder;   }
	const bool    GetStriker() const { return m_bStriker; }
	const bool    GetInverse() const { return m_bInverse; }
	const bool    GetShadow() const  { return m_bShadow;  }
	const bool    GetOutLine() const { return m_bOutLine; }
	const bool    GetGlow() const    { return m_bGlow;    }

	const bool    IsPass()		const    { return m_bPass;    }
	const bool    IsNone()		const    { return m_bNone;    }
	const bool    IsCapture()	const    { return m_bCapture; }
	const bool    IsDrag()		const    { return m_bDrag;    }
	const bool    IsMsgSkip()	const    { return m_bMsgSkip; }
	const bool    IsToolTip()	const    { return m_bToolTip; }

	void	SetMoveAble( BOOL bMoveAble, int nX, int nY ) 
	{ 
		m_bIsMoveAble = bMoveAble; 
		if( m_bIsMoveAble ) {
			m_ptMovingOffset.x = nX - m_rect.left;
			m_ptMovingOffset.y = nY - m_rect.top;
		}
	}

	void SetShow( bool bShow ) { m_bShow = bShow; }
	const bool IsShow() const { return m_bShow; }

protected:

	void	SetDefValue( const CONTROL_ARG& control_arg );

	void	OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID );

	void	setState( F3dZUISprite::_SET_TYPE_ state );

	void	createControl_1( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void	createControl_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void	createControl_9( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	void	createControl_Vertical_3( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	void	addChildControl( F3dZEditableControl* pChild );
	F3dZEditableControl* GetChildControl( const WCHAR* ID );

	void	resizeControl_1( const int& nWidth, const int& nHeight );
	void	resizeControl_3( const int& nWidth, const int& nHeight );
	void	resizeControl_9( const int& nWidth, const int& nHeight );

	void	resizeControl_Vertical_3( const int& nWidth, const int& nHeight );

	void	renderCaption( double dTime );
	void    renderChild( double dTime );
	void    renderWire();

	void    parseFont( const WCHAR* pStr );
	void    parseStyle( const WCHAR* pStr );

	void    MovePosChild( int x, int y );

	std::wstring	m_ID;
	int				m_nDepth;
	F3dZEditableWnd*					m_pParentWnd;
	std::vector<F3dZEditableControl*>	m_vChildList;

	int				m_nPosX;
	int				m_nPosY;
	UI_RECT			m_ori_rect;
	UI_RECT			m_rect;

	short			m_nSetCount;
	short			m_nState;
	BOOL			m_bLeftButton;
	BOOL			m_bRightButton;
	BOOL			m_bMiddleButton;

	int				m_nToolTipID;
	std::wstring	m_strToolTip;

	int				m_nCaptionID;
	std::wstring	m_strCaption;
	std::wstring	m_strSprite;


	std::vector< F3dCUISpritePtr >*	m_vSprite;

	F3dZUITextPtr	m_Caption;


//for Tool

	BOOL			m_bIsVertical;
	BOOL			m_bIsMoveAble;
	POINT			m_ptMovingOffset;
	static int		s_nGrid;
	F3dColor		m_TextColor;
	F3dColor		m_TextBGColor;

	BOOL			m_bWireRender;
	F3dCUISpritePtr	m_vWireSprite;

	std::wstring	m_strFontName;
	int				m_nFontSize;
	DWORD			m_dwStyle;
	DWORD			m_dwFlag;
	DWORD			m_dwAlign;

	bool			m_bLeft;
	bool			m_bTop;
	bool			m_bRight;
	bool			m_bBottom;
	bool			m_bCenter;
	bool			m_bVCenter;

	bool			m_bItalic;
	bool			m_bBold;
	bool			m_bUnder;
	bool			m_bStriker;
	bool			m_bInverse;
	bool			m_bShadow;
	bool			m_bOutLine;
	bool			m_bGlow;

	bool			m_bPass;   
	bool			m_bNone;   
	bool			m_bCapture;
	bool			m_bDrag;   
	bool			m_bMsgSkip;
	bool			m_bToolTip;

	bool			m_bSelect;
	bool			m_bCheck;

	bool			m_bShow;

private:

};

#pragma once

class F3dZEditableWndManager;

class F3dZEditableWnd
{
public:

	enum _RET_WND_FLAG
	{
		_RET_WND_NONE = 1 << 1,
		_RET_WND_USED = 1 << 2,
	};

	F3dZEditableWnd( F3dZEditableWndManager* pWndManager );
	virtual ~F3dZEditableWnd();

	virtual BOOL	CreateWnd( const WINDOW_ARG& window_arg );
	virtual void    AddControl( F3dZEditableControl* pControl );
	virtual BOOL    RemoveControl( F3dZEditableControl* pControl );

	virtual void    SortControl();

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual int		OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetVal );

	virtual void	OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID ) {};

	F3dZEditableControl*	GetControl( const WCHAR* ID );

	BOOL 			SetCaptureControl( F3dZEditableControl* pControl );
	void			ReleaseCaptureControl( F3dZEditableControl* pControl );
	F3dZEditableControl*	GetCaptureControl() { return m_pCaptureControl; }

	BOOL			IsInRect( const int& x, const int& y );

	const WCHAR* GetID() { return m_ID.c_str(); }
	const WCHAR* GetCaptionID() { return m_CaptionID.c_str(); }
	

	void			Show( BOOL bShowMode ) { m_bShow = bShowMode; }
	BOOL			IsShow()			   { return m_bShow; }

	void			MovePos( int x, int y );
	void			MovePosOffset( int offsetX, int offsetY );
	const BOOL		IsMoveAble() const { return m_bMoving; }

	void			GetRect( UI_RECT& rect )	{	rect = m_rect; 	}
	void			SetSize( const int& nWidth, const int& nHeight );

	void            GetControlList( std::vector<std::wstring>& control_list );

	void            CheckGroup( int nX, int nY, UI_RECT& rect );

	static void SetMoveGrid( const int nGrid ) { s_nGrid = nGrid; }
	static void SetRenderBG( bool bRenderBG )  { s_bRenderBG = bRenderBG; }
	void SetSprBG( const WCHAR* pStr )  { m_strSprBG = pStr; }
	void			Save();
	void			GroupSave();
	void            RefreshBG();

	F3dZEditableControl*	GetCheckControl();

	void            ChangeOrderUp( const WCHAR* pID );
	void            ChangeOrderDn( const WCHAR* pID );

protected:

	UI_RECT								m_GroupRect;
	std::vector< F3dZEditableControl* > m_vGroupList;

	F3dZEditableControl*				m_pCaptureControl;

	std::wstring				m_ID;
	std::wstring				m_CaptionID;
	BOOL						m_bShow;

	UI_RECT						m_rect;
	UI_RECT						ori_m_rect;

	POINT						m_ptMovingOffset;

	BOOL						m_bMoving;

	F3dZEditableWndManager*				m_pWndManager;
	
	std::vector<F3dZEditableControl*>	m_vControlList;

	//BG
	F3dZEditableControl*		m_pBG;
	std::wstring				m_strSprBG;

	static int					s_nGrid;
	static bool					s_bRenderBG;
};
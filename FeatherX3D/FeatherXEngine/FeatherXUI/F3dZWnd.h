
#pragma once

class F3dZWndManager;

class F3dZWnd
{
public:

	enum _RET_WND_FLAG
	{
		_RET_WND_NONE = 1 << 1,
		_RET_WND_USED = 1 << 2,
	};

	enum _WND_FLAG_
	{
		_WND_NONE       = 1 << 1,
		_WND_PASS       = 1 << 2,
		_WND_MOVEABLE   = 1 << 3,
		_WND_LIMIT_MOVE = 1 << 4,
		_WND_SKIP       = 1 << 5,
		_WND_SKIP_3D_CHECK = 1 << 6,
	};

	F3dZWnd( F3dZWndManager* pWndManager );
	virtual ~F3dZWnd();

	virtual BOOL	CreateWnd( const WINDOW_ARG& window_arg );
	virtual void    AddControl( F3dZControl* pControl );

	void			ClearChildWnd();
	virtual void    AddChildWnd( F3dZWnd* pWnd, int nOffSetX=0, int nOffSetY=0 );
	virtual void    ShowChild( const WCHAR* ID, BOOL bShow );
	virtual void    SetParent( F3dZWnd* pWnd );

	virtual void    SortControl();

	virtual void	onRefreshText( const RECT& rcTexRect ) {};

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime, double dMeanDT );
	virtual void	RenderControl(double dTime,double dMeanDT);

	virtual void	RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer );

	virtual int		OnKeyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, int& nPrevRetVal );
	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetVal );

	virtual void	OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID, WPARAM wParam=0, LPARAM lParam=0 ) {};

	F3dZControl*	GetControl( const WCHAR* ID );

	BOOL 			SetCaptureControl( F3dZControl* pControl );
	void			ReleaseCaptureControl( F3dZControl* pControl );
	F3dZControl*	GetCaptureControl() { return m_pCaptureControl; }

	BOOL			IsInRect( const int& x, const int& y );

	const WCHAR* GetID() { return m_ID.c_str(); }
	const WCHAR* GetCaptionID() { return m_CaptionID.c_str(); }

	//NumberPad
	virtual void	OnNumberValue( int nValue ) {}

	//Drag
	ZUIDragAndDropObject*	GetDragObject();
	void					SetDragObject( ZUIDragAndDropObject* pDragObject, const POINT& ptOffset );
	void					SetDrop( ZUIDragAndDropObject* pDragObject, F3dZControl* pRecvControl );
	void					MoveDrop( ZUIDragAndDropObject* pDragObject, F3dZControl* pRecvControl );
	virtual ZUIDragAndDropRender* OnDragObject( ZUIDragAndDropObject* pDragObject ) { return NULL; }
	virtual void				  OnDropObject( ZUIDragAndDropObject* pDragObject, F3dZControl* pRecvControl ) { }
	virtual void				  OnMoveDropObject( ZUIDragAndDropObject* pDragObject, F3dZControl* pRecvControl ) { }
	virtual void				  OnDropEmpty( ZUIDragAndDropObject* pDragObject ) { }
	

	virtual	void    OnShow() { }
	virtual	void    OnAlphaOpenComplete() { }
	virtual	void    OnAlphaCloseComplete() { }

	virtual	void    RePosition() { }
	virtual	void    ReSize( bool bFlag = FALSE ) { }

	void			SetRenderTopLevel( bool bFlag );

	void			Show( BOOL bShowMode );
	BOOL			IsShow()			   { return m_bShow; }

	void			SetSize( const int& nWidth, const int& nHeight );
	void			SetSizeUpdate(const int& nWidth,const int& nHeight);
	virtual	void    OnChangeSize() { }

	virtual void	OnMove() { }

	virtual void	MovePosOnlyWnd( int x, int y );
	virtual void	MovePos( int x, int y );
	virtual void	MovePosOffset( int offsetX, int offsetY );

	const BOOL		IsMoveAble() const { return m_bMoving; }

	void			GetRect( UI_RECT& rect )	{	rect = m_rect; 	}

	void			SetWndFlag( DWORD dwFlag ) { m_dwFlag = dwFlag; }
	const BOOL		IsWndFlag( DWORD dwFlag ) { return ( m_dwFlag & dwFlag ) ? TRUE : FALSE; }

	virtual	void    OnLimitResolution() { }
	void			LimitResolution( int nLeft=0, int nTop=0, int nRight=0, int nBottom=0 );
	const bool		IsLimitResolution();

	virtual	void    CustomSetAlpha( float fValue ) {};

	void			RemoveControlforTemp( std::vector< F3dZControl* >& vRemoveList );
	void			AddControlforTemp( F3dZControl* pControl );

	//Tool Tip
	void			SetToolTip( F3dZControl* pControl, int nType );
	void			DisableToolTip( F3dZControl* pControl, int nType );
	void			SetToolTipType( int nIndex );

	//for Tool
	void            GetControlList( std::vector<std::wstring>& control_list );

	virtual void	SetAlphaOpen();
	virtual void	SetAlphaClose();

	virtual void	SetBillBoard( bool bFlag );
	virtual void	Set3DRender(bool bFlag);
	virtual void	Set3DCursor(bool bFlag);
	void			SetCenterBillBoard( const F3dVector& vtxCenter );
	void			SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight);
	void			SetCenter(const F3dVector& vtxCenter);
	void			SetAlpha( float fValue );
	const BOOL		IsShowReady() const;
	const BOOL		IsAlphaClose() const;
	void			ChildWndCenterAlign(int nWidth,int nHeight,int nStartX,int nStartY);
	void			ChildWndTopCenterAlign(int nWidth,int nHeight,int nStartX,int nStartY);
	virtual void	onCommand( const WCHAR* pCmd ) {}
	F3dVector		GetCenter() {return m_vtxCenter;} 
protected:

	enum _WND_STATE
	{
		ST_NONE,
		ST_ALPHA_INIT_OPEN,
		ST_ALPHA_INIT_CLOSE,
		ST_ALPHA_OPEN,
		ST_ALPHA_CLOSE,
	};

	void			setState( USHORT uState ) { m_uState = uState; }
	const USHORT	getState() const          { return m_uState; }

	const BOOL		IsShowChange() const	  { return m_bShowChange; }

	DWORD						m_dwFlag;
	F3dZControl*				m_pCaptureControl;

	double						m_dCurTime;
	double						m_dAlphaTime;

	USHORT						m_uState;

	std::wstring				m_ID;
	std::wstring				m_CaptionID;
	BOOL						m_bShow;
	BOOL						m_bShowChange;

	UI_RECT						m_rect;
	UI_RECT						ori_m_rect;

	POINT						m_ptMovingOffset;

	BOOL						m_bMoving;
	BOOL						m_bTopFocus;

	F3dZWndManager*				m_pWndManager;
	std::vector<F3dZControl*>	m_vControlList;
	std::vector<F3dZControl*>	m_vTempControlList;

	boost::unordered_map< std::wstring, F3dZControl* > m_vhashControl;

	ZUIDragAndDropIconRender*	m_pDragAndDropRender;

	float						m_fAlphaValue;

	std::vector< F3dZWnd* >     m_vChildWnd;
	F3dZWnd*					m_pParent;
	bool						m_b3DCursor;
	bool						m_b3DRender;
	F3dVector					m_vtxCenter;
};

typedef boost::shared_ptr< F3dZWnd > F3dZWndPtr;

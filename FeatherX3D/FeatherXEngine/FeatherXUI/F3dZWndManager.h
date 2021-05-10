
#pragma once

struct WndManagerReceiver
{
	WndManagerReceiver() {}
	virtual ~WndManagerReceiver() { }

	virtual void OnMsgBoxNotify( const WCHAR* winID, _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* pID, const WCHAR* pExtraMsg = NULL ) {}
};

class F3dZWndManager : public FBSingleton< F3dZWndManager >
{
public:
	F3dZWndManager();
	virtual ~F3dZWndManager();

	void		SetHandler( WndManagerReceiver* pUIHandler ) { m_pUIHandler = pUIHandler; }

	void		Init();
	void		Finish();

	void		Process( double dTime, double dMeanDT );
	void		Render( double dTime, double dMeanDT );

	BOOL CreateWnd( const WCHAR* pFileName, F3dZWnd* pWnd, bool bModal = false );
	BOOL CreateChildWnd( const WCHAR* pFileName, F3dZWnd* pWnd );

	F3dZWnd* GetWnd( const WCHAR* pID );
	F3dZWnd* GetModalWnd( const WCHAR* pID );
	F3dZWnd* GetSortWnd( const WCHAR* pID );
	F3dZWnd* GetDelaySortWnd( const WCHAR* pID );

	BOOL 	 ShowWnd( const WCHAR* pID, BOOL bShowMode, BOOL bFocus );
	BOOL	 SetRenderTop(const WCHAR* pID,BOOL bTop);
	void	 MoveWnd( const WCHAR* pID, int nPosX, int nPosY );

	void	 SetWindowFlag( const WCHAR* pID, DWORD dwFlag );

	void	 ShowAllWindow( BOOL bShowMode );

	BOOL	 IsModalShow();

	enum MSG_TYPE
	{
		TYPE_OK,
		TYPE_OKCANCEL,
		TYPE_YESNOCANCEL,
	};

	//Msg Box : Modal, Modalless
	void		OpenMessageWnd( F3dZWnd* pWnd, WINDOW_ARG& window_arg, USHORT nMsgBoxID, MSG_TYPE msgType, bool bModal, std::vector<int>& captionList, float fKeepTime );

	void		OnMsgBoxNotify( const WCHAR* winID, _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* pID, const WCHAR* pExtraMsg = NULL );

	//Name Panel
	void		AddExistPanel( F3dZWnd* pWnd, bool bDelay = false );
	void		AddPanel( F3dZWnd* pWnd, WINDOW_ARG& window_arg, bool bDelay = false );
	BOOL		IsExistPanel( const WCHAR* winID );

	void		AllRePosition();

	void		CheckShowToolTip( const WCHAR* pID );

	//Capture
	BOOL 		SetCapture( F3dZWnd* pWnd );
	void		ReleaseCapture( F3dZWnd* pWnd );
	F3dZWnd*	GetCaptureWnd() { return m_pCaptureWnd; }

	//Focus
	void		SetFocus(F3dZWnd* pWnd);
	F3dZWnd*	GetFocusWnd();

	BOOL        MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	//Receiver
	virtual void OnWndMsg( std::wstring ID, int nData ) { }

	//Drag And Drop
	void SetDragObject( ZUIDragAndDropObject* pDragObject, ZUIDragAndDropRender* pDragRender, const POINT& ptOffset );
	ZUIDragAndDropObject* GetDragObject();

	void		SetDragRenderPos( int nX, int nY ); 

	//Edit ฐüทร
	static void SetHandle( HWND hWnd, HINSTANCE hInst )	{ s_hWnd = hWnd; s_hInstance = hInst;	}
	static HWND GetHWND()								{ return s_hWnd;      }
	static HINSTANCE GetInstance()						{ return s_hInstance; }

	static BOOL	IsLeftBtnDown()							{ return s_bLeftButton;  }
	static BOOL	IsRightBtnDown()						{ return s_bRightButton; }
	static INT  curX()									{ return s_nX; }
	static INT  curY()									{ return s_nY; }

	void		EraseWnd( F3dZWnd* pWnd, bool bModal );
	void		EraseSortWnd( F3dZWnd* pWnd );

	void		EraseWndAll( F3dZWnd* pWnd );

	//Tool Tip
	enum
	{
		TOOLTIP_TITLE,
		TOOLTIP_MINI,
		TOOLTIP_COMPARE,

		TOOLTIP_MAX,
	};

	void		SetToolTipControl( F3dZControlToolTip* pToolTip, int nIndex );

	void		SetToolTip( F3dZControl* pControl, int nType );
	void		DisableToolTip( F3dZControl* pControl, int nType );
	void        RefreshToolTip( F3dZWnd* pWndParent );
	void		CloseToolTip();

	void		AddWnd( F3dZWnd* pWnd, bool bModal );

	void		SetMsgBoxShow( BOOL bShow ) { m_bIsMsgBoxShow = bShow; }

	void		SetDelaySortedWnd( float fValue ) { m_dDelaySortedWnd = fValue; }
	const bool  IsInRectAll(const int nX,const int nY);
	
protected:

	void		addModalWnd( F3dZWnd* pWnd );
	void		addWnd( F3dZWnd* pWnd );
	void		addSortWnd( F3dZWnd* pWnd );
	void		addDelaySortWnd( F3dZWnd* pWnd );

	BOOL        _mouseMsg( UINT uMsg, const int& x, const int& y );
	BOOL        _keyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );

	static HWND					s_hWnd;
	static HINSTANCE			s_hInstance;
	static BOOL					s_bLeftButton;
	static BOOL					s_bRightButton;
	static INT					s_nX;
	static INT					s_nY;

	WndManagerReceiver*			m_pUIHandler;

	F3dZWnd*					m_pCaptureWnd;
	F3dZWnd*					m_pFocusWnd;

	boost::unordered_map< std::wstring, F3dZWnd* > m_vModalHash;
	std::list< F3dZWnd* >					   m_listModal;

	boost::unordered_map< std::wstring, F3dZWnd* > m_vWndHash;
	std::list< F3dZWnd* >					   m_listWnd;

	boost::unordered_map< std::wstring, F3dZWnd* > m_vSortWndHash;
	std::list< F3dZWnd* >					   m_listSortWnd;

	double									   m_dCheckDelayTime;
	std::queue< F3dZWnd* >					   m_qDelaySortWnd;
	boost::unordered_map< std::wstring, F3dZWnd* > m_vDelaySortWndHash;

	// Drag And Drop
	ZUIDragAndDropObject*	m_pDragObject;
	ZUIDragAndDropRender*	m_pDragRender;
	POINT					m_ptDragRenderOffset;

	int						m_nActToolTip;
	F3dZControlToolTip*		m_pToolTip[TOOLTIP_MAX];
	F3dZControl*			m_pLastToolTip;

	BOOL					m_bIsMsgBoxShow;

	double					m_dDelaySortedWnd;
};



typedef boost::shared_ptr< F3dZWndManager > F3dZWndManagerPtr;

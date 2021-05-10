
#pragma once


class F3dZEditableWndManager : public FBSingleton< F3dZEditableWndManager >
{
public:
	F3dZEditableWndManager();
	virtual ~F3dZEditableWndManager();

	void		SetHandler( WndManagerReceiver* pUIHandler ) { m_pUIHandler = pUIHandler; }

	void		Process( double dTime, double dMeanDT );
	void		Render( double dTime );

	BOOL CreateWnd( const WCHAR* pFileName, F3dZEditableWnd* pWnd );
	BOOL AddWnd( F3dZEditableWnd* pWnd );

	F3dZEditableWnd*	GetWnd( const WCHAR* pID );
	BOOL 				ShowWnd( const WCHAR* pID, bool bShowMode );

	//Msg Box : Modal
	//Msg Box : Modalless

	//Capture
	BOOL 				SetCapture( F3dZEditableWnd* pWnd );
	void				ReleaseCapture( F3dZEditableWnd* pWnd );
	F3dZEditableWnd*	GetCaptureWnd() { return m_pCaptureWnd; }

	//Focus
	void				SetFocus(F3dZEditableWnd* pWnd);
	F3dZEditableWnd*	GetFocusWnd();

	BOOL        MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	//Receiver
	virtual void OnWndMsg( std::wstring ID, int nData ) { }

	//Edit 관련
	void CheckGroup( int nX, int nY, UI_RECT& rect );
	static void SetHandle( HWND hWnd, HINSTANCE hInst )	{ s_hWnd = hWnd; s_hInstance = hInst;	}
	static HWND GetHWND()								{ return s_hWnd;      }
	static HINSTANCE GetInstance()						{ return s_hInstance; }

	//컨트롤 위치 제어
	void	OnKeyDown( int nKey );

protected:

	BOOL        _mouseMsg( UINT uMsg, const int& x, const int& y );
	BOOL        _keyMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );

	static HWND					s_hWnd;
	static HINSTANCE			s_hInstance;

	WndManagerReceiver*			m_pUIHandler;

	F3dZEditableWnd*					m_pCaptureWnd;
	F3dZEditableWnd*					m_pFocusWnd;

	boost::unordered_map< std::wstring, F3dZEditableWnd* > m_vWndHash;
	std::list< F3dZEditableWnd* >					   m_listWnd;
	
};
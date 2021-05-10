#pragma once

class FCUIManager : public FBSingleton< FCUIManager > , public WndManagerReceiver
{
public:

	FCUIManager();
	~FCUIManager();

	void Init();
	void ShowWindow( const WCHAR* szName, BOOL bShow, BOOL bFocus = TRUE );
	void Process( double dTime, double dMeanDT );
	void Render( double dTime, double dMeanDT );

	FCUIGameMainPtr GetUIGameMain() {return m_pUIGameMain;}
	BOOL MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:

	F3dZWndManagerPtr	m_zWndManger;
	FCUIGameMainPtr m_pUIGameMain;
	FCUIStartPtr m_pUIStart;
	FCUIEndPtr m_pUIEnd;
};
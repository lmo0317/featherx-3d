#pragma once

class CFeatherXOutputWindow : public FBSingleton< CFeatherXOutputWindow >
{
public:
	CFeatherXOutputWindow( );
	~CFeatherXOutputWindow();

	void	Init();
	void	Render();
	void	Process();
	void	Destroy();

	void	CreateLogWindow();
	int		Log( LPSTR fmt, ... );

private:
	unsigned int	m_nTarget;
	char			m_szFilename[MAX_PATH];
	HWND			m_hwnd;
	HWND			m_hwndList;
	static	LRESULT CALLBACK
		CFeatherXOutputWindow::WndProcCFeatherXOutputWindow( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};
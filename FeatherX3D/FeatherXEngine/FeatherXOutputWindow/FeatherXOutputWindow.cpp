#include "FeatherXEnginePrecompile.h"

CFeatherXOutputWindow::CFeatherXOutputWindow( )
{
	CreateLogWindow();
}

CFeatherXOutputWindow::~CFeatherXOutputWindow()
{
	DestroyWindow( m_hwnd );
}

void	CFeatherXOutputWindow::Init()
{

}
void	CFeatherXOutputWindow::Render()
{

}
void	CFeatherXOutputWindow::Process()
{

}
void	CFeatherXOutputWindow::Destroy()
{

}


void CFeatherXOutputWindow::CreateLogWindow()
{
	int			x, y, cx, cy;
	WNDCLASS	wc;
	RECT		rc;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)CFeatherXOutputWindow::WndProcCFeatherXOutputWindow;
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= GetModuleHandle( NULL );
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH)GetStockObject( WHITE_BRUSH) ;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= L"ZLogWindow";

	RegisterClass(&wc);

	cx = ZF_LOG_WINDOW_CX;
	cy = ZF_LOG_WINDOW_CY;
	x = GetSystemMetrics( SM_CXSCREEN ) - cx;
	y = 0;
	m_hwnd = CreateWindowW( L"ZLogWindow", ZF_LOG_WINDOW_TITLE, WS_POPUP | WS_CAPTION, x, y, cx, cy, NULL, NULL, GetModuleHandle( NULL ), NULL );
	GetClientRect( m_hwnd, &rc );

	m_hwndList = CreateWindowW( L"LISTBOX", L"", WS_CHILD | WS_VSCROLL, 0, 0, rc.right, rc.bottom, m_hwnd, NULL, GetModuleHandle( NULL ), NULL );
	ShowWindow( m_hwnd, SW_SHOW );
	ShowWindow( m_hwndList, SW_SHOW );
}

int CFeatherXOutputWindow::Log( LPSTR fmt, ... )
{
	char	buff[1024];
	char	date[128];
	char	time[128];

	_strdate( date );
	_strtime( time );
	vsprintf( buff, fmt, (char *)(&fmt+1) );

	// Console에 출력할 경우 
	if( m_nTarget & ZF_LOG_TARGET_CONSOLE )
	{
		printf( "(date[%s] time[%s]) : %s\n", date, time, buff );
	}

	// Log File에 출력할 경우
	if( m_nTarget & ZF_LOG_TARGET_FILE )
	{
		FILE*	fp = NULL;
		fp = fopen( m_szFilename, "a+" );
		if( fp )
		{
			fprintf( fp, "(date[%s] time[%s]) : %s\n", date, time, buff );
			fclose( fp );
		}
	}

	// Log Window에 출력할 경우 
	if( m_nTarget & ZF_LOG_TARGET_WINDOW )
	{
		SendMessage( m_hwndList, LB_ADDSTRING, 0, (LPARAM) buff );
		UINT32 n = SendMessage( m_hwndList, LB_GETCOUNT, 0, 0L ) - 1;
		SendMessage( m_hwndList, LB_SETCURSEL, (WPARAM)n, 0L );
	}
	return 1;
}

LRESULT CALLBACK 
CFeatherXOutputWindow::WndProcCFeatherXOutputWindow( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}


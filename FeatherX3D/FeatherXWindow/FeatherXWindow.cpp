#include "FeatherXWindowPrecompile.h"

#include <d3d9.h>

HWND g_hWnd;
CGameMain g_GameMain;

/************************************************************************/
/* FeatherXWindow 관련 소스                                              */
/************************************************************************/

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	g_GameMain.MsgProc(hWnd,msg,wParam,lParam);

	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

bool InitWindow()
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"MEngine", NULL };
	RegisterClassEx( &wc ); 

	g_hWnd = CreateWindow( L"MEngine", L"MEngine", 
		WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	if(g_hWnd == NULL)
		return false;

	ShowWindow( g_hWnd, SW_SHOWDEFAULT );
	UpdateWindow( g_hWnd );
	return true;
}

bool DestroyWindow()
{	
	UnregisterClass( L"MEngine", (HINSTANCE)GetWindowLong(g_hWnd,GWL_HINSTANCE) );
	return true;
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	InitWindow();
	g_GameMain.Init(g_hWnd);

	MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,0U,0U,PM_REMOVE))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			g_GameMain.Process();
			g_GameMain.Render();				
		}			
	}
	g_GameMain.Destroy();
	DestroyWindow();

	return 0;
}


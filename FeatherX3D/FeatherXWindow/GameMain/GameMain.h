#pragma once

class CGameMain
{
public:

	CGameMain();
	~CGameMain();

	bool Init(HWND hWnd);
	bool Destroy();
	bool Process();
	bool Render();
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:

};
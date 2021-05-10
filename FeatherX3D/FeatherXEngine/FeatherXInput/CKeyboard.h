#pragma once

class CKeyboard
{
public:
	CKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd);
	~CKeyboard();

	bool  KeyDown(int key);
	bool  KeyUp(int key);

	bool  Update();

	void  Clear() { ZeroMemory(m_keys, 256 * sizeof(char)); }

	bool  Acquire();
	bool  Unacquire();

private:
	LPDIRECTINPUTDEVICE8  m_pDIDev;
	char    m_keys[256];
};
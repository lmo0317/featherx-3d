#pragma once

//#define DIRECTINPUT_VERSION 0x0800
#pragma once

class CFeatherXInput : public FBSingleton< CFeatherXInput >
{
public:
	CFeatherXInput();
	~CFeatherXInput();

	void	Init();
	void	Process();
	void	Render();
	void    Destroy();

	bool  Initialize(HWND hwnd, HINSTANCE appInstance, bool isExclusive = true, DWORD flags = 0);
	bool  Shutdown();

	void  AcquireAll();
	void  UnacquireAll();

	CKeyboard  *GetKeyboard() { return m_pKeyboard; }
	CMouse     *GetMouse()    { return m_pMouse; }

	bool  Update();

	bool  KeyDown(int key);
	bool  KeyUp(int key);

	bool SteeringLeft(int key);
	bool Steeringright(int key);

	bool  ButtonDown(int button);
	bool  ButtonUp(int button);
	void  GetMouseMovement(int &dx, int &dy);
	int   GetMouseWheelMovement();

	const bool IsKeyDown(const int nValue);

private:
	CKeyboard  *m_pKeyboard;
	CMouse     *m_pMouse;
	
	
	LPDIRECTINPUT8 m_pDI;
};
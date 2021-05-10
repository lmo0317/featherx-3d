#pragma once

class CMouse
{
public:
	CMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive = true);
	~CMouse();

	bool  ButtonDown(int button) { return (m_state.rgbButtons[button] & 0x80) ? true : false; }
	bool  ButtonUp(int button) { return (m_state.rgbButtons[button] & 0x80) ? false : true; }
	int   GetWheelMovement() { return m_state.lZ; }
	void  GetMovement(int &dx, int &dy) { dx = m_state.lX; dy = m_state.lY; }

	bool  Update();

	bool  Acquire();
	bool  Unacquire();

private:
	LPDIRECTINPUTDEVICE8  m_pDIDev;
	DIMOUSESTATE          m_state;
};
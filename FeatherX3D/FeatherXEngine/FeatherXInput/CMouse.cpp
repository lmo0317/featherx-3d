#include "FeatherXEnginePrecompile.h"

CMouse::CMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive)
{
	if (FAILED(pDI->CreateDevice(GUID_SysMouse, &m_pDIDev, NULL)))
	{
		// error processing
	}

	if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIMouse)))
	{
		// error processing
	}

	DWORD flags;
	if (isExclusive)
		flags = DISCL_BACKGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
	else
		flags = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;


	if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, flags)))
	{
		// error processing
	}

	if (FAILED(m_pDIDev->Acquire()))
	{
		// error processing
	}

	if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
	{
		// error processing
	}
}

CMouse::~CMouse()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}

bool CMouse::Update()
{
	if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
	{
		if (FAILED(m_pDIDev->Acquire()))
		{
			return false;
		}
		if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
		{
			return false;
		}
	}

	return true;
}

bool CMouse::Acquire()
{
	return (!FAILED(m_pDIDev->Acquire()));
}

bool CMouse::Unacquire()
{
	return (!FAILED(m_pDIDev->Unacquire()));
}

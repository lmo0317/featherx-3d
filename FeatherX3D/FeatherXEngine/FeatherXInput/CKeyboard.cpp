#include "FeatherXEnginePrecompile.h"

CKeyboard::CKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd)
{
	if (FAILED(pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDev, NULL)))
	{
		// error processing
	}

	if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIKeyboard)))
	{
		// error processing
	}

	if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		// error processing
	}

	if (FAILED(m_pDIDev->Acquire()))
	{
		// error processing
	}

	Clear();
}


CKeyboard::~CKeyboard()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}

bool CKeyboard::Update()
{
	if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
	{
		if (FAILED(m_pDIDev->Acquire()))
		{
			return false;
		}
		if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
		{
			return false;
		}
	}
	return true;
}

bool CKeyboard::Acquire()
{
	Clear();
	return (!FAILED(m_pDIDev->Acquire()));
}

bool CKeyboard::Unacquire()
{
	Clear();
	return (!FAILED(m_pDIDev->Unacquire()));
}

bool  CKeyboard::KeyDown(int key)
{
	return (m_keys[key] & 0x80) ? true : false; 
}

bool  CKeyboard::KeyUp(int key)
{
	return (m_keys[key] & 0x80) ? false : true; 
}

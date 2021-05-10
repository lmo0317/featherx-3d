#include "FeatherXEnginePrecompile.h"

CFeatherXInput::CFeatherXInput()
{
}
CFeatherXInput::~CFeatherXInput()
{
	SAFE_DELETE(m_pKeyboard);
	SAFE_DELETE(m_pMouse);
}

void	CFeatherXInput::Init()
{
	
}

void CFeatherXInput::Process()
{
}

const bool CFeatherXInput::IsKeyDown(const int nValue)
{
	if(KeyDown(nValue))
	{
		return true;
	}

	return false;
}

void CFeatherXInput::Render()
{

}

void    CFeatherXInput::Destroy()
{

}

bool CFeatherXInput::Initialize(HWND hwnd, HINSTANCE appInstance, bool isExclusive, DWORD flags)
{
	if (FAILED(DirectInput8Create(appInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void **)&m_pDI,NULL)))
		return false;

	if(flags & IS_USEKEYBOARD)
	{
		m_pKeyboard = new CKeyboard(m_pDI, hwnd);
		if(m_pKeyboard == NULL)
			return false;
	}
	if(flags & IS_USEMOUSE)
	{
		m_pMouse = new CMouse(m_pDI, hwnd, isExclusive);
		if (m_pMouse == NULL)
			return false;
	}
	if(flags & IS_USEOPENCV)
	{

	}
	return true;
}

bool CFeatherXInput::Shutdown()
{
	UnacquireAll();
	if(m_pKeyboard)
	{
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	if(m_pMouse)
	{
		delete m_pMouse;
		m_pMouse = NULL;
	}

	if (FAILED(m_pDI->Release()))
		return false;
	return true;
}

bool CFeatherXInput::Update()
{
	if(m_pKeyboard)
		m_pKeyboard->Update();

	if(m_pMouse)
		m_pMouse->Update();
	
	return true;
}

void CFeatherXInput::AcquireAll()
{
	if(m_pKeyboard)
		m_pKeyboard->Acquire();

	if(m_pMouse)
		m_pMouse->Acquire();
}

void CFeatherXInput::UnacquireAll()
{
	if(m_pKeyboard)
		m_pKeyboard->Unacquire();

	if(m_pMouse)
		m_pMouse->Unacquire();
}


bool  CFeatherXInput::KeyDown(int key)
{
	return (m_pKeyboard && m_pKeyboard->KeyDown(key)); 
}
bool  CFeatherXInput::KeyUp(int key)
{
	return (m_pKeyboard && m_pKeyboard->KeyUp(key)); 
}

bool  CFeatherXInput::ButtonDown(int button) 
{
	return (m_pMouse && m_pMouse->ButtonDown(button)); 
}
bool  CFeatherXInput::ButtonUp(int button)
{
	return (m_pMouse && m_pMouse->ButtonUp(button)); 
}
void  CFeatherXInput::GetMouseMovement(int &dx, int &dy)
{ 
	if (m_pMouse) m_pMouse->GetMovement(dx, dy); 
}
int   CFeatherXInput::GetMouseWheelMovement() 
{
	return (m_pMouse) ? m_pMouse->GetWheelMovement() : 0; 
}

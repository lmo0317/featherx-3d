#include "FeatherXClientPrecompile.h"

FCUIGameMain::FCUIGameMain(F3dZWndManager* pWndManager) :F3dZWnd(pWndManager)
{

}

FCUIGameMain::~FCUIGameMain()
{

}

void FCUIGameMain::Init()
{

}

void FCUIGameMain::Finish()
{

}

void FCUIGameMain::OnShow()
{
	m_pTime = (F3dZControlNumber*)GetControl(L"num");
	m_pTime->SetAllowOne(true);
	m_pTime->SetCipher(5);
	m_pTime->SetNumber(0);
}

void FCUIGameMain::RePosition()
{

}

void FCUIGameMain::Process( double dTime, double dMeanDT )
{
	F3dZWnd::Process( dTime, dMeanDT );	
}

void FCUIGameMain::SetTime(double dTime)
{
	m_pTime->SetNumber((int)dTime);
}


void FCUIGameMain::OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID, WPARAM wParam, LPARAM lParam )
{
	if( notify_msg == LBUTTON_CLICK )
	{
		if( !_wcsicmp( ID, L"make_room" ) )
		{
			
		}
	}
	else if( notify_msg == LBUTTON_DBL_CLICK )
	{	
	}
}
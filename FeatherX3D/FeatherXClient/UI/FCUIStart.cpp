#include "FeatherXClientPrecompile.h"

FCUIStart::FCUIStart(F3dZWndManager* pWndManager) :F3dZWnd(pWndManager),
m_dTime(0)
{

}

FCUIStart::~FCUIStart()
{

}

void FCUIStart::Init()
{

}
void FCUIStart::Finish()
{

}

void FCUIStart::OnShow()
{
	int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();
	
	m_pStart = GetControl(L"start");
	m_pStart->SetCaption(L"START");

	int nX, nY;
	F3dCDevice::GetSingleton()->GetScreenStart( nX, nY );

	MovePos( nX+nWidth/2 - m_rect.GetWidth()/2, nY+nHeight/2-m_rect.GetHeight()/2 );
}

void FCUIStart::RePosition()
{

}

void FCUIStart::Process( double dTime, double dMeanDT )
{
	m_dTime += dMeanDT;
	if(m_dTime > 5)
	{
		FCUIManager::GetSingleton()->ShowWindow(L"start",FALSE);
	}
}

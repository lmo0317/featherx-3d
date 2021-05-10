#include "FeatherXClientPrecompile.h"

FCUIEnd::FCUIEnd(F3dZWndManager* pWndManager) :F3dZWnd(pWndManager),
	m_dTime(0)
{

}

FCUIEnd::~FCUIEnd()
{

}

void FCUIEnd::Init()
{

}
void FCUIEnd::Finish()
{

}

void FCUIEnd::OnShow()
{
	int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();
	m_pEnd = GetControl(L"end");
	m_pEnd->SetCaption(L"END");

	int nX, nY;
	F3dCDevice::GetSingleton()->GetScreenStart( nX, nY );
	MovePos( nX+nWidth/2 - m_rect.GetWidth()/2, nY+nHeight/2-m_rect.GetHeight()/2 );
}

void FCUIEnd::RePosition()
{

}

void FCUIEnd::Process( double dTime, double dMeanDT )
{
	m_dTime += dMeanDT;
	if(m_dTime > 5)
	{
		FCUIManager::GetSingleton()->ShowWindow(L"start",FALSE);
	}
}

#include "FeatherXEnginePrecompile.h"

CFeatherXCamera::CFeatherXCamera():
	m_vLook(0,0,0),
	m_vEye(0,0,0),
	m_vUp(0,0,0),
	m_vecTargetPos(0,0,0),
	m_vecPos(0,0,0),
	m_fZoom(100)
{	
	/*
	SetZoom(2000.0f);
	SetTheta(180.0f);
	SetPhi(25.0f);

	SetMinPhi(-80.0f);
	SetMaxPhi(90.0f);
	SetMinZoom(5.0f);
	SetMaxZoom(100000.0f);

	SetUpCamera();
	*/
}
CFeatherXCamera::~CFeatherXCamera()
{

}

void CFeatherXCamera::Init()
{

}

void CFeatherXCamera::Process()
{

}

void CFeatherXCamera::Render()
{

}

void CFeatherXCamera::Destroy()
{

}

bool CFeatherXCamera::SetUpCamera()
{
	m_vUp.x = 0.0f;
	m_vUp.y = 1.0f;
	m_vUp.z = 0.0f;

	m_vLook = m_vecTargetPos;

	float radTheta = (m_fTheta * 2*F3D_PI)/360.0f;
	float radPhi = (m_fPhi * 2*F3D_PI)/360.0f;

	m_vEye.x = cos(radPhi)*sin(radTheta)*m_fZoom	+ m_vLook.x;
	m_vEye.y = sin(radPhi)*m_fZoom					+ m_vLook.y;
	m_vEye.z = cos(radPhi)*cos(radTheta)*m_fZoom	+ m_vLook.z;
	if(m_fTheta == 359)
		return false;

	return true;
}

bool CFeatherXCamera::RotateCamera(float dTheta, float dPhi)
{
	m_fTheta += dTheta;
	if(m_fTheta >= 360.0f)
		m_fTheta -= 360.0f;
	if(m_fTheta < 0)
		m_fTheta += 360.0f;

	if(m_fPhi + dPhi > MAX_PHI)
		m_fPhi = MAX_PHI;
	else if(m_fPhi + dPhi < MIN_PHI)
		m_fPhi = MIN_PHI;
	else
		m_fPhi += dPhi;
	return true;
}

bool CFeatherXCamera::ZoomCamera(float dZoom)
{
	if(m_fZoom + dZoom > MAX_ZOOM)
		m_fZoom = MAX_ZOOM;
	else if(m_fZoom + dZoom < MIN_ZOOM)
		m_fZoom = MIN_ZOOM;
	else
		m_fZoom += dZoom;
	return true;
}

namespace
{
	static F3dViewPort s_ViewPort;
}

void CFeatherXCamera::Get3Dto2DVector( const F3dVector & v3D, F3dVector & v2D )
{
	//int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	//int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();

	int nWidth = 0;
	int nHeight = 0;

	s_ViewPort.Width  = nWidth;
	s_ViewPort.Height = nHeight;
	s_ViewPort.X      = 0;
	s_ViewPort.Y      = 0;
	s_ViewPort.MinZ   = 0.f;
	s_ViewPort.MaxZ   = 1.f;

	F3dMatrix wlmat;
	Identity( wlmat );
	D3DXVec3Project( &v2D, &v3D, &s_ViewPort, &m_matProj, &m_matView, &wlmat );
}

void CFeatherXCamera::Get2Dto3DVector( const F3dVector & v2D, F3dVector & v3D )
{
	//int nWidth = F3dCDevice::GetSingleton()->GetViewWidth();
	//int nHeight= F3dCDevice::GetSingleton()->GetViewHeight();
	
	int nWidth = 0;
	int nHeight = 0;

	s_ViewPort.Width  = nWidth;
	s_ViewPort.Height = nHeight;
	s_ViewPort.X      = 0;
	s_ViewPort.Y      = 0;
	s_ViewPort.MinZ   = 0.f;
	s_ViewPort.MaxZ   = 1.f;

	F3dMatrix wlmat;
	Identity( wlmat );
	D3DXVec3Unproject( &v3D, &v2D, &s_ViewPort, &m_matProj, &m_matView, &wlmat );
}
#include "FeatherXEngineCommonPrecompile.h"

F3dCDisplay::F3dCDisplay()
:	m_pD3D( NULL ),
	m_pD3DDevice( NULL ),
	m_hWnd( NULL ),
	m_bFullScreen( false ),
	m_nWidth( 0 ),
	m_nHeight( 0 ),
	m_bFixedRatio( false ),
	m_nFixedRatioWidth( 16 ),
	m_nFixedRatioHeight( 9 )
{
}

F3dCDisplay::~F3dCDisplay()
{
	if( m_pD3DDevice != NULL) {
		m_pD3DDevice->Release();
		m_pD3DDevice=NULL;
	}
	if( m_pD3D != NULL) {
		m_pD3D->Release();
		m_pD3D=NULL;
	}
}

HRESULT	F3dCDisplay::InitDisplay( HWND hWnd, IDirect3D9* pD3D, IDirect3DDevice9* pD3DDevice, const D3DPRESENT_PARAMETERS& D3Dpp )
{
	m_hWnd = hWnd;

	m_pD3D = pD3D;
	m_pD3DDevice = pD3DDevice;
	m_D3Dpp = D3Dpp;

	LPDIRECT3DSURFACE9 pBackBuffer;
	m_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	pBackBuffer->GetDesc( &m_sfBackBuffer );
	pBackBuffer->Release();

	m_nWidth  = m_sfBackBuffer.Width;
	m_nHeight = m_sfBackBuffer.Height;
	m_bFullScreen = m_D3Dpp.Windowed == FALSE;

	return S_OK;
}

void F3dCDisplay::SetFullScreen( bool bFullScreen )
{
	if( m_bFullScreen == bFullScreen )
		return;

	m_bFullScreen = bFullScreen;
//	Resize( m_nWidth, m_nHeight, bFullScreen );
}

HRESULT F3dCDisplay::Resize( int nWidth, int nHeight, bool bFullScreen, UINT uRefreshRate )
{
	if( m_pD3DDevice == NULL )
		return S_FALSE;

	m_sfBackBuffer.Width = m_nWidth = m_D3Dpp.BackBufferWidth = nWidth;
	m_sfBackBuffer.Height= m_nHeight =m_D3Dpp.BackBufferHeight= nHeight;

	m_bFullScreen = bFullScreen;
	if( m_bFullScreen == false ) {
		// 현재 모드를 얻는다.
		D3DDISPLAYMODE d3ddm;
		HRESULT hResult = m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
		if( FAILED( hResult ) )
			return hResult;

		m_D3Dpp.BackBufferFormat = d3ddm.Format;
		m_D3Dpp.Windowed = TRUE;
		m_D3Dpp.FullScreen_RefreshRateInHz = 0;
	}
	else {
		m_D3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_D3Dpp.Windowed = FALSE;
		m_D3Dpp.FullScreen_RefreshRateInHz = uRefreshRate;
	}
	return S_OK;
}

HRESULT F3dCDisplay::Reset()
{
	return m_pD3DDevice->Reset( &m_D3Dpp );
}

HRESULT	F3dCDisplay::ChangeMultiSample( D3DMULTISAMPLE_TYPE MultiSampleType, DWORD MultiSampleQuality )
{
	//LOGOUT( L"ChangeMultiSample= "__FUNCTIONW__ );
	if( m_pD3DDevice == NULL )
		return E_FAIL;

	m_D3Dpp.MultiSampleType = MultiSampleType;
	m_D3Dpp.MultiSampleQuality = MultiSampleQuality;

	return S_OK;
}

HRESULT F3dCDisplay::ChangePresentationInterval( UINT PresentationInterval )
{
	if( m_pD3DDevice == NULL )
		return E_FAIL;

	m_D3Dpp.PresentationInterval = PresentationInterval;
	return S_OK;
}

void F3dCDisplay::SetFixedRatio( bool bFixedRatio, int nFixedRatioWidth, int nFixedRatioHeight )
{
	m_bFixedRatio = bFixedRatio;
	m_nFixedRatioWidth = nFixedRatioWidth;
	m_nFixedRatioHeight = nFixedRatioHeight;
}

#pragma once

class						F3dCDisplay
{
public:
							F3dCDisplay();
							~F3dCDisplay();

	HRESULT					InitDisplay( HWND hWnd, IDirect3D9* pD3D, IDirect3DDevice9*	pD3DDevice, const D3DPRESENT_PARAMETERS& D3Dpp );
	HRESULT					Resize( int nWidth, int nHeight, bool bFullScreen, UINT uRefreshRate );
	HRESULT					ChangeMultiSample( D3DMULTISAMPLE_TYPE MultiSampleType, DWORD MultiSampleQuality );
	HRESULT					ChangePresentationInterval( UINT PresentationInterval );

	HRESULT					Reset();

	void					SetFullScreen( bool bFullScreen );
	bool					IsFullScreen()  const { return m_bFullScreen; }

	//화면 정보
	const D3DSURFACE_DESC&	GetBackDesc()	const { return m_sfBackBuffer; }
	HWND					GetHWnd()		const { return m_hWnd; }

	IDirect3D9*				GetD3D()		const { return m_pD3D; }
	IDirect3DDevice9*		GetD3Device()	const { return m_pD3DDevice; }

	int						GetWidth()		const { return m_nWidth; }
	int						GetHeight()		const { return m_nHeight; }

	void					SetFixedRatio( bool bFixedRatio, int nFixedRatioWidth, int nFixedRatioHeight );
	bool					IsFixedRatio() const { return m_bFixedRatio; }
	int						GetFixedRatioWIdth() const { return m_nFixedRatioWidth; }
	int						GetFixedRatioHeight() const { return m_nFixedRatioHeight; }

	D3DPRESENT_PARAMETERS*	GetPresentParameters() { return &m_D3Dpp; }

private:
	D3DSURFACE_DESC			m_sfBackBuffer;

	HWND					m_hWnd;
	bool					m_bFullScreen;

	int						m_nWidth, m_nHeight;
	bool					m_bFixedRatio;
	int						m_nFixedRatioWidth, m_nFixedRatioHeight;

	D3DPRESENT_PARAMETERS	m_D3Dpp;

	// D3D interface
	IDirect3D9*				m_pD3D;
	IDirect3DDevice9*		m_pD3DDevice;
};

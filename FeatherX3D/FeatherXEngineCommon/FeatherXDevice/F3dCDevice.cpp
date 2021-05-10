#include "FeatherXEngineCommonPrecompile.h"

F3dCDevice::F3dCDevice()
:m_pD3D(NULL),
m_pD3DDevice(NULL),
m_pActiveDisplayMode(NULL),
m_pDisplay(NULL)
{
	ZeroMemory( m_aRenderState.data(), m_aRenderState.size() * sizeof( DWORD ) );
}
F3dCDevice::~F3dCDevice()
{

}

void F3dCDevice::Init()
{

}

void F3dCDevice::Process()
{

}

void F3dCDevice::Render()
{

}

void F3dCDevice::Destroy()
{

}

HRESULT F3dCDevice::InitD3D( HWND hWnd )
{

	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;               
	ZeroMemory( &d3dpp, sizeof(d3dpp) );       
	d3dpp.Windowed = TRUE;                     
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;  
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &m_pD3DDevice ) ) )
	{
		return E_FAIL;
	}

	GetD3DDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	m_pDisplay = new F3dCDisplay;
	HRESULT hResult = m_pDisplay->InitDisplay( hWnd, m_pD3D, m_pD3DDevice, d3dpp );
	if( FAILED( hResult ) ) {
		DX_LOGOUT( hResult, L"Init Display Fail -" );
		return hResult;
	}

	return S_OK;
}

VOID F3dCDevice::Cleanup()
{
	if( m_pD3DDevice != NULL) 
		m_pD3DDevice->Release();

	if( m_pD3D != NULL)
		m_pD3D->Release();
}


void F3dCDevice::DrawIndexedPrimitive( D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount )
{
	IDirect3DDevice9* pD3DDevice = GetD3DDevice();
	pD3DDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

void F3dCDevice::DrawIndexedPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void * pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride )
{
	IDirect3DDevice9* pD3DDevice = GetD3DDevice();
	pD3DDevice->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride );
}

void F3dCDevice::DrawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride )
{
	IDirect3DDevice9* pD3DDevice = GetD3DDevice();
	pD3DDevice->DrawPrimitiveUP( PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride );
}

void F3dCDevice::DrawPrimitive( D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount )
{
	IDirect3DDevice9* pD3DDevice = GetD3DDevice();
	pD3DDevice->DrawPrimitive( PrimitiveType, StartVertex, PrimitiveCount );
}

void F3dCDevice::SetTexture(DWORD Sampler, IDirect3DBaseTexture9 * pTexture)
{
	IDirect3DDevice9* pD3DDevice = GetD3DDevice();
	pD3DDevice->SetTexture(Sampler, pTexture);
}

HRESULT F3dCDevice::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	_MBASSERT( State >= 0 && State < 300 );
	m_aRenderState[ State ] = Value;
	return	m_pD3DDevice->SetRenderState( State, Value );
}

DWORD F3dCDevice::GetRenderState( D3DRENDERSTATETYPE State )
{
	return m_aRenderState[ State ];
}

HRESULT F3dCDevice::SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	return m_pD3DDevice->SetTextureStageState( Stage, Type, Value );
}

void F3dCDevice::SetFVF( unsigned long fvf )
{
	m_pD3DDevice->SetFVF(fvf);
}


const F3dDisplayMode* F3dCDevice::GetDisplayMode( int nWidth, int nHeight ) const
{
	if( m_DispModeList.empty() )
		return NULL;
	std::vector< F3dDisplayMode >::const_iterator it = m_DispModeList.begin();
	for(; it != m_DispModeList.end(); ++it )
	{
		const F3dDisplayMode* pDM = &(*it);
		if( nWidth == pDM->nViewWidth && nHeight == pDM->nViewHeight ) {
			return pDM;
		}
	}

	int nMinV = 100000;
	std::vector< F3dDisplayMode >::const_iterator MinIT = m_DispModeList.begin();;
	it = m_DispModeList.begin();
	for(; it != m_DispModeList.end(); ++it )
	{
		const F3dDisplayMode* pDM = &(*it);
		int nV = abs( nWidth - pDM->nViewWidth ) + abs( nHeight - pDM->nViewHeight );
		if( nV < nMinV ) {
			MinIT = it;
			nMinV = nV;
		}
	}
	return &(*MinIT);
}

int F3dCDevice::GetViewWidth() const
{
	if( m_pActiveDisplayMode ) {
		return m_pActiveDisplayMode->nViewWidth;
	}
	return (int)m_pDisplay->GetBackDesc().Width;
}

int F3dCDevice::GetViewHeight() const
{
	if( m_pActiveDisplayMode ) {
		return m_pActiveDisplayMode->nViewHeight;
	}
	return (int)m_pDisplay->GetBackDesc().Height;
}

void F3dCDevice::GetScreenStart( int& x, int& y ) const
{
	int nViewW = F3dCDevice::GetSingleton()->GetViewWidth();
	int nViewH = F3dCDevice::GetSingleton()->GetViewHeight();
	F3dCDisplay* pDisplay = F3dCDevice::GetSingleton()->GetDisplay();

	x = (pDisplay->GetWidth() - nViewW)/2;
	y = (pDisplay->GetHeight() - nViewH)/2;
}
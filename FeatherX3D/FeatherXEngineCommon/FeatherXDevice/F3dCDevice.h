#pragma once
#include <boost/array.hpp>

struct F3dDisplayMode
{
	D3DDISPLAYMODE SysDM;	// 시스템 디스플래이모드.
	bool	bVirtualDM;		// 가상 displaymode인가.
	int		nViewWidth;		// 뷰 displaymode width
	int		nViewHeight;	// 뷰 displaymode height
};


class F3dCDevice : public FBSingleton<F3dCDevice>
{
public:
	F3dCDevice();
	~F3dCDevice();
	HRESULT InitD3D( HWND hWnd );
	VOID Cleanup();
	LPDIRECT3DDEVICE9 GetD3DDevice() {return m_pD3DDevice;}

	void Init();
	void Process();
	void Render();
	void Destroy();

	void					SetTexture(DWORD Sampler, IDirect3DBaseTexture9 * pTexture);
	void					DrawIndexedPrimitive( D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount );
	void					DrawIndexedPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void * pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride );
	void					DrawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride );
	void					DrawPrimitive( D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount );
	void					SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) { m_pD3DDevice->SetSamplerState(Sampler, Type, Value); }
	HRESULT					SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );
	DWORD					GetRenderState( D3DRENDERSTATETYPE State );
	HRESULT					SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value );
	void					SetFVF( unsigned long fvf );

	std::vector< F3dDisplayMode >& GetDisplayMode() { return m_DispModeList; }
	const F3dDisplayMode*	GetDisplayMode( int nWidth, int nHeight ) const;
	void					SetActiveDisplayMode( const F3dDisplayMode* pDM ) { m_pActiveDisplayMode = pDM; }
	const F3dDisplayMode*	GetActiveDisplayMode() const { return m_pActiveDisplayMode; }

	int						GetViewWidth() const;
	int						GetViewHeight() const;
	void					GetScreenStart( int& x, int& y ) const;

	F3dCDisplay*			GetDisplay()	const;

private:
	LPDIRECT3D9 m_pD3D; 
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	boost::array< DWORD, 300 > m_aRenderState;

	F3dCDisplay* m_pDisplay;
	std::vector< F3dDisplayMode > m_DispModeList;
	const F3dDisplayMode*	m_pActiveDisplayMode;
};

inline F3dCDisplay* F3dCDevice::GetDisplay() const
{
	return m_pDisplay;
}
#pragma once
/*
	F3dTexture
	Create: 2002, 12 by dobl
	Last  : 2002, 12 by dobl

	Desc: texture甫 包府
*/

class F3dCTexture;
typedef boost::shared_ptr< F3dCTexture > F3dCTexturePtr;

#ifndef TOOL_CLR
#define _MTLOADER // test
#endif
// texture
class					F3dCTexture
{
public:
	enum {
		TF_UNKNOWN	= D3DFMT_UNKNOWN,
		TF_R8G8B8	= D3DFMT_R8G8B8,
		TF_A8R8G8B8	= D3DFMT_A8R8G8B8,
		TF_X8R8G8B8	= D3DFMT_X8R8G8B8,
		TF__R5G6B5	= D3DFMT_R5G6B5,
		TF_X1R5G5B5	= D3DFMT_X1R5G5B5,
		TF_A1R5G5B5	= D3DFMT_A1R5G5B5,
		TF_A4R4G4B4	= D3DFMT_A4R4G4B4,

		TF_DXT1     = MAKEFOURCC('D', 'X', 'T', '1'),
		TF_DXT2     = MAKEFOURCC('D', 'X', 'T', '2'),
	};

	enum {
		TEXTURE_DIFFUSE		= 1,
		TEXTURE_SPECULAR	= 2,
		TEXTURE_BUMP		= 3,
		TEXTURE_REFLECTION	= 4,
	};

							F3dCTexture();
	virtual					~F3dCTexture();

	virtual void			LockRect( RECT *rect, void** ppBuf, int& stride );
	virtual void			Unlock();

	void					Clear();

	HRESULT					LostDevice();
	HRESULT					ResetDevice();
	bool					LostDeviceVerify();

	void					AddRef();
	int						Release();
	int						GetRef() const			{ return m_nAddRef; }
	void					SetRef( int nAddRef )	{ m_nAddRef = nAddRef; }

	bool					IsRenderTarget()	const	{ return m_bRenderTarget; }
	bool					IsCubeMap()			const	{ return m_bCubeMap; }
	const WCHAR*			GetName()			const	{ return m_strName.c_str(); }
	int						GetMapID()			const	{ return m_nMapID;   }
#ifdef _MTLOADER
	DWORD					GetWidth()			const	{ WaitForComplete(); return m_dwWidth;  }
	DWORD					GetHeight()			const	{ WaitForComplete(); return m_dwHeight; }
	DWORD					GetFormat()			const	{ WaitForComplete(); return m_dwFormat; }
#else
	DWORD					GetWidth()			const	{ return m_dwWidth;  }
	DWORD					GetHeight()			const	{ return m_dwHeight; }
	DWORD					GetFormat()			const	{ return m_dwFormat; }
#endif
	bool					IsDetail()			const	{ return m_bDetail; }

	void					SetMapID( int nMapID )		{ m_nMapID = nMapID; }
	void					SetDetail( bool bDetail )	{ m_bDetail = bDetail; }

#ifdef _MTLOADER
//	LPDIRECT3DTEXTURE9		GetTexture() const { WaitForComplete(); return m_pTexture; }
	LPDIRECT3DTEXTURE9		GetTexture( bool bWait = false ) const;
	LPDIRECT3DCUBETEXTURE9	GetCubeTexture() const { WaitForComplete(); return m_pCubeTexture; }
	bool					IsLoadComplete() const { return m_bLoadComplete; }
#else
	LPDIRECT3DTEXTURE9		GetTexture()		 const { return m_pTexture; }
	LPDIRECT3DCUBETEXTURE9	GetCubeTexture()	 const { return m_pCubeTexture; }
#endif
	ID3DXRenderToSurface*	GetRenderToSurface() const { return m_pRenderToSurface; }

#ifdef _MTLOADER
	static bool WaitForCompleteAll(unsigned long timeout = -1);
	static void Finish();

	void WaitForComplete() const;

	mutable CRITICAL_SECTION *m_CS;
	mutable volatile bool m_bLoadComplete;
#endif

	static HRESULT			Loader( const WCHAR *szFileName, F3dCTexturePtr& pTexture, DWORD dwColorKey = 0xFFFF00FF );
	static HRESULT			LoaderDetail( const WCHAR *szFileName, F3dCTexturePtr& pTexture, bool bDetail );
#ifdef _MTLOADER
	static HRESULT			_LoaderDetail( const WCHAR *szFileName, F3dCTexture* pTexture );
#endif
	static HRESULT			LoaderCube( const WCHAR *szFileName, F3dCTexturePtr& pTexture );
//	HRESULT					Loader( const F3dCDevice *pDevice, int nSrcData, const void *pSrcData );

	static HRESULT			CreateRenderTarget( int nWidth, int nHeight, D3DFORMAT Format, F3dCTexturePtr& pTexture );
	static HRESULT			CreateTexture( int width, int height, D3DFORMAT Format, D3DPOOL pool, int nLevel, F3dCTexturePtr& pTexture );
	HRESULT					BeginScene( const F3dViewPort& ViewPort );
	HRESULT					EndScene();

	void					SetName( const WCHAR* strName ) { m_strName = strName; }
	void					SaveTexture( const WCHAR* pFullPath );

	void					SetReleaseTime();
	double					GetReleaseTime() const { return m_dReleseTime; }

	friend class			F3dCTextureManager;

private:
	volatile int			m_nAddRef;
	bool					m_bCubeMap;
	bool					m_bLostDevice;
	bool					m_bRenderTarget;
	bool					m_bLocked;
	std::wstring			m_strName;
	int						m_nMapID;
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;
	DWORD					m_dwFormat;
	bool					m_bDetail;
	D3DFORMAT				m_Format;

	LPDIRECT3DSURFACE9		m_pSurfDest;
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DTEXTURE9		m_pWaitTexture;
	LPDIRECT3DCUBETEXTURE9	m_pCubeTexture;
	ID3DXRenderToSurface* m_pRenderToSurface;

	double					m_dReleseTime; // 昏力等 矫埃.
};
typedef boost::shared_ptr< F3dCTexture > F3dCTexturePtr;

inline void F3dCTexture::Clear()
{
	m_strName	= L"";
	m_nMapID	= TEXTURE_DIFFUSE;
	m_dwWidth	= 0;
	m_dwHeight	= 0;
	m_dwFormat  = 0;
	if( m_pTexture ) {
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

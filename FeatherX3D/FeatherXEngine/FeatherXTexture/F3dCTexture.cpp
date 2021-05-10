#include "FeatherXEnginePrecompile.h"
#include <process.h>
double g_dCurTime = 0;

F3dCTexture::F3dCTexture()
:	m_bCubeMap( false ),
	m_bLostDevice( false ),
	m_bRenderTarget( false ),
	m_bLocked( false ),
	m_pSurfDest( NULL ),
	m_pTexture( NULL ),
	m_pWaitTexture( NULL ),
	m_pCubeTexture( NULL ),
	m_pRenderToSurface( NULL ),
	m_bDetail( false ),
	m_nAddRef(0),
	m_dReleseTime( 0 )
{
	Clear();

#ifdef _MTLOADER
	m_CS = NULL;
	m_bLoadComplete = true;
#endif
}

F3dCTexture::~F3dCTexture()
{
	SAFE_RELEASE( m_pSurfDest );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pRenderToSurface );
	m_pWaitTexture = NULL;

#ifdef _MTLOADER
	if (m_CS != NULL)
	{
		DeleteCriticalSection(m_CS);
		delete m_CS;
	}
#endif
}

void F3dCTexture::AddRef()
{
	InterlockedIncrement( (volatile LONG *)&m_nAddRef );
}

int F3dCTexture::Release()
{
	if( m_pWaitTexture == NULL && m_pTexture == NULL )
		return 0;

	InterlockedDecrement( (volatile LONG *)&m_nAddRef  );
	if( m_nAddRef <= 0 ) {
	}
	return m_nAddRef;
}

struct _TIME_BOMB_
{
	_TIME_BOMB_( DWORD dwCheckTime ) : m_pFileName(NULL), m_nW(0), m_nH(0)
	{
		m_dwCheckTime = dwCheckTime;
		m_dwStart = getElapsedMilliSecond();
	}
	void SetInfo( const WCHAR* pFileName, int nW, int nH )
	{
		m_pFileName = pFileName;
		m_nW = nW;
		m_nH = nH;
	}
	~_TIME_BOMB_()
	{
		DWORD loadtime = getElapsedMilliSecond()-m_dwStart;
		if( loadtime >= m_dwCheckTime ) {
		}
	}

	const WCHAR* m_pFileName;
	int m_nW, m_nH;
	DWORD m_dwStart, m_dwCheckTime;
};

HRESULT F3dCTexture::Loader( const WCHAR *szFileName, F3dCTexturePtr& pTexture, DWORD dwColorKey )
{
	HRESULT hResult = E_FAIL;
	LPDIRECT3DTEXTURE9 pD3DTexture;

	hResult = D3DXCreateTextureFromFileExW( F3dCDevice::GetSingleton()->GetD3DDevice(),
		szFileName,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		dwColorKey,
		NULL,
		NULL,
		&pD3DTexture );

	if( FAILED( hResult ) ) {
		DX_LOGOUT( hResult, L"F3dCTexture::Loader - Fail : %s", szFileName );
		return hResult;
	}

	D3DSURFACE_DESC desc;
	pD3DTexture->GetLevelDesc( 0, &desc );

	pTexture.reset(new F3dCTexture);
	pTexture->m_strName	= szFileName;
	MBStringUtil::ToLower( pTexture->m_strName );
	pTexture->m_nMapID	= 0;
	pTexture->m_dwWidth	= desc.Width;
	pTexture->m_dwHeight= desc.Height;
	pTexture->m_dwFormat= desc.Format;
	pTexture->m_pTexture= pD3DTexture;
	pTexture->m_bRenderTarget = false;
	pTexture->m_bCubeMap = false;
	pTexture->m_bDetail = false;
	return hResult;
}

#ifdef _MTLOADER
static HANDLE s_Thread[4] = { 0, 0 };
static CRITICAL_SECTION s_CS;
static std::vector <F3dCTexturePtr> s_List;
static HANDLE s_hEvent, s_hEventInv;
static volatile bool s_bTerminate = false;
static F3dCTexturePtr s_pNullTexture;

static void _threadmain(void *param)
{
	while(s_bTerminate == false)
	{
		F3dCTexturePtr texture;

		EnterCriticalSection(&s_CS);
		if (s_List.empty() == true)
		{
			ResetEvent(s_hEvent);
			SetEvent(s_hEventInv);
		}
		else
		{
			std::vector <F3dCTexturePtr>::iterator it = s_List.begin();
			texture = *it;
			s_List.erase(it);
		}
		LeaveCriticalSection(&s_CS);

		if( texture != NULL )
			texture->WaitForComplete();

		WaitForSingleObject(s_hEvent, 5000);
	}
}

bool F3dCTexture::WaitForCompleteAll(unsigned long timeout)
{
	return WaitForSingleObject(s_hEventInv, timeout) == WAIT_TIMEOUT ? false : true;
}

void F3dCTexture::Finish()
{
	s_bTerminate = true;
	SetEvent(s_hEvent);
	WaitForMultipleObjects(_countof(s_Thread), s_Thread, TRUE, 50000);
	s_List.clear();
	s_pNullTexture.reset();
}

HRESULT F3dCTexture::LoaderDetail( const WCHAR *szFileName, F3dCTexturePtr& pTexture, bool bDetail )
{
	bool bFirst = false;
	if( s_Thread[0] == NULL )
	{
		bFirst = true;
		InitializeCriticalSection(&s_CS);
		s_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		s_hEventInv = CreateEvent(NULL, TRUE, TRUE, NULL);
		for(int i=0; i<_countof(s_Thread); i++)
			s_Thread[i] = (HANDLE)_beginthread(_threadmain, 0, 0);
	}

	pTexture.reset(new F3dCTexture);
	pTexture->m_strName = szFileName;
	MBStringUtil::ToLower( pTexture->m_strName );
	pTexture->m_bDetail = bDetail;

	if( bFirst ) {
		s_pNullTexture = pTexture;
	}
	else {
		pTexture->m_pWaitTexture = s_pNullTexture->m_pTexture;
	}

	pTexture->m_bLoadComplete = false;
	if( bFirst ) {
		pTexture->WaitForComplete();
	}
	else {
		EnterCriticalSection(&s_CS);
		s_List.push_back(pTexture);
		SetEvent(s_hEvent);
		ResetEvent(s_hEventInv);
		LeaveCriticalSection(&s_CS);
	}
	return D3D_OK;
}

void F3dCTexture::WaitForComplete() const
{
	if (m_bLoadComplete == true)
		return ;

	EnterCriticalSection(&s_CS);
	if (m_CS == NULL)
	{
		m_CS = new CRITICAL_SECTION;
		InitializeCriticalSection(m_CS);
	}
	LeaveCriticalSection(&s_CS);

	EnterCriticalSection(m_CS);
	if( m_bLoadComplete == false )
	{
		_LoaderDetail( m_strName.c_str(), (F3dCTexture*)this );
		m_bLoadComplete = true;
	}
	LeaveCriticalSection(m_CS);
}

HRESULT F3dCTexture::_LoaderDetail( const WCHAR *szFileName, F3dCTexture* pTexture )
#else
HRESULT F3dCTexture::LoaderDetail( const WCHAR *szFileName, F3dCTexturePtr& pTexture, bool bDetail )
#endif
{
	HRESULT hResult = E_FAIL;
	LPDIRECT3DTEXTURE9 pD3DTexture;
	float fDetail = F3dCTextureManager::GetSingleton()->GetDetail();
	D3DXIMAGE_INFO ImageInfo;

	WCHAR* pBuf    = NULL;
	int   nBufLen = 0;

	MagiMemoryStream	memStream;
	hResult = D3DXGetImageInfoFromFile(szFileName, &ImageInfo );

	if( hResult != S_OK ) {
		return hResult;
	}

#ifndef _MTLOADER
	pTexture.reset(new F3dCTexture);
	pTexture->m_strName = szFileName;
	MBStringUtil::ToLower( pTexture->m_strName );
	pTexture->m_bDetail = bDetail;
#endif

	if( pTexture->m_bDetail == false ) {
		fDetail = 1.f;
	}
	int nWidth = (int)(ImageInfo.Width * fDetail);
	int nHeight = (int)(ImageInfo.Height * fDetail);
	nWidth = std::max( 1, nWidth );
	nHeight = std::max( 1, nHeight );

if( MagiFileSystem::IsPackedFile() ) {
	if( pTexture->m_bDetail ) {
		hResult = D3DXCreateTextureFromFileInMemoryEx( F3dCDevice::GetSingleton()->GetD3DDevice(),
			pBuf,
			(UINT)nBufLen,
			nWidth,
			nHeight,
			ImageInfo.MipLevels, //mipmap
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0xFFFF00FF,
			NULL,
			NULL,
			&pD3DTexture );
	}
	else {
		hResult = D3DXCreateTextureFromFileInMemoryEx( F3dCDevice::GetSingleton()->GetD3DDevice(),
			pBuf,
			(UINT)nBufLen,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0xFFFF00FF,
			NULL,
			NULL,
			&pD3DTexture );
	}

} 
else {
	if( pTexture->m_bDetail ) {
		hResult = D3DXCreateTextureFromFileExW( F3dCDevice::GetSingleton()->GetD3DDevice(),
												szFileName,
												nWidth,
												nHeight,
												ImageInfo.MipLevels, //mipmap
												0,
												D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED,
												D3DX_DEFAULT,
												D3DX_DEFAULT,
												0xFFFF00FF,
												NULL,
												NULL,
												&pD3DTexture );
	}
	else {
		hResult = D3DXCreateTextureFromFileExW( F3dCDevice::GetSingleton()->GetD3DDevice(),
			szFileName,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0xFFFF00FF,
			NULL,
			NULL,
			&pD3DTexture );
	}

}
	if( FAILED( hResult ) ) {
		DX_LOGOUT( hResult, L"F3dCTexture::LoaderDetail - Fail : %s", szFileName );
		return hResult;
	}

	D3DSURFACE_DESC desc;
	pD3DTexture->GetLevelDesc( 0, &desc );
#ifndef _MTLOADER
	pTexture.reset(new F3dCTexture);
	pTexture->m_strName	= szFileName;
	MBStringUtil::ToLower( pTexture->m_strName );
#endif
	pTexture->m_nMapID	= 0;
	pTexture->m_dwWidth	= desc.Width;
	pTexture->m_dwHeight= desc.Height;
	pTexture->m_dwFormat= desc.Format;
	pTexture->m_pTexture= pD3DTexture;
	pTexture->m_pWaitTexture = NULL;
	pTexture->m_bRenderTarget = false;
	pTexture->m_bCubeMap = false;
	return hResult;
}

HRESULT F3dCTexture::LoaderCube( const WCHAR *szFileName, F3dCTexturePtr& pTexture )
{
	HRESULT hResult = S_OK;
	LPDIRECT3DCUBETEXTURE9 pD3DTexture;
	hResult = D3DXCreateCubeTextureFromFileExW( F3dCDevice::GetSingleton()->GetD3DDevice(),
		szFileName,
		D3DX_DEFAULT,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFFFF00FF,
		NULL,
		NULL,
		&pD3DTexture );
	if( FAILED( hResult ) )
		return hResult;

	D3DSURFACE_DESC desc;
	pD3DTexture->GetLevelDesc( 0, &desc );

	pTexture.reset(new F3dCTexture);
	pTexture->m_strName	= szFileName;
	MBStringUtil::ToLower( pTexture->m_strName );
	pTexture->m_nMapID	= 0;
	pTexture->m_dwWidth	= desc.Width;
	pTexture->m_dwHeight= desc.Height;
	pTexture->m_dwFormat= desc.Format;
	pTexture->m_pCubeTexture= pD3DTexture;
	pTexture->m_bRenderTarget = false;
	pTexture->m_bCubeMap= true;

//#ifdef _DEBUG
//	F3dCDevice::GetSingleton()->GetD3DDevice()->CheckVGAMem();
//#endif

	return hResult;
}

/*HRESULT F3dCTexture::Loader( const F3dCDevice *pDevice, int nSrcData, const void *pSrcData )
{  
	HRESULT hResult = S_OK;
	hResult = D3DXCreateTextureFromFileInMemory( pDevice->GetD3DDevice(),
										pSrcData,
										nSrcData,
										&m_pTexture );

	if( FAILED( hResult ) ) 
		return hResult;

	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc( 0, &desc );
	m_dwWidth	= desc.Width;
	m_dwHeight	= desc.Height;
	m_dwFormat	= desc.Format;

	return hResult;
}
*/

HRESULT F3dCTexture::CreateRenderTarget( int nWidth, int nHeight, D3DFORMAT Format, F3dCTexturePtr& pTexture )
{
	HRESULT hResult = S_OK;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();
	LPDIRECT3DTEXTURE9 pD3DTexture;
	hResult = pD3DDevice->CreateTexture( nWidth, nHeight, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &pD3DTexture, NULL );
	if( FAILED(hResult) ) {
		return DX_LOGOUT( hResult, L"m_pTexture CreateRenderTarget texture fail -" );
	}

	D3DSURFACE_DESC desc;
	pD3DTexture->GetLevelDesc( 0, &desc );

	pTexture.reset( new F3dCTexture );
	pTexture->m_nMapID	= 0;
	pTexture->m_dwWidth	= desc.Width;
	pTexture->m_dwHeight= desc.Height;
	pTexture->m_dwFormat= desc.Format;
	pTexture->m_pTexture= pD3DTexture;
	pTexture->m_Format  = Format;
	pTexture->m_bRenderTarget = true;

	hResult = D3DXCreateRenderToSurface( pD3DDevice, desc.Width, desc.Height, desc.Format, 
											TRUE, D3DFMT_D24S8,&(pTexture->m_pRenderToSurface) );

	if( FAILED(hResult) ) {
		return DX_LOGOUT( hResult, L"m_pTexture D3DXCreateRenderToSurface fail -" );
	}

	hResult = pD3DTexture->GetSurfaceLevel( 0, &pTexture->m_pSurfDest );
	if( FAILED( hResult ) ) {
		return DX_LOGOUT( hResult, L"m_pShadowTexture->GetTexture()->GetSurfaceLevel( 0, &m_pSurfDest )" );
	}

//#ifdef _DEBUG
//	F3dCDevice::GetSingleton()->GetD3DDevice()->CheckVGAMem();
//#endif

	return hResult;
}

HRESULT	F3dCTexture::CreateTexture( int width, int height, D3DFORMAT Format, D3DPOOL pool, int nLevel, F3dCTexturePtr& pTexture )
{
	HRESULT hResult = S_OK;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();

	if(pD3DDevice == NULL)
		return S_FALSE;

	LPDIRECT3DTEXTURE9 pD3DTexture;
	hResult = pD3DDevice->CreateTexture( width, height, nLevel, 0, Format, D3DPOOL_MANAGED, &pD3DTexture, NULL );
	if( FAILED(hResult) ) {
		return DX_LOGOUT( hResult, L"m_pTexture CreateTexture texture fail -" );
	}

	D3DSURFACE_DESC desc;
	pD3DTexture->GetLevelDesc( 0, &desc );

	pTexture.reset( new F3dCTexture );
	pTexture->m_nMapID	      = 0;
	pTexture->m_dwWidth	      = desc.Width;
	pTexture->m_dwHeight      = desc.Height;
	pTexture->m_dwFormat      = desc.Format;
	pTexture->m_pTexture      = pD3DTexture;
	pTexture->m_bRenderTarget = false;
	pTexture->m_bCubeMap      = false;

//#ifdef _DEBUG
//	F3dCDevice::GetSingleton()->GetD3DDevice()->CheckVGAMem();
//#endif

	return hResult;
}

void F3dCTexture::LockRect( RECT *rect, void** ppBuf, int& stride )
{
	if ( !m_bLocked && m_pTexture != NULL )	{
		D3DLOCKED_RECT rt;
		HRESULT hr = m_pTexture->LockRect( 0, &rt, rect, 0/*D3DLOCK_NOSYSLOCK*/ );
		if ( hr == D3D_OK )		{
			*ppBuf = rt.pBits;
			stride = rt.Pitch;
			m_bLocked = true;
		}
	}
	else	{
		*ppBuf = NULL;
		stride = 0;
	}
}

void F3dCTexture::Unlock()
{
	if ( m_bLocked ) {
		m_pTexture->UnlockRect(0);
		m_bLocked = false;
	}
}

HRESULT F3dCTexture::LostDevice()
{
	m_bLostDevice = true;
	if( m_bRenderTarget ) {
		SAFE_RELEASE( m_pRenderToSurface );
		SAFE_RELEASE( m_pTexture );
		SAFE_RELEASE( m_pSurfDest );
	}
	return S_OK;
}

HRESULT F3dCTexture::ResetDevice()
{
	if( m_bLostDevice == false )
		return S_OK;

	if( m_bRenderTarget == false ) 
		return S_OK;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();

	HRESULT hResult = S_OK;
	hResult = pD3DDevice->CreateTexture( m_dwWidth, m_dwHeight, 1, D3DUSAGE_RENDERTARGET, m_Format, D3DPOOL_DEFAULT, &m_pTexture, NULL );
	if( FAILED(hResult) ) {
		DX_LOGOUT( hResult, L"m_pTexRender Create texture fail -" );
		return hResult;
	}

	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc( 0, &desc );

	hResult = D3DXCreateRenderToSurface( pD3DDevice, desc.Width, desc.Height, desc.Format, TRUE, D3DFMT_D24S8, &m_pRenderToSurface );
	if( FAILED(hResult) ) {
		DX_LOGOUT( hResult, L"m_pTexture D3DXCreateRenderToSurface fail -" );
		return hResult;
	}

	hResult = m_pTexture->GetSurfaceLevel( 0, &m_pSurfDest );
	if( FAILED( hResult ) ) {
		DX_LOGOUT( hResult, L"m_pShadowTexture->GetTexture()->GetSurfaceLevel( 0, &m_pSurfDest )" );
		return hResult;
	}
	m_bLostDevice = false;

	return hResult;
}

bool F3dCTexture::LostDeviceVerify()
{
	if( m_bRenderTarget == false ) 
		return true;

	bool bResult = true;
	if( m_pRenderToSurface ) 
		bResult = false;

	if( m_pTexture )
		bResult = false;

	if( m_pSurfDest )
		bResult = false;

	if( bResult == false ) {
		LOGOUT( L"LostDeviceVerify=false "__FUNCTIONW__ );
	}
	return bResult;
}

HRESULT F3dCTexture::BeginScene( const F3dViewPort& ViewPort )
{
	if( m_bLostDevice == true )
		return E_FAIL;
	if( m_bRenderTarget == false ) 
		return E_FAIL;

	HRESULT hResult = m_pRenderToSurface->BeginScene( m_pSurfDest, &ViewPort );
	if( FAILED( hResult ) )
		return hResult;

	return S_OK;
}

HRESULT F3dCTexture::EndScene()
{
	if( m_bLostDevice == true )
		return E_FAIL;
	if( m_bRenderTarget == false ) 
		return E_FAIL;

#ifdef _RUSTY_BATCH
	F3dCDevice::GetSingleton()->GetD3DDevice()->Flush();
#endif
	return m_pRenderToSurface->EndScene( D3DX_FILTER_DITHER );
}

void F3dCTexture::SaveTexture( const WCHAR* pFullPath )
{
	if( m_pTexture )
		D3DXSaveTextureToFileW( pFullPath, D3DXIFF_TGA, m_pTexture, NULL );
}

void F3dCTexture::SetReleaseTime()
{
	m_dReleseTime = ::GetCurTime();
	m_nAddRef = 0;
}

#ifdef _MTLOADER
LPDIRECT3DTEXTURE9 F3dCTexture::GetTexture( bool bWait ) const
{
	if( bWait ) {
		WaitForComplete();
	}
	if( m_pTexture == NULL )
		return m_pWaitTexture;
	return m_pTexture;
}
#endif

#include "FeatherXEnginePrecompile.h"

F3dCTextureManager::F3dCTextureManager() :
Detail_(1.f),
	LastTextureID_(0),
	Disable_( false )
{
}

F3dCTextureManager::~F3dCTextureManager()
{
}

HRESULT F3dCTextureManager::Init()
{
	 

	bool bTemp = Disable_;
	Disable_ = false;
	CreateTexture( L"wait.dds", true );
	Disable_ = bTemp;
	return S_OK;
}

void F3dCTextureManager::Finish()
{
	 
	// delete null.dds
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( 0 );
	if( it != Texture_.end() ) {
		Texture_.erase( it );
	}
#ifdef _MTLOADER
	F3dCTexture::Finish();
#endif
	assert( Texture_.empty() );
	it = Texture_.begin();
	for(; it != Texture_.end(); ++it ) {
		LOGOUT( L"AllocTexture %s", it->second->GetName() );
	}
	Texture_.clear();
	TextureID_.clear();
	CubeTextureID_.clear();
	RenderTargetTextureID_.clear();
	CmpTexture_.clear();
	Texture4Render_.clear();
	RecycleTexture_.clear();
}

void F3dCTextureManager::Disable()
{
	Disable_ = true;
}

void F3dCTextureManager::Release( UINT64 TextureID )
{
	 
	if( TextureID == 0 )
		return;

	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( TextureID );
	if( it == Texture_.end() )
		return;

	int nRef = it->second->Release();
	if( nRef > 0 ) {
		return;
	}
	if( it->second->IsCubeMap() ) {
		CubeTextureID_.erase( it->second->GetName() );
	}
	else if( it->second->IsRenderTarget() ) {
		RenderTargetTextureID_.erase( it->second->GetName() );
	}
	else {
		const F3dCTexturePtr& pT = it->second;
		TextureID_.erase( pT->GetName() );
//		if( pT->IsLoadComplete() && pT->GetWidth() > 32 && pT->GetHeight() > 32 ) {
#ifdef _RECYCLE_TEXTURE
			it->second->SetReleaseTime();
			RecycleTexture_.insert( std::make_pair( pT->GetName(), pT ) );
#endif
//			LOGOUT( L"RecycleTexture_.size() = %d", RecycleTexture_.size() );
//		}
	}

	Texture_.erase( TextureID );
}

const UINT F3dCTextureManager::GetTextureCount()
{
	 
	return (UINT)Texture_.size();
}

UINT64 F3dCTextureManager::CreateTexture( const WCHAR* szName, bool bDetail )
{
	if( Disable_ == true )
		return 0;

	if( wcslen( szName ) < 2 ) //파일 이름 이상
		return 0;

	if( NULL == szName  ) {
		assert( false );
		return 0;
	}

	std::wstring strName = szName;
	MBStringUtil::ToLower( strName );

	UINT64 TextureID = FindTextureID( strName.c_str() );
	if( TextureID != 0 ) {
		return TextureID;
	}

	F3dCTexturePtr pTexture;
	{
#ifdef _RECYCLE_TEXTURE
		 
		auto it = RecycleTexture_.find( strName );
		if( it != RecycleTexture_.end() ) {
			pTexture = it->second;
			pTexture->SetRef( 0 );
			RecycleTexture_.erase( strName );
//			LOGOUT( L"RecycleTexture_.size() = %d", RecycleTexture_.size() );
		}
#endif
	}

	HRESULT hResult = S_OK;
	if( pTexture == NULL ) {
//		if( bDetail ) {
			hResult = F3dCTexture::LoaderDetail( szName, pTexture, bDetail );
//		}
//		else {
//			hResult = F3dCTexture::Loader( szName, pTexture );
//		}
		if( FAILED( hResult ) ) {
			LOGOUT( L"+++file not found.-TextureManager %s", szName );
			return 0;
		}
	}
	return InsertTexture( pTexture );
}

UINT64 F3dCTextureManager::CreateCubeTexture( const WCHAR* szName, bool bDetail )
{
	if( Disable_ == true )
		return 0;

	UINT64 TextureID = FindCubeTextureID( szName );
	if( TextureID != 0 ) {
		return TextureID;
	}

	F3dCTexturePtr pTexture;
	HRESULT hResult = F3dCTexture::LoaderCube( szName, pTexture );
	if( FAILED( hResult ) ) {
		return 0;
	}

	return InsertTexture( pTexture );
}

UINT64 F3dCTextureManager::CreateTextureRenderTarget( const WCHAR* pName, int nWidth, int nHeight, D3DFORMAT Format )
{
	if( Disable_ == true )
		return 0;

	UINT64 TextureID = FindTextureRenderTargetID( pName );
	if( TextureID != 0 ) {
		return TextureID;
	}

	F3dCTexturePtr pNull;
	F3dCTexturePtr pTexture;
	HRESULT hResult = F3dCTexture::CreateRenderTarget( nWidth, nHeight, Format, pTexture );
	if( FAILED( hResult ) ) {
		return 0;
	}
	return InsertTexture( pTexture );
}

UINT64 F3dCTextureManager::CreateTexture( int width, int height, D3DFORMAT Format, D3DPOOL pool /*= D3DPOOL_MANAGED*/, int nLevel /*= 1*/ )
{
	if( Disable_ == true )
		return 0;

	F3dCTexturePtr pNull;
	F3dCTexturePtr pTexture;
	HRESULT hResult = F3dCTexture::CreateTexture( width, height, Format, pool , nLevel, pTexture );
	if( FAILED( hResult ) ) {
		return 0;
	}
	return InsertTexture( pTexture );
}

F3dCTexture* F3dCTextureManager::FindTexture( UINT64 TextureID )
{
	 

	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( TextureID );
	if( it == Texture_.end() ) {
//		if( F3dCDevice::GetSingleton()->GetD3DDevice()->GetMainThreadID() == GetCurrentThreadId() ) {
//			LOGOUT( L"Main Thread - FindTexturePtr = NULL" );
//		}
		return NULL;
	}

//	if( F3dCDevice::GetSingleton()->GetD3DDevice()->GetMainThreadID() == GetCurrentThreadId() ) {
//		LOGOUT( L"Main Thread - FindTexturePtr = %s", it->second->GetName() );
//	}

	return it->second.get();
}

F3dCTexturePtr F3dCTextureManager::FindTexturePtr( UINT64 TextureID )
{
	 

	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( TextureID );
	if( it == Texture_.end() ) {
//		if( F3dCDevice::GetSingleton()->GetD3DDevice()->GetMainThreadID() == GetCurrentThreadId() ) {
//			LOGOUT( L"Main Thread - FindTexturePtr = NULL" );
//		}

		F3dCTexturePtr pTexture;
		return pTexture;
	}

//	if( F3dCDevice::GetSingleton()->GetD3DDevice()->GetMainThreadID() == GetCurrentThreadId() ) {
//		LOGOUT( L"Main Thread - FindTexturePtr = %s", it->second->GetName() );
//	}

	return it->second;
}

F3dCTexture* F3dCTextureManager::FindTexture( const WCHAR* szName )
{
	 
	UINT64 ID = 0;
	auto it = TextureID_.find( szName );
	if( it != TextureID_.end() ) {
		ID = it->second;
	}
	else if( ID == 0 ) {
		it = CubeTextureID_.find( szName );
		if( it != CubeTextureID_.end() ) {
			ID = it->second;
		}
	}
	else if( ID == 0 )  {
		it = RenderTargetTextureID_.find( szName );
		if( it != RenderTargetTextureID_.end() ) {
			ID = it->second;
		}
	}

	if( ID == 0 )
		return nullptr;

	auto it2 = Texture_.find( ID );
	if( it2 != Texture_.end() )
		return it2->second.get();
	return nullptr;
}

UINT64 F3dCTextureManager::FindTextureID( const WCHAR* szName )
{
	std::wstring strName = szName;
	MBStringUtil::ToLower( strName );
	auto it = TextureID_.find( strName.c_str() );
	if( it == TextureID_.end() )
		return 0;

	auto it2 = Texture_.find( it->second );
	if( it2 == Texture_.end() ) {
		return 0;
	}

	it2->second->AddRef();
	return it->second;
}

UINT64 F3dCTextureManager::FindCubeTextureID( const WCHAR* szName )
{
	std::wstring strName = szName;
	MBStringUtil::ToLower( strName );
	auto it = CubeTextureID_.find( strName.c_str() );
	if( it == CubeTextureID_.end() )
		return 0;

	auto it2 = Texture_.find( it->second );
	if( it2 == Texture_.end() ) {
		return 0;
	}

	it2->second->AddRef();
	return it->second;
}

UINT64 F3dCTextureManager::FindTextureRenderTargetID( const WCHAR* szName )
{
	std::wstring strName = szName;
	MBStringUtil::ToLower( strName );
	auto it = RenderTargetTextureID_.find( strName.c_str() );
	if( it == RenderTargetTextureID_.end() )
		return 0;

	auto it2 = Texture_.find( it->second );
	if( it2 == Texture_.end() ) {
		return 0;
	}

	it2->second->AddRef();
	return it->second;
}

UINT64 F3dCTextureManager::InsertTexture( const F3dCTexturePtr& pTexture )
{
	UINT64 TextureID = GenerateTextureID();

	if( !pTexture )
		return TextureID;

	pTexture->AddRef();
	Texture_.insert( std::make_pair( TextureID, pTexture ) );

	if( pTexture->IsRenderTarget() ) {
		RenderTargetTextureID_.insert( std::make_pair( pTexture->GetName(), TextureID ) );
	}
	else if( pTexture->IsCubeMap() ) {
		CubeTextureID_.insert( std::make_pair( pTexture->GetName(), TextureID ) );
	}
	else {
		TextureID_.insert( std::make_pair( pTexture->GetName(), TextureID ) );
	}

	return TextureID;
}

HRESULT F3dCTextureManager::LostDevice()
{
	//LOGOUT( L"LostDevice= "__FUNCTIONW__ );

	 

	HRESULT hResult = S_OK;
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		hResult = it->second->LostDevice();
		if( hResult != S_OK ) {
			return hResult;
		}
	}
	return hResult;
}

HRESULT F3dCTextureManager::ResetDevice()
{
	HRESULT hResult = S_OK;
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		hResult = it->second->ResetDevice();
		if( hResult != S_OK ) {
			return hResult;
		}
	}

	return hResult;
}

bool F3dCTextureManager::LostDeviceVerify()
{
	 

	bool bResult = true;
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		bResult = it->second->LostDeviceVerify();
		if( bResult == false ) {
			LOGOUT( L"LostDeviceVerify=false "__FUNCTIONW__ );
			return bResult;
		}
	}
	return bResult;
}

void F3dCTextureManager::SetDetail( float fDetail )
{
	if( Detail_ == fDetail ) return;
	Detail_ = std::min( std::max( fDetail, 0.1f ), 1.f );
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		if( it->second->IsDetail() == false )
			continue;
		if( it->second->IsRenderTarget() == true )
			continue;

		std::wstring strName = it->second->GetName();
		int nAddRef = it->second->GetRef();
		F3dCTexturePtr pTexture;
		F3dCTexture::LoaderDetail( strName.c_str(), pTexture, true );
		pTexture->SetRef( nAddRef );
		it->second = pTexture;
	}
}

UINT64 F3dCTextureManager::GenerateTextureID()
{
	return LastTextureID_++;
}

void F3dCTextureManager::GetSquareSize( const int nX, const int nY, int & nOutX, int & nOutY )
{
	int   nExpX, nExpY;
	float fExpX, fExpY;
	float fLog2 = logf( 2.0f );
	fExpX = logf( (float)nX - 1 ) / fLog2;
	fExpY = logf( (float)nY - 1 ) / fLog2;
	nExpX = ( int ) fExpX + 1;
	nExpY = ( int ) fExpY + 1;

	nOutX = 1 << nExpX;
	nOutY = 1 << nExpY;
}


void F3dCTextureManager::CmpSave()
{
	CmpTexture_.clear();

	MBHashMap< UINT64, F3dCTexturePtr >::const_iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		CmpTexture_.push_back( it->second->GetName() );
	}
}

void F3dCTextureManager::CmpCheck()
{
	MBHashMap< UINT64, F3dCTexturePtr >::const_iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		std::vector< std::wstring >::iterator CmpIT = std::find( CmpTexture_.begin(), CmpTexture_.end(), it->second->GetName() );
		if( CmpIT != CmpTexture_.end() ) {
			assert( false );
		}
	}
}

void F3dCTextureManager::StartRender()
{
	Texture4Render_ = Texture_;
}

F3dCTexture* F3dCTextureManager::FindTexture_Render( UINT64 TextureID )
{
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture4Render_.find( TextureID );
	if( it == Texture4Render_.end() )
		return NULL;

	return it->second.get();
}

F3dCTexturePtr F3dCTextureManager::FindTexturePtr_Render( UINT64 TextureID )
{
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture4Render_.find( TextureID );
	if( it == Texture4Render_.end() ) {
		F3dCTexturePtr pTexture;
		return pTexture;
	}

	return it->second;
}

void F3dCTextureManager::BreakupRecycleTexture()
{
#ifndef _RECYCLE_TEXTURE
	return;
#endif
	if( RecycleTexture_.size() < 300 )
		return;

	 
	std::vector< SReleaseTexture > vReleaseTexture;
	vReleaseTexture.reserve( RecycleTexture_.size() );
	for( auto it = RecycleTexture_.begin(); it != RecycleTexture_.end(); ++it )
	{
		SReleaseTexture rt;
		rt.strName = it->second->GetName();
		rt.dReleaseTime = it->second->GetReleaseTime();
		vReleaseTexture.push_back( rt );
	}

	std::sort( vReleaseTexture.begin(), vReleaseTexture.end(), []( const SReleaseTexture& a, const SReleaseTexture& b ) -> bool {
		if( a.dReleaseTime < b.dReleaseTime )
			return true;
		return false;
	});

	int nCount = vReleaseTexture.size() - 300;
	for( int i = 0; i < nCount; ++i )
	{
		RecycleTexture_.erase( vReleaseTexture[i].strName );
	}
//	LOGOUT( L"RecycleTexture_.size() = %d", RecycleTexture_.size() );
}

void F3dCTextureManager::GetTextureAll( std::vector< F3dCTexturePtr >& vTexture )
{
	for each( auto& p in Texture_ )
	{
		vTexture.push_back( p.second );
	}
}

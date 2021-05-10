#include "FeatherXEnginePrecompile.h"

F3dCTextTextureManager::F3dCTextTextureManager() :
	LastTextureID_(0)
{
}

F3dCTextTextureManager::~F3dCTextTextureManager()
{
}

HRESULT	F3dCTextTextureManager::Init()
{
	CreateTexture(L"null.dds", false );
	return S_OK;
}

void F3dCTextTextureManager::Finish()
{
	// delete null.dds
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( 0 );
	if( it != Texture_.end() ) {
		Texture_.erase( it );
	}

	assert( Texture_.empty() );
	it = Texture_.begin();
	for(; it != Texture_.end(); ++it ) {
		LOGOUT( L"AllocTexture %s", it->second->GetName() );
	}
	Texture_.clear();
}

void F3dCTextTextureManager::Release( UINT64 TextureID )
{
	 

	if( TextureID == 0 )
		return;

	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( TextureID );
	if( it == Texture_.end() )
		return;

	if( it->second.get() ) {
		int nRef = it->second->Release();
		if( nRef > 0 ) {
			return;
		}
		Texture_.erase( TextureID );
	}
}

UINT64 F3dCTextTextureManager::CreateTexture( const WCHAR* szName, bool bDetail )
{
	if( wcslen(szName) < 2 ) //파일 이름 이상
		return 0;

	if( NULL == szName  ) {
		assert( false );
		return 0;
	}

	UINT64 TextureID = FindTextureID( szName );
	if( TextureID != 0 ) {
		return TextureID;
	}

	F3dCTexturePtr pTexture;
	HRESULT hResult = S_OK;
//	if( bDetail ) {
		hResult = F3dCTexture::LoaderDetail( szName, pTexture, bDetail );
//	}
//	else {
//		hResult = F3dCTexture::Loader( szName, pTexture );
//	}
	if( FAILED( hResult ) ) {
		return 0;
	}

	TextureID = GenerateTextureID();
	InsertTexture( TextureID, pTexture );

	return TextureID;
}

UINT64 F3dCTextTextureManager::CreateTexture( int width, int height, D3DFORMAT Format, D3DPOOL pool /*= D3DPOOL_MANAGED*/, int nLevel /*= 1*/ )
{
	F3dCTexturePtr pNull;
	F3dCTexturePtr pTexture;
	HRESULT hResult = F3dCTexture::CreateTexture( width, height, Format, pool , nLevel, pTexture );
	if( FAILED( hResult ) ) {
		return 0;
	}
	UINT64 TextureID = GenerateTextureID();
	InsertTexture( TextureID, pTexture );
	return TextureID;
}

F3dCTexture* F3dCTextTextureManager::FindTexture( UINT64 TextureID )
{
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.find( TextureID );
	if( it == Texture_.end() )
		return NULL;

	return it->second.get();
}

F3dCTexture* F3dCTextTextureManager::FindTexture( const WCHAR* pName )
{
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		if( _wcsicmp( pName, it->second->GetName())) {
			return it->second.get();
		}
	}

	return 0;
}

HRESULT F3dCTextTextureManager::LostDevice()
{
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

HRESULT F3dCTextTextureManager::ResetDevice()
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

bool F3dCTextTextureManager::LostDeviceVerify()
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

const UINT F3dCTextTextureManager::GetTextureCount()
{
	return (UINT)Texture_.size();
}

UINT64 F3dCTextTextureManager::GenerateTextureID()
{
	return LastTextureID_++;
}

UINT64 F3dCTextTextureManager::FindTextureID( const WCHAR* pName )
{
	MBHashMap< UINT64, F3dCTexturePtr >::iterator it = Texture_.begin();
	for(; it != Texture_.end(); ++it )
	{
		if( it->second->IsRenderTarget() )
			continue;

		if( it->second->IsCubeMap() )
			continue;

		if( _wcsicmp( pName, it->second->GetName() ) == 0 ) {
			it->second->AddRef();
			return it->first;
		}
	}

	return 0;
}

void F3dCTextTextureManager::InsertTexture( UINT64 TextureID, const F3dCTexturePtr& pTexture )
{
	pTexture->AddRef();
	Texture_.insert( std::make_pair( TextureID, pTexture ) );
}

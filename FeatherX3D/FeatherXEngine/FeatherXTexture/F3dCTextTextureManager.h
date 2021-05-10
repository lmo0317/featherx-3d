#pragma once

class F3dCTextTextureManager : public FBSingleton< F3dCTextTextureManager >
{
public:
	F3dCTextTextureManager();
	~F3dCTextTextureManager();

	HRESULT				Init();
	void				Finish();

	void				Release( UINT64 TextureID );
	UINT64				CreateTexture( const WCHAR* pName, bool bNull = false );
	UINT64				CreateTexture( int width, int height, D3DFORMAT Format, D3DPOOL pool = D3DPOOL_MANAGED, int nLevel = 1 );

	F3dCTexture*		FindTexture( UINT64 TextureID );
	F3dCTexture*		FindTexture( const WCHAR* pName );

	HRESULT				LostDevice();
	HRESULT				ResetDevice();
	bool				LostDeviceVerify();

	const UINT			GetTextureCount();

private:
	UINT64				GenerateTextureID();
	UINT64				FindTextureID( const WCHAR* pName );
	void				InsertTexture( UINT64 TextureID, const F3dCTexturePtr& pTexture );

	MBHashMap< UINT64, F3dCTexturePtr > Texture_;
	UINT64				LastTextureID_;
};

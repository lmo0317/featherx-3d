#pragma once

class F3dCTextureManager : public FBSingleton< F3dCTextureManager >
{
public:
	F3dCTextureManager();
	~F3dCTextureManager();

	HRESULT				Init();
	void				Finish();
	void				Disable();

	void				Release( UINT64 TextureID );
	UINT64				CreateTexture( const WCHAR* pName, bool bDetail );
	UINT64				CreateCubeTexture( const WCHAR* pName, bool bDetail );
	UINT64				CreateTextureRenderTarget( const WCHAR* pName, int nWidth, int nHeight, D3DFORMAT Format );
	UINT64				CreateTexture( int width, int height, D3DFORMAT Format, D3DPOOL pool = D3DPOOL_MANAGED, int nLevel = 1 );

	F3dCTexture*		FindTexture( UINT64 TextureID );
	F3dCTexturePtr		FindTexturePtr( UINT64 TextureID );
	F3dCTexture*		FindTexture( const WCHAR* szName );

	F3dCTexture*		FindTexture_Render( UINT64 TextureID );
	F3dCTexturePtr		FindTexturePtr_Render( UINT64 TextureID );

	HRESULT				LostDevice();
	HRESULT				ResetDevice();
	bool				LostDeviceVerify();

	const UINT			GetTextureCount();
	void				SetDetail( float fDetail );
	float				GetDetail() const			{ return Detail_; }

	static void			GetSquareSize( const int nX, const int nY, int & nOutX, int & nOutY );

	void				CmpSave();
	void				CmpCheck();

	void				StartRender();
	void				BreakupRecycleTexture();

	void				GetTextureAll( std::vector< F3dCTexturePtr >& vTexture );

private:
	struct SReleaseTexture
	{
		double dReleaseTime;
		std::wstring strName;
	};

	UINT64				FindTextureID( const WCHAR* pName );
	UINT64				FindCubeTextureID( const WCHAR* pName );
	UINT64				FindTextureRenderTargetID( const WCHAR* pName );
	UINT64				InsertTexture( const F3dCTexturePtr& pTexture );

	UINT64				GenerateTextureID();
	boost::unordered_map< std::wstring, UINT64 > TextureID_;
	boost::unordered_map< std::wstring, UINT64 > CubeTextureID_;
	boost::unordered_map< std::wstring, UINT64 > RenderTargetTextureID_;
	boost::unordered_map< std::wstring, F3dCTexturePtr > RecycleTexture_;

	MBHashMap< UINT64, F3dCTexturePtr > Texture_;
	MBHashMap< UINT64, F3dCTexturePtr > Texture4Render_;
	float Detail_;
	UINT64 LastTextureID_;
	bool Disable_;
	std::vector< std::wstring > CmpTexture_;
};

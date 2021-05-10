#include "FeatherXEnginePrecompile.h"
#include "MBStringUtill.h"

using namespace MBStringUtil;

//////////////////////////////////////////////////////////////////////////
F3dCUISprite::~F3dCUISprite()
{ 
	 
	if( !bSkipRelease ) {
		if( bText )
			F3dCTextTextureManager::GetSingleton()->Release( TextureID );
		else
			F3dCTextureManager::GetSingleton()->Release( TextureID );
	}
}

//////////////////////////////////////////////////////////////////////////
F3dZUITexture::~F3dZUITexture()
{  
	F3dCTextureManager::GetSingleton()->Release( TextureID );
}

//////////////////////////////////////////////////////////////////////////
F3dZUISpriteManager::F3dZUISpriteManager()
{  

}

F3dZUISpriteManager::~F3dZUISpriteManager()
{  

}

void F3dZUISpriteManager::Init()
{  

}

void F3dZUISpriteManager::Finish()
{  
	{
		boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.begin();
		for(; it != m_vSpriteHash.end(); ++it )
		{
			F3dZUISprite* pSprite = it->second;
			delete pSprite;
		}

		m_vSpriteHash.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	{
		boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vDynamicSpriteHash.begin();
		for(; it != m_vDynamicSpriteHash.end(); ++it )
		{
			F3dZUISprite* pSprite = it->second;
			delete pSprite;
		}

		m_vDynamicSpriteHash.clear();
	}
}

bool F3dZUISpriteManager::Load( SPRITE_ARG& sprite_arg, bool bTextureLoad/*=false*/ )
{ 
	 
	int nSpriteCount = (int)sprite_arg.vAniList.size();
	for( int i(0); nSpriteCount>i; ++i )
	{
		MBStringUtil::ToLower( sprite_arg.vAniList[i].strName );
		F3dZUISprite* pSprite = NULL;
		try
		{
			pSprite = new F3dZUISprite;
			pSprite->strType   = sprite_arg.vAniList[i].strType;
			pSprite->strName   = sprite_arg.vAniList[i].strName;
			pSprite->nSetCount = sprite_arg.vAniList[i].nSetCount;
			pSprite->vTexList  = new std::vector<F3dZUITexture*>[pSprite->nSetCount]; //Texture Set
			pSprite->bLoaded = bTextureLoad;

			int nSetCnt  = pSprite->nSetCount;
			int nTextCnt = (int)sprite_arg.vAniList[i].vTextList.size();
			int nLoopCnt = nTextCnt/nSetCnt;
			int nTexLoop = 0;
		
			for( int k(0); nSetCnt>k; ++k )
			{
				for( int j(0); nLoopCnt>j; ++j )
				{
					F3dZUITexture* pText = new F3dZUITexture;
					pText->strName = sprite_arg.vAniList[i].vTextList[nTexLoop].strName;

					Replace( pText->strName, L"/", L"\\" );
				
					pText->nWidth  = sprite_arg.vAniList[i].vTextList[nTexLoop].nWidth;
					pText->nHeight = sprite_arg.vAniList[i].vTextList[nTexLoop].nHeight;

					//TODO : UITextureManager 로 수정 예정
					if( bTextureLoad ) {
						pText->TextureID = F3dCTextureManager::GetSingleton()->CreateTexture( pText->strName.c_str(), false );
					}

					pSprite->vTexList[k].push_back( pText );
					++nTexLoop;
				}
			}

	#ifdef _DEBUG
			boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.find( sprite_arg.vAniList[i].strName.c_str() );
			if( it != m_vSpriteHash.end() )
			{
				assert( 0 && "Used Duplicate Sprite Name" );
			}
	#endif

			m_vSpriteHash.insert( std::make_pair( sprite_arg.vAniList[i].strName, pSprite ) );
		}
		catch (std::bad_alloc &ba)
		{
			std::cout << ba.what() << std::endl;
			if (NULL != pSprite)
				delete pSprite;
		}
	}

	return false;
}

void F3dZUISpriteManager::DelSprite( const WCHAR* pAniName )
{  
	boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.find( pAniName );
	if( it != m_vSpriteHash.end() )
	{
		F3dZUISprite* pSprite = it->second;
		delete pSprite;

		m_vSpriteHash.erase( it );
		return;
	}
}

void F3dZUISpriteManager::AddSprite( F3dZUISprite* pNewSpr )
{  
	boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.find( pNewSpr->strName.c_str() );
	if( it != m_vSpriteHash.end() )
	{
		return;
	}

	MBStringUtil::ToLower( pNewSpr->strName );

	m_vSpriteHash.insert( std::make_pair( pNewSpr->strName.c_str(), pNewSpr ) );
}

void F3dZUISpriteManager::GetSpriteList( std::vector<const F3dZUISprite*>& sprList )
{  
	boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.begin();
	for(; it != m_vSpriteHash.end(); ++it )
	{
		F3dZUISprite* pSprite = it->second;
		sprList.push_back( pSprite );
	}
}

const F3dZUISprite* F3dZUISpriteManager::GetSpriteInfo( const WCHAR* pAniName )
{  
	if( pAniName == NULL  ) {
		//		_MBASSERT(0 && "NULL AniName" );
		return NULL;
	}

	std::wstring strAniName = pAniName;
	MBStringUtil::ToLower( strAniName );
	
	boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.find( strAniName.c_str() );

	if( it != m_vSpriteHash.end() )
	{
		F3dZUISprite* pSprite = it->second;
		//		//Ref Count 증가를 위해 무조건 검색
		{
			pSprite->bLoaded = true;
			for( int j(0); pSprite->nSetCount>j; ++j )
			{
				UINT nTextCnt = (UINT)pSprite->vTexList[j].size();
				for( UINT i(0); nTextCnt>i; ++i ) //Texture Loading
				{
					F3dZUITexture* pUITex = pSprite->vTexList[j][i];
				}
			}
		}

		return pSprite;
	}

	if( wcslen( pAniName ) > 0 ) {
		LOGOUT( L"Not Found Sprite! %s\n", pAniName );
		//#ifdef _RUSTYHEARTS_GAME_
		//		return GetSprite( L"empty_slot" );
		//#endif
		//		_MBASSERT(0 && "Not Found Sprite!" );
	}

	return NULL;
}

const F3dZUISprite* F3dZUISpriteManager::GetSprite( const WCHAR* pAniName )
{
	 
	if( pAniName == NULL  ) {
//		_MBASSERT(0 && "NULL AniName" );
		return NULL;
	}

	std::wstring strAniName = pAniName;
	MBStringUtil::ToLower( strAniName );

	boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vSpriteHash.find( strAniName.c_str() );

	if( it != m_vSpriteHash.end() )
	{
		F3dZUISprite* pSprite = it->second;
//		//Ref Count 증가를 위해 무조건 검색
		{
			pSprite->bLoaded = true;
			for( int j(0); pSprite->nSetCount>j; ++j )
			{
				UINT nTextCnt = (UINT)pSprite->vTexList[j].size();
				for( UINT i(0); nTextCnt>i; ++i ) //Texture Loading
				{
					F3dZUITexture* pUITex = pSprite->vTexList[j][i];
					pUITex->TextureID = F3dCTextureManager::GetSingleton()->CreateTexture( pUITex->strName.c_str(), false );
				}
			}
		}

		return pSprite;
	}

	if( wcslen( pAniName ) > 0 ) {
		LOGOUT( L"Not Found Sprite! %s\n", pAniName );
//#ifdef _RUSTYHEARTS_GAME_
//		return GetSprite( L"empty_slot" );
//#endif
//		_MBASSERT(0 && "Not Found Sprite!" );
	}

	return NULL;
}

//Guild Icon 등, User Contents 용
const F3dZUISprite* F3dZUISpriteManager::GetDynamicSprite( const WCHAR* pFullPath, const WCHAR* pFileName )
{  
	boost::unordered_map< std::wstring, F3dZUISprite* >::iterator it = m_vDynamicSpriteHash.find( pFileName );

	if( it != m_vDynamicSpriteHash.end() )
	{
		F3dZUISprite* pSprite = it->second;
		{
			pSprite->bLoaded = true;
			for( int j(0); pSprite->nSetCount>j; ++j )
			{
				UINT nTextCnt = (UINT)pSprite->vTexList[j].size();
				for( UINT i(0); nTextCnt>i; ++i ) //Texture Loading
				{
					F3dZUITexture* pUITex = pSprite->vTexList[j][i];
					pUITex->TextureID = F3dCTextureManager::GetSingleton()->CreateTexture( pUITex->strName.c_str(), false );
				}
			}
		}

		return pSprite;
	}
	
	//없으니 새거 추가
	AddDynamicSprite( pFullPath, pFileName );

	return GetDynamicSprite( pFullPath, pFileName );
}

void F3dZUISpriteManager::AddDynamicSprite( const WCHAR* pFullPath, const WCHAR* pFileName )
{  
	static int s_DynamicSpriteCount = 1;

	int nSpriteCount = 1;
	for( int i(0); nSpriteCount>i; ++i )
	{
		F3dZUISprite* pSprite = NULL;

		try
		{
			pSprite = new F3dZUISprite;
			pSprite->strType   = L"static";
			pSprite->strName   = MBStringUtil::StringFormat( L"dynamic_icon_%d", s_DynamicSpriteCount++ ).c_str();
			pSprite->nSetCount = 1;
			pSprite->vTexList  = new std::vector<F3dZUITexture*>[pSprite->nSetCount]; //Texture Set
			pSprite->bLoaded   = true;

			int nSetCnt  = pSprite->nSetCount;
			int nTextCnt = (int)1;
			int nLoopCnt = nTextCnt/nSetCnt;
			int nTexLoop = 0;

			for( int k(0); nSetCnt>k; ++k )
			{
				for( int j(0); nLoopCnt>j; ++j )
				{
					F3dZUITexture* pText = new F3dZUITexture;
					pText->strName = pFullPath;

					pText->nWidth  = 32;
					pText->nHeight = 32;

					pSprite->vTexList[k].push_back( pText );
					++nTexLoop;
				}
			}

			m_vDynamicSpriteHash.insert( std::make_pair( pFileName, pSprite ) );
		}
		catch (std::bad_alloc &ba)
		{
			std::cout << ba.what() << std::endl;
			if (NULL != pSprite)
				delete pSprite;
		}
	}
}
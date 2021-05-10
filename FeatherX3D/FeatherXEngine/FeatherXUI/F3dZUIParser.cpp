#include "FeatherXEnginePrecompile.h"

#include <algorithm>
#include <cctype>       // std::tolower

//#ifdef _PACKED_FILE_
#include "MagiFileSystem.h"
#include "MagiStream.h"
//#endif

using namespace MBStringUtil;

F3dZUIParser::F3dZUIParser()
{  
}

F3dZUIParser::~F3dZUIParser()
{  
}

HRESULT F3dZUIParser::Init()
{  
	F3dZUISpriteManager::CreateSingleton();
	F3dZUISpriteManager::GetSingleton()->Init();

	DoParseSprite( L"ui\\defaultsprite.xml" );

	return S_OK;
}

void F3dZUIParser::Finish()
{  
	F3dZUISpriteManager::GetSingleton()->Finish();
	F3dZUISpriteManager::DestroySingleton();
}

void F3dZUIParser::Reload()
{  
	F3dZUISpriteManager::GetSingleton()->Finish();

	DoParseSprite( L"ui\\defaultsprite.xml" );
}

void F3dZUIParser::_doParseWindow( const TiXmlElement* pElemWindow, WINDOW_ARG& window_arg )
{  
	wchar_t szName[256] = {0,};
	const char* pWinID      = pElemWindow->Attribute("id");
	const char* pWinStyle   = pElemWindow->Attribute("style");
	const char* pWinCaption = pElemWindow->Attribute("caption");
//	MBCLog::GetSingleton()->DebugOutputA("Window - %s %s %s\n",pWinID, pWinStyle, pWinCaption);

	assert( pWinID      );
	assert( pWinStyle   );

	if( pWinID      ) 	{
		AC2WC( szName, 256, pWinID, (int)strlen(pWinID)+1 );
		window_arg.window_info.ID         = szName;
	}
	if( pWinStyle   ) 	{
		AC2WC( szName, 256, pWinStyle, (int)strlen(pWinStyle)+1 );
		window_arg.window_info.strStyle   = szName;
	}

	if( pWinCaption ) window_arg.window_info.nCaptionID = atoi(pWinCaption);

	for ( pElemWindow = pElemWindow->FirstChildElement(); pElemWindow != NULL; pElemWindow = pElemWindow->NextSiblingElement() )
	{
		const char* pValue = pElemWindow->Value();
		if( !_stricmp( pValue, "pos" ) )		{
			const char* pAttrX = pElemWindow->Attribute( "x" ); 
			const char* pAttrY = pElemWindow->Attribute( "y" ); 
//			MBCLog::GetSingleton()->DebugOutputA("pos - %s %s\n",pAttrX, pAttrY);

			if( pAttrX ) window_arg.window_info.x = atoi(pAttrX);
			if( pAttrY ) window_arg.window_info.y = atoi(pAttrY);
		}
		else if( !_stricmp( pValue, "size" ) )		{
			const char* pAttrWidth  = pElemWindow->Attribute( "width"  );   
			const char* pAttrHeight = pElemWindow->Attribute( "height" ); 
//			MBCLog::GetSingleton()->DebugOutputA("size - %s %s\n",pAttrWidth, pAttrHeight); 

			if( pAttrWidth  ) window_arg.window_info.nWidth  = atoi(pAttrWidth );
			if( pAttrHeight ) window_arg.window_info.nHeight = atoi(pAttrHeight);
		}
		else if( !_stricmp( pValue, "control" ) )		{
			CONTROL_ARG control_arg;
			const char* pAttrId       = pElemWindow->Attribute( "id"      ); 
			const char* pAttrCaption  = pElemWindow->Attribute( "caption" );
			const char* pAttrTooltip  = pElemWindow->Attribute( "tooltip" );
			const char* pAttrAni      = pElemWindow->Attribute( "ani"     ); 
			const char* pAttrStyle    = pElemWindow->Attribute( "style"   ); 
			const char* pAttrfontName = pElemWindow->Attribute( "font_name" ); 
			const char* pAttrfontSize = pElemWindow->Attribute( "font_size" ); 
			const char* pAttrfontFlag = pElemWindow->Attribute( "font_flag" ); 
			const char* pAttrX        = pElemWindow->Attribute( "x" );     
			const char* pAttrY        = pElemWindow->Attribute( "y" );     
			const char* pAttrWidth    = pElemWindow->Attribute( "width"  );  
			const char* pAttrHeight   = pElemWindow->Attribute( "height" ); 
			const char* pAttrColor    = pElemWindow->Attribute( "color" ); 
			const char* pAttrColorBG  = pElemWindow->Attribute( "bg_color" ); 

//			MBCLog::GetSingleton()->DebugOutputA("control - %s %s %s %s %s %s %s %s %s %s %s\n",pAttrId, pAttrCaption, pAttrAni, pAttrStyle, pAttrfontName, pAttrfontSize, pAttrfontFlag, pAttrX, pAttrY, pAttrWidth, pAttrHeight); 

			assert(pAttrId);
			assert(pAttrWidth);
			assert(pAttrHeight);

			if( pAttrId      ) 			{
				AC2WC( szName, 256, pAttrId, (int)strlen(pAttrId)+1 );
				control_arg.ID       = szName;
			}
			if( pAttrAni     ) 			{
				AC2WC( szName, 256, pAttrAni, (int)strlen(pAttrAni)+1 );
				control_arg.strAni   = szName;
			}
			if( pAttrStyle   ) 			{
				AC2WC( szName, 256, pAttrStyle, (int)strlen(pAttrStyle)+1 );
				control_arg.strStyle = szName;
			}

			if( pAttrfontName   ) 			{
				AC2WC( szName, 256, pAttrfontName, (int)strlen(pAttrfontName)+1 );
				control_arg.strFontName = szName;
			}
			else
				control_arg.strFontName = L"dotum";

			if( pAttrfontFlag   ) 			{
				AC2WC( szName, 256, pAttrfontFlag, (int)strlen(pAttrfontFlag)+1 );
				control_arg.strFontFlag  = szName;
			}

			if( pAttrfontSize ) 
				control_arg.nFontSize  = atoi(pAttrfontSize);
			else
				control_arg.nFontSize  = 10;

			if( pAttrCaption  ) control_arg.nCaptionID = atoi(pAttrCaption);
			if( pAttrTooltip  ) control_arg.nToolTipID = atoi(pAttrTooltip);
			if( pAttrX        ) control_arg.x          = atoi(pAttrX);
			if( pAttrY        ) control_arg.y          = atoi(pAttrY);
			if( pAttrWidth    ) control_arg.nWidth     = atoi(pAttrWidth );
			if( pAttrHeight   ) control_arg.nHeight    = atoi(pAttrHeight);

			char *HEX_P;
			if( pAttrColor    ) {
				control_arg.dwTextColor   = strtoul( pAttrColor, &HEX_P, 16 );
				control_arg.dwTextColor |= 0xFF000000;
			}
			else {
				control_arg.dwTextColor = 0xFFFFFFFF;
			}

			if( pAttrColorBG  ) {
				control_arg.dwTextColorBG = strtoul( pAttrColorBG, &HEX_P, 16);
				control_arg.dwTextColorBG |= 0xFF000000;
			}
			else {
				control_arg.dwTextColorBG = 0xFF000000;
			}

			window_arg.vControlList.push_back( control_arg );
		}
	}
}

void F3dZUIParser::_doParseSpriteTex( const TiXmlElement* pChildElem, ANI_ARG& ani_arg )
{  
	wchar_t szName[256] = {0,};
	for( ; pChildElem != NULL; pChildElem = pChildElem->NextSiblingElement() )
	{
		const char* pValue = pChildElem->Value();
		if( !_stricmp( pValue, "tex" ) )		{
			TEX_ARG tex_arg;

			const char* pAttrName   = pChildElem->Attribute( "name"  );   
			const char* pAttrWidth  = pChildElem->Attribute( "width"  );   
			const char* pAttrHeight = pChildElem->Attribute( "height" ); 
//			MBCLog::GetSingleton()->DebugOutputA("Tex - %s %s %s\n", pAttrName, pAttrWidth, pAttrHeight);

			assert(pAttrName  );
			assert(pAttrName  );
			assert(pAttrHeight);

			if( pAttrName   ) 			{
				AC2WC( szName, 256, pAttrName, (int)strlen(pAttrName)+1 );
				tex_arg.strName = szName;
			}
			if( pAttrName   ) tex_arg.nWidth  = atoi(pAttrWidth);
			if( pAttrHeight ) tex_arg.nHeight = atoi(pAttrHeight);

			std::transform(tex_arg.strName.begin() , tex_arg.strName.end() , tex_arg.strName.begin() , (int(*)(int)) std::tolower);

			ani_arg.vTextList.push_back( tex_arg );
		}
	}
}

void F3dZUIParser::_doParseSprite( const TiXmlElement* pElemSprite, SPRITE_ARG& sprite_arg )
{  
//	MBCLog::GetSingleton()->DebugOutputA("Sprite - \n");

	wchar_t szName[256] = {0,};

	for ( pElemSprite = pElemSprite->FirstChildElement(); pElemSprite != NULL; pElemSprite = pElemSprite->NextSiblingElement() )
	{
		const char* pValue = pElemSprite->Value();
		if( !_stricmp( pValue, "ani" ) )		{

			ANI_ARG ani_arg;
			const char* pAttrType = pElemSprite->Attribute( "type"  );   
			const char* pAttrName = pElemSprite->Attribute( "name"  ); 
			const char* pAttrSet  = pElemSprite->Attribute( "set"   );
//			MBCLog::GetSingleton()->DebugOutputA("Ani - %s %s\n", pAttrType, pAttrName);

			assert(pAttrType);
			assert(pAttrName);
			assert(pAttrSet );

			if( pAttrType ) 			{
				AC2WC( szName, 256, pAttrType, (int)strlen(pAttrType)+1 );
				ani_arg.strType = szName;
			}

			if( pAttrName ) 			{
				AC2WC( szName, 256, pAttrName, (int)strlen(pAttrName)+1 );
				ani_arg.strName = szName;
			}

			if( pAttrSet )
				ani_arg.nSetCount = atoi(pAttrSet);

			const TiXmlElement* pChildElem = pElemSprite->FirstChildElement("tex");
			_doParseSpriteTex( pChildElem, ani_arg );

			sprite_arg.vAniList.push_back( ani_arg );
		}
	}
}

BOOL F3dZUIParser::DoParseWindow( const wchar_t* pFileName, WINDOW_ARG& window_arg )
{  
	char file[_MAX_PATH] = {0,};
if( MagiFileSystem::IsPackedFile() ) {
	MagiStream* pStream = MagiPackageFileSystem::Instance()->GetStream( pFileName );
	if( !pStream ) return FALSE;

	MagiMemoryStream	memStream( (size_t)pStream->GetLength() );
	pStream->Read( memStream.GetBuf(), (size_t)pStream->GetLength() );
	pStream->DecRef();
	TiXmlDocument doc;
	doc.SetCondenseWhiteSpace( false );
	if ( !doc.LoadBuffer( memStream.GetBuf(), (int)memStream.GetLength() ) )
	{	
		_MBASSERT(0 && "TiXmlDocument Load Fail" );
		LOGOUT( L"F3dZUIParser::DoParseWindow - %s : ", pFileName );
		LOGOUTA( "%s\n", doc.ErrorDesc() );
		return FALSE;
	}

	const TiXmlElement* pElemWindow = doc.FirstChildElement("window");
	if( pElemWindow )	{
		_doParseWindow( pElemWindow, window_arg );
		return TRUE;
	}
}
else
{
	WC2AC( file, _MAX_PATH, pFileName, (int)wcslen(pFileName)+1 );

	TiXmlDocument doc( file );
	doc.SetCondenseWhiteSpace( false );

	if ( !doc.LoadFile() )
	{	
		_MBASSERT(0 && "TiXmlDocument Load Fail" );
		LOGOUT( L"F3dZUIParser::DoParseWindow - %s : ", pFileName );
		LOGOUTA( "%s\n", doc.ErrorDesc() );
		return FALSE;
	}

	const TiXmlElement* pElemWindow = doc.FirstChildElement("window");
	if( pElemWindow )	{
		_doParseWindow( pElemWindow, window_arg );
		return TRUE;
	}
}


	return FALSE;
}

BOOL F3dZUIParser::DoParseSprite( const wchar_t* pFileName )
{  
	char file[_MAX_PATH] = {0,};

if( MagiFileSystem::IsPackedFile() ) {
	MagiStream* pStream = MagiPackageFileSystem::Instance()->GetStream( pFileName );
	if( !pStream ) return FALSE;

	MagiMemoryStream	memStream( (size_t)pStream->GetLength() );
	pStream->Read( memStream.GetBuf(), (size_t)pStream->GetLength() );
	pStream->DecRef();
	TiXmlDocument doc;
	doc.SetCondenseWhiteSpace( false );
	if ( !doc.LoadBuffer( memStream.GetBuf(), (int)memStream.GetLength() ) )
	{
		_MBASSERT(0 && "TiXmlDocument Load Fail" );
		return FALSE;
	}

	const TiXmlElement* pElemSprite = doc.FirstChildElement("sprite");
	if( pElemSprite )	{
		SPRITE_ARG sprite_arg;
		_doParseSprite( pElemSprite, sprite_arg );
		F3dZUISpriteManager::GetSingleton()->Load( sprite_arg );
		return TRUE;
	}
}
else
{
	WC2AC( file, _MAX_PATH, pFileName      , (int)wcslen(pFileName)+1 );

	TiXmlDocument doc( file );

	doc.SetCondenseWhiteSpace( false );
	if ( !doc.LoadFile() )
	{
		_MBASSERT(0 && "TiXmlDocument Load Fail" );
		return FALSE;
	}

	const TiXmlElement* pElemSprite = doc.FirstChildElement("sprite");
	if( pElemSprite )	{
		SPRITE_ARG sprite_arg;
		_doParseSprite( pElemSprite, sprite_arg );
		F3dZUISpriteManager::GetSingleton()->Load( sprite_arg );
		return TRUE;
	}
}


	return FALSE;
}


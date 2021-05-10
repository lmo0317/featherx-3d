
#pragma once

class TiXmlElement;

class F3dZUIParser : public FBSingleton< F3dZUIParser >
{
public:

	enum
	{
		MODE_NONE,
		MODE_WINDOW,
		MODE_SPRITE,
	};

	F3dZUIParser();
	~F3dZUIParser();

	HRESULT Init();
	void    Finish();

	void    Reload();

	BOOL DoParseWindow( const wchar_t* pFileName, WINDOW_ARG& window_arg );
	BOOL DoParseSprite( const wchar_t* pFileName );

protected:

	void _doParseWindow( const TiXmlElement* pElem, WINDOW_ARG& window_arg );
	void _doParseSprite( const TiXmlElement* pElem, SPRITE_ARG& sprite_arg );
	void _doParseSpriteTex( const TiXmlElement* pElem, ANI_ARG& ani_arg );

private:

};
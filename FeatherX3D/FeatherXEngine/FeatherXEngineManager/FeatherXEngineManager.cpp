#include "FeatherXEnginePrecompile.h"

CFeatherXEngineManager::CFeatherXEngineManager()
{
}

CFeatherXEngineManager::~CFeatherXEngineManager()
{

}

void CFeatherXEngineManager::StartEngine(HWND hWnd)
{
	m_hWnd = hWnd;
	
	MBCLog::CreateSingleton();
	F3dCDevice::CreateSingleton();
	F3dCRenderMgr::CreateSingleton();
	CFeatherXLight::CreateSingleton();
	CFeatherXCamera::CreateSingleton();
	CFeatherXInput::CreateSingleton();
	CFeatherXModelManager::CreateSingleton();
	F3dCWaterManager::CreateSingleton();
	F3dCFxManager::CreateSingleton();
	F3dCTailManager::CreateSingleton();
	CFeatherXShaderManager::CreateSingleton();
	CFeatherXPostEffect::CreateSingleton();
	CFeatherXXmlParser::CreateSingleton();
	CFeatherXTextureManager::CreateSingleton();
	F3dCTextTextureManager::CreateSingleton();
	mpManager::CreateSingleton();

	F3dZUIParser::CreateSingleton();
	F3dCTextureManager::CreateSingleton();

	Init();
}

void CFeatherXEngineManager::Init()
{	 
	CFeatherXInput::GetSingleton()->Initialize(m_hWnd,(HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),false,IS_USEKEYBOARD|IS_USEMOUSE);
	if(!SUCCEEDED( F3dCDevice::GetSingleton()->InitD3D( m_hWnd )))
	{
		return;
	}

	F3dZUIText::InitTextRender();
	F3dCDevice::GetSingleton()->Init();
	F3dCRenderMgr::GetSingleton()->Init();
	CFeatherXLight::GetSingleton()->Init();
	CFeatherXCamera::GetSingleton()->Init();
	CFeatherXInput::GetSingleton()->Init();
	CFeatherXModelManager::GetSingleton()->Init();
	F3dCWaterManager::GetSingleton()->Init();
	F3dCFxManager::GetSingleton()->Init();
	F3dCTailManager::GetSingleton()->Init();
	CFeatherXShaderManager::GetSingleton()->Init();
	CFeatherXPostEffect::GetSingleton()->Init();
	CFeatherXXmlParser::GetSingleton()->Init();
	CFeatherXTextureManager::GetSingleton()->Init();
	F3dZUIParser::GetSingleton()->Init();
	F3dCTextureManager::GetSingleton()->Init();
	F3dCTextTextureManager::GetSingleton()->Init();

	F3dZUIText::AddFreeTypeFont( L"font"      , L"font.ttf"       );
	F3dZUIText::AddFreeTypeFont( L"font2"     , L"font2.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font_eng"  , L"font_eng.ttf"   );
	F3dZUIText::AddFreeTypeFont( L"font3"     , L"font3.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font4"     , L"font4.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font5"     , L"font5.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font6"     , L"font6.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font7"     , L"font7.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font8"     , L"font8.ttf"      );
	F3dZUIText::AddFreeTypeFont( L"font_magic", L"font_magic.ttf" );
	F3dZUIText::AddFreeTypeFont( L"font_nomi" , L"font_nomi.ttf"  );

	std::wstring strError;
	mpManager::GetSingleton()->Init(strError);
	MBCLog::GetSingleton()->Init("LOG\\output.txt");

	OpenConsoleWindow();
}

void CFeatherXEngineManager::Process(double dTime,double dt)
{
	mpManager::GetSingleton()->Process(dTime,dt);
	F3dCDevice::GetSingleton()->Process();
	F3dCRenderMgr::GetSingleton()->Process();
	CFeatherXLight::GetSingleton()->Process();
	CFeatherXCamera::GetSingleton()->Process();
	CFeatherXInput::GetSingleton()->Process();
	CFeatherXModelManager::GetSingleton()->Process();
	F3dCWaterManager::GetSingleton()->Process();
	F3dCFxManager::GetSingleton()->Process(dTime,dt);
	F3dCTailManager::GetSingleton()->Process(dTime,dt);
	CFeatherXShaderManager::GetSingleton()->Process();
	CFeatherXPostEffect::GetSingleton()->Process();
	CFeatherXXmlParser::GetSingleton()->Process();
	CFeatherXTextureManager::GetSingleton()->Process();
}

void CFeatherXEngineManager::Render(double dTime,double dt)
{	
	F3dCRenderMgr::GetSingleton()->Render(dTime,dt);
}

void CFeatherXEngineManager::Destroy()
{
	F3dCDevice::GetSingleton()->Cleanup();
	CFeatherXInput::GetSingleton()->Shutdown();

	F3dCDevice::GetSingleton()->Destroy();
	F3dCRenderMgr::GetSingleton()->Destroy();
	CFeatherXLight::GetSingleton()->Destroy();
	CFeatherXCamera::GetSingleton()->Destroy();
	CFeatherXInput::GetSingleton()->Destroy();
	CFeatherXModelManager::GetSingleton()->Destroy();
	F3dCWaterManager::GetSingleton()->Destroy();
	F3dCFxManager::GetSingleton()->Destroy();
	F3dCTailManager::GetSingleton()->Destroy();
	CFeatherXShaderManager::GetSingleton()->Destroy();
	CFeatherXPostEffect::GetSingleton()->Destroy();
	CFeatherXXmlParser::GetSingleton()->Destroy();
	CFeatherXTextureManager::GetSingleton()->Destroy();
}

void CFeatherXEngineManager::OpenConsoleWindow()
{
	AllocConsole();
	FILE* tmpFile;
	freopen_s(&tmpFile, "CONOUT$",  "wt", stdout);
	printf("CONSOLE\n");
}
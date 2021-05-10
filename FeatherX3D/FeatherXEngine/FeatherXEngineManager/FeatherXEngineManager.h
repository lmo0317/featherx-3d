#pragma once

class CFeatherXEngineManager :public FBSingleton<CFeatherXEngineManager>
{
public:
	CFeatherXEngineManager();
	~CFeatherXEngineManager();

	void Init();
	void Process(double dTime,double dt);
	void Destroy();
	void Render(double dTime,double dt);

	void StartEngine(HWND hWnd);
	void OpenConsoleWindow();

private:
	HWND m_hWnd;
};
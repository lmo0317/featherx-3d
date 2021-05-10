#pragma once

class CFeatherXClientManager : public FBSingleton<CFeatherXClientManager>
{
public:
	CFeatherXClientManager();
	~CFeatherXClientManager();

	void Init(HWND hWnd);
	void Process();
	void Destroy();
	void Render();
	void InitCamera();
	void CreateWorld();
	bool ProcessInput();

	FCGameMain* GetGameMain() {return m_pGameMain;}
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:

	Magi::Timer* m_pTimer;
	double m_dOldTime;
	double m_dDT;

	FCGameMain* m_pGameMain;
};
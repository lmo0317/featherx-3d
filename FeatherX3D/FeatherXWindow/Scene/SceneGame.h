#pragma once

class CSceneGame : public CSceneBase
{
public:
	CSceneGame();
	~CSceneGame();

	virtual bool Process();
	virtual bool Render();
	virtual bool ProcessInput();
	virtual bool InitScene();

	void InitCamera();
	void CreateWorld();
	void CreateLoad();
	void CreateWall(F3dVector& pos,char* modelFile);
	F3dCModel* CreateBox(F3dVector& pos,F3dVector& vecRot,char* modelFile);

private:
};
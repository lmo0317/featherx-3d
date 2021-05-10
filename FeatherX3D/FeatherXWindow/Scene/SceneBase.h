#pragma once

class CSceneBase
{
public:
	virtual bool Process();
	virtual bool Render();
	virtual bool ProcessInput();
	virtual bool InitScene();

	CSceneBase();
	~CSceneBase();
};
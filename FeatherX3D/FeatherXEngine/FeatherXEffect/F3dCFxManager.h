#pragma once

class F3dCFxManager : public FBSingleton< F3dCFxManager >
{
public:
	F3dCFxManager();
	~F3dCFxManager();
	void	Init();
	void	Process(double dTime,double dt);
	void	Render();
	void  Destroy();
	void	SetPlay();
	F3dCParticle* Create(WCHAR* strFile);

private:
	std::vector< F3dCParticle* > m_vecFx;
};
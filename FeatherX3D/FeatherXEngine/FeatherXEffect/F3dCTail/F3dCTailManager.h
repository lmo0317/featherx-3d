#pragma once

class F3dCTailManager : public FBSingleton<F3dCTailManager>
{
public:
	F3dCTailManager();
	~F3dCTailManager();

	void Init();
	void Process(double dTime,double dt);
	void Render(double dTime,double dt);
	void Destroy();
	F3dCTail* CreateTail();

private:

	std::vector<F3dCTail*> m_vecTail;
};
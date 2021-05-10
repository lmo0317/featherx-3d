#pragma once

class F3dCWaterManager : public FBSingleton< F3dCWaterManager > 
{
public:
	F3dCWaterManager();
	~F3dCWaterManager();

	void Init();
	void Process();
	void Render();
	void Destroy();

private:

	std::vector<F3dCWater*> m_vecWater;

};
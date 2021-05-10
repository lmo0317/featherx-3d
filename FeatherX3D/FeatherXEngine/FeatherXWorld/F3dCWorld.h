#pragma once

class F3dCWorld 
{
public:
	F3dCWorld();
	~F3dCWorld();

	void Init();
	void Process();
	void Render();
	void Destroy();

	fcoWorld* GetWorld() {return m_pWorld;}

private:
	fcoWorld* m_pWorld;
};
#pragma once

class FCWorld
{
public:
	FCWorld();
	~FCWorld();

	void Init(MAP_INFO& mapInfo);
	F3dCModel* CreateObject(WCHAR * strFile);

	void Process(double dTime,double dt);
	void Render(double dTime,double dt);

private:

	F3dCWorld* m_pWorld;
	std::vector<F3dCModel*> m_vecObject;
};

#pragma once

class FCWorldManager : public FBSingleton<FCWorldManager>
{
public:
	FCWorldManager();
	~FCWorldManager();
	void CreateWorld(WCHAR* strFile);
	int SelectType(WCHAR* strType);
	void Process(double dTime,double dt);
	void Render(double dTime,double dt);

private:

	FCWorld * m_pWorld;
	std::vector<MAP_INFO> m_vecMapInfo;
};
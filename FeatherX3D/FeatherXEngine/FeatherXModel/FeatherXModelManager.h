#pragma once

class CFeatherXModelManager : public FBSingleton< CFeatherXModelManager >
{
public:
	CFeatherXModelManager();
	~CFeatherXModelManager();
	void	Init();
	void	Render();
	void	Process();
	void	Destroy();

	F3dCModel* CreateModel( WCHAR* strFile);
	ZCParser* GetParser() {return m_pParser;}

private:

	ZCParser*		m_pParser;
	std::vector<F3dCModel*> m_vecModel;
};
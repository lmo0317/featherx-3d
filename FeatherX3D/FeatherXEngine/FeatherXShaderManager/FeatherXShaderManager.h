#pragma once

class CFeatherXShaderManager : public FBSingleton< CFeatherXShaderManager >
{
public:
	CFeatherXShaderManager();
	~CFeatherXShaderManager();

	void	Init();
	void	Process();
	void	Render();
	void	Destroy();

	CFeatherXShader* GetShader(CString strFileName);

protected : 
	std::vector< CFeatherXShader* >					m_vShader;
};


#pragma once

class CFeatherXXmlParser : public FBSingleton< CFeatherXXmlParser >
{
public:
	CFeatherXXmlParser();
	~CFeatherXXmlParser();
	void	ParserXML();

	void	Init();
	void	Process();
	void	Render();
	void	Destroy();
	CFeatherXXml* GetXML() {return m_pXML;}

private:
	CFeatherXXml*			m_pXML;
};
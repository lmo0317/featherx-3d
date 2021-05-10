#include "FeatherXEnginePrecompile.h"

CFeatherXXmlParser::CFeatherXXmlParser()
{
	m_pXML	= new CFeatherXXml;
}

CFeatherXXmlParser::~CFeatherXXmlParser()
{

}

void CFeatherXXmlParser::Init()
{
	ParserXML();
}

void	CFeatherXXmlParser::Process()
{

}

void	CFeatherXXmlParser::Render()
{

}

void	CFeatherXXmlParser::Destroy()
{

}

void CFeatherXXmlParser::ParserXML()
{
	m_pXML->Open(L"test.xml");
}
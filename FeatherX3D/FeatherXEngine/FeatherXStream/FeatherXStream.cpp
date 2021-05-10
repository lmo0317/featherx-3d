#include "FeatherXEnginePrecompile.h"

CFeatherXStream::CFeatherXStream()
{
	m_pBuf = new char[STREAM_SIZE];
}


CFeatherXStream::~CFeatherXStream()
{

}

void	CFeatherXStream::Init()
{

}

void	CFeatherXStream::Process()
{

}

void	CFeatherXStream::Render()
{

}

void	CFeatherXStream::Destroy()
{

}


void CFeatherXStream::Put(void *ptr,int offset)
{
	memcpy(m_pBuf + m_nCurrent,ptr,offset);
	m_nCurrent += offset;
}

void* CFeatherXStream::Get(int start_pos,int offset)
{
	char *ptr;
	memcpy(ptr,m_pBuf + m_nCurrent,offset);
	m_nCurrent += offset;
	return ptr;
}
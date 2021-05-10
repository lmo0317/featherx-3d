#include "FeatherXEnginePrecompile.h"

CFeatherXTexture::CFeatherXTexture()
{
	m_pTexture = NULL;
	m_szName = NULL;
	m_pPrev = NULL;
	m_pNext = NULL;
	m_Count = 0;
}

CFeatherXTexture::~CFeatherXTexture()
{
	if(m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	if(m_pPrev)
		m_pPrev->m_pNext = m_pNext;

	if(m_szName)delete[] m_szName;

	SAFE_RELEASE(m_pTexture);

}
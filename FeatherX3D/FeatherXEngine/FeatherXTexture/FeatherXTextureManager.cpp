#include "FeatherXEnginePrecompile.h"

CFeatherXTextureManager::CFeatherXTextureManager()
{
	m_pHead = NULL;
	m_pTail = NULL;
}

CFeatherXTextureManager::~CFeatherXTextureManager()
{
	CFeatherXTexture* Temp = m_pHead;
	CFeatherXTexture* Temp2;
	while(Temp)
	{
		Temp2 = Temp->m_pNext;
		delete Temp;
		Temp = Temp2;
	}
}

void	CFeatherXTextureManager::Init()
{

}
void	CFeatherXTextureManager::Process()
{

}
void	CFeatherXTextureManager::Render()
{

}
void	CFeatherXTextureManager::Destroy()
{

}


CFeatherXTexture* CFeatherXTextureManager::LoadTexture(LPDIRECT3DDEVICE9 pDevice, WCHAR* Name, bool bMipMap)
{
	CDxUtil DXUtil;
	HRESULT hr;
	CFeatherXTexture* Temp = Search(Name);
	if(Temp)
	{
		Temp->m_Count++;
		return Temp;
	}
	else
	{

		Temp = new CFeatherXTexture;
		Temp->m_szName = new WCHAR[wcslen(Name)+2];
		wcscpy(Temp->m_szName, Name);
		TCHAR szPath[MAX_PATH];

		DXUtil.FindMediaFile(szPath, Temp->m_szName);
		if(bMipMap)
			hr = D3DXCreateTextureFromFileEx(pDevice, szPath, 
			0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER , D3DX_FILTER_BOX,
			/*D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE ,*/
			/*D3DX_FILTER_BOX ,D3DX_FILTER_BOX ,*/
			0xFF000000, NULL, NULL, &(Temp->m_pTexture));
		else
			hr = D3DXCreateTextureFromFileEx(pDevice, szPath, 
			0, 0, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER , D3DX_FILTER_BOX,
			/*D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE ,*/
			/*D3DX_FILTER_BOX ,D3DX_FILTER_BOX ,*/
			0xFF000000, NULL, NULL, &(Temp->m_pTexture));

		if(FAILED(hr))
		{
			delete Temp;
			return NULL;
		}
		Add(Temp);
	}
	return Temp;
}

bool CFeatherXTextureManager::Add(CFeatherXTexture* pTextureList)
{

	if(m_pHead == NULL)
	{
		m_pHead = pTextureList;
		if(m_pTail==NULL)
			m_pTail = pTextureList;
	}
	else
	{
		m_pTail->m_pNext = pTextureList;
		pTextureList->m_pPrev = m_pTail;
		m_pTail = pTextureList;
	}
	return true;
}

CFeatherXTexture* CFeatherXTextureManager::Search(WCHAR* Name)
{
	CFeatherXTexture* Temp = m_pHead;
	while(Temp)
	{
		if(_wcsicmp(Temp->m_szName,Name)==0)
			return Temp;

		Temp = Temp->m_pNext;
	}
	return NULL;
}

CFeatherXTexture* CFeatherXTextureManager::Search(LPDIRECT3DTEXTURE9 pTexture)
{
	CFeatherXTexture* Temp = m_pHead;
	while(Temp)
	{
		if(Temp->m_pTexture == pTexture)return Temp;
		Temp = Temp->m_pNext;
	}
	return NULL;
}

void CFeatherXTextureManager::Delete(LPDIRECT3DTEXTURE9 pTexture)
{
	if(pTexture == NULL)return;
	CFeatherXTexture* Temp = Search(pTexture);
	if(Temp)
	{
		Temp->m_Count --;
		if(Temp->m_Count<0)
		{
			if(Temp == m_pHead)
				m_pHead = Temp->m_pNext;
			if(Temp == m_pTail)
				m_pTail = Temp->m_pPrev;
			delete Temp;
			Temp = NULL;
		}
	}
}

void CFeatherXTextureManager::ReleaseAll()
{
	CFeatherXTexture* Temp = m_pHead;
	while(Temp)
	{
		SAFE_RELEASE(Temp->m_pTexture);
		Temp = Temp->m_pNext;
	}
}

void CFeatherXTextureManager::Restore(LPDIRECT3DDEVICE9 pDevice)
{
	CDxUtil DXUtil;
	CFeatherXTexture* Temp = m_pHead;
	while(Temp)
	{
		// SAFE_RELEASE(Temp->m_pTexture);
		TCHAR szPath[MAX_PATH];
		DXUtil.FindMediaFile(szPath, Temp->m_szName);
		D3DXCreateTextureFromFileEx(pDevice, szPath, 
			0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER , D3DX_FILTER_BOX,
			/*D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE ,*/
			/*D3DX_FILTER_BOX ,D3DX_FILTER_BOX ,*/
			0xFF000000, NULL, NULL, &(Temp->m_pTexture));
		Temp = Temp->m_pNext;
	}
}

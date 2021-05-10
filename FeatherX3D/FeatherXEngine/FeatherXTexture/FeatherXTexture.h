#pragma once

class CFeatherXTexture
{
public:
	LPDIRECT3DTEXTURE9 m_pTexture;
	WCHAR* m_szName;
	int m_Count;

	CFeatherXTexture* m_pPrev;
	CFeatherXTexture* m_pNext;

	CFeatherXTexture();
	~CFeatherXTexture();
};
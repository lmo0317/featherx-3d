#pragma once

class CFeatherXTextureManager : public FBSingleton< CFeatherXTextureManager >
{
public:
	void Restore(LPDIRECT3DDEVICE9 pDevice);
	void ReleaseAll();
	CFeatherXTextureManager();
	virtual ~CFeatherXTextureManager();

	void	Init();
	void	Process();
	void	Render();
	void	Destroy();

	CFeatherXTexture* m_pHead;
	CFeatherXTexture* m_pTail;

	CFeatherXTexture* LoadTexture(LPDIRECT3DDEVICE9 pDevice, WCHAR* Name, bool bMipMap= true);
	CFeatherXTexture* Search(LPDIRECT3DTEXTURE9 pTexture);
	CFeatherXTexture* Search(WCHAR* Name);
	void Delete(LPDIRECT3DTEXTURE9 pTexture);
	bool Add(CFeatherXTexture*);
};
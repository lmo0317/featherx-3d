#pragma once

#include "NxVec3.h"

class fcoWorld
{
public:
	fcoWorld();
	~fcoWorld();

	LPDIRECT3DVERTEXBUFFER9* GetVB() {return &m_pVB;}
	LPDIRECT3DINDEXBUFFER9*  GetIB() {return &m_pIB;}

	float GetZHeight() {return m_czHeight;}
	float GetXHeight() {return m_cxHeight;}

	void SetZHeight(DWORD czHeight) {m_czHeight = czHeight;}
	void SetXHeight(DWORD cxHeight) {m_cxHeight = cxHeight;}

	int GetVBNum() {return m_czHeight * m_cxHeight;}
	int GetIBNum() {return (m_cxHeight-1)*(m_czHeight-1)*2;}

	LPDIRECT3DTEXTURE9* GetTexHeight() {return &m_pTexHeight;}
	LPDIRECT3DTEXTURE9* GetTexDiffuse() {return &m_pTexDiffuse;}

	NxVec3* GetVertex() {return m_pVerts;}
	void SetVertex(NxVec3* pVerts) {m_pVerts = pVerts;}

	NxU32* GetFaces() {return m_pFaces;}
	void SetFaces(NxU32* pFaces) {m_pFaces = pFaces;}

	bool InitVB();
	bool InitIB();
	bool InitTexture();
	bool InitGeometry();

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVB; /// 정점을 보관할 정점버퍼
	LPDIRECT3DINDEXBUFFER9	m_pIB; /// 인덱스를 보관할 인덱스버퍼

	LPDIRECT3DTEXTURE9		m_pTexHeight; /// Texture 높이맵
	LPDIRECT3DTEXTURE9		m_pTexDiffuse; /// Texture 색깔맵

	DWORD					m_cxHeight;
	DWORD					m_czHeight;

	NxVec3*		m_pVerts;
	NxU32*		m_pFaces;

	NxU32		m_nbVerts;
	NxU32		m_nbFaces;
};
#pragma once

class CFeatherXShader
{
public:
	CFeatherXShader();
	~CFeatherXShader();

	D3DXHANDLE GetMatView() {return  m_hMatView;}
	D3DXHANDLE GetLightDir() {return m_hLightDir;}
	D3DXHANDLE GetColor() {return m_hColor;}
	D3DXHANDLE GetEyePos() {return m_hEyePos;}
	D3DXHANDLE GetWVP() {return m_hWVP;}
	D3DXHANDLE GetMatProjectionView() {return  m_hMatProjection;}
	D3DXHANDLE GetTechnique() { return m_hTechnique;}
	LPD3DXEFFECT GetEffect() {return m_pEffect;}	
	LPDIRECT3DVERTEXDECLARATION9 GetDecl() {return m_pDecl;}
	bool InitShader(LPDIRECT3DDEVICE9 Device, CString name, D3DVERTEXELEMENT9 *VertexDecl);
	bool Begin(int iStart,...);
	void End(LPDIRECT3DDEVICE9 &Device);
	D3DXHANDLE GetHandleTechnique() const 	{ return m_hTechnique; }
	D3DXHANDLE GetHandleMatProjection() const 	{ return m_hMatProjection; }
	D3DXHANDLE GetHandleMatView() const 	{ return m_hMatView; }
	D3DXHANDLE GetHandleTexture() const    { return m_hTexture;}
	void DeleteShader();	
	bool ResetDevice();
	void LostDevice();
	void SetVertexDeclaration(LPDIRECT3DVERTEXDECLARATION9 pDecl) { m_pDecl = pDecl; }	
	bool SetShaderMatrix(LPDIRECT3DDEVICE9 &Device, D3DXMATRIX & matView,D3DXMATRIX & matPro);
	CString GetFileName() {return m_strFileName;}
private:

	CString m_strFileName;
	D3DXHANDLE m_hTechnique;
	D3DXHANDLE m_hMatProjection;
	D3DXHANDLE m_hMatView;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hWVP;
	D3DXHANDLE m_hLightDir;
	D3DXHANDLE m_hEyePos;
	D3DXHANDLE m_hColor;
	LPD3DXEFFECT m_pEffect;
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
};
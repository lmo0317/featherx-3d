// D3dUtil.h: interface for the CD3dUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DUTIL_H__5FD0C84F_EBA0_11D4_94A9_000102929A18__INCLUDED_)
#define AFX_D3DUTIL_H__5FD0C84F_EBA0_11D4_94A9_000102929A18__INCLUDED_

#pragma once

#include <D3D9.h>
#include <D3DX9Math.h>

class CD3dUtil  
{
public:
	CD3dUtil();
	virtual ~CD3dUtil();

//-----------------------------------------------------------------------------
// Name: D3DUtil_InitLight()
// Desc: Initializes a D3DLIGHT structure, setting the light position. The
//       diffuse color is set to white, specular and ambient left as black.
//-----------------------------------------------------------------------------
	void InitMaterial( D3DMATERIAL9& mtrl, FLOAT r=0.0f, FLOAT g=0.0f,
                                               FLOAT b=0.0f, FLOAT a=1.0f );
//-----------------------------------------------------------------------------
// Name: D3DUtil_CreateTexture()
// Desc: Helper function to create a texture. It checks the root path first,
//       then tries the DXSDK media path (as specified in the system registry).
//-----------------------------------------------------------------------------
	void InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType,
                        FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f );

	HRESULT CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
								   LPDIRECT3DTEXTURE9* ppTexture,
								   D3DFORMAT d3dFormat = D3DFMT_UNKNOWN );
//-----------------------------------------------------------------------------
// Name: D3DUtil_SetColorKey()
// Desc: Changes all texels matching the colorkey to transparent, black.
//-----------------------------------------------------------------------------
	HRESULT SetColorKey( LPDIRECT3DTEXTURE9 pTexture, DWORD dwColorKey );
//-----------------------------------------------------------------------------
// Name: D3DUtil_CreateVertexShader()
// Desc: Assembles and creates a file-based vertex shader
//-----------------------------------------------------------------------------
	HRESULT CreateVertexShader( LPDIRECT3DDEVICE9 pd3dDevice, 
										TCHAR* strFilename, DWORD* pdwVertexDecl,
										DWORD* pdwVertexShader );
									
//-----------------------------------------------------------------------------
// Name: D3DUtil_GetCubeMapViewMatrix()
// Desc: Returns a view matrix for rendering to a face of a cubemap.
//-----------------------------------------------------------------------------
	D3DXMATRIX GetCubeMapViewMatrix( DWORD dwFace );

//-----------------------------------------------------------------------------
// Name: D3DUtil_GetRotationFromCursor()
// Desc: Returns a quaternion for the rotation implied by the window's cursor
//       position.
//-----------------------------------------------------------------------------
	D3DXQUATERNION GetRotationFromCursor( HWND hWnd,
                                              FLOAT fTrackBallRadius=1.0f );

//-----------------------------------------------------------------------------
// Name: D3DUtil_SetDeviceCursor
// Desc: Builds and sets a cursor for the D3D device based on hCursor.
//-----------------------------------------------------------------------------
	HRESULT SetDeviceCursor( LPDIRECT3DDEVICE9 pd3dDevice, HCURSOR hCursor );

};

#endif // !defined(AFX_D3DUTIL_H__5FD0C84F_EBA0_11D4_94A9_000102929A18__INCLUDED_)

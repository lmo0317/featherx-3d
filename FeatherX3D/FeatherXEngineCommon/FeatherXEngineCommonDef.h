#pragma once

/// 사용자 정점을 정의할 구조체
struct CUSTOMVERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR2		t;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
struct MYINDEX
{
	WORD _0,_1,_2;
};

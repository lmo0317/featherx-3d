#include "FeatherXEnginePrecompile.h"
 


F3dCRenderTargetUISprite::F3dCRenderTargetUISprite() : m_nOffSetX(0), m_nOffSetY(0)
{
	 
}

F3dCRenderTargetUISprite::~F3dCRenderTargetUISprite()
{
	 
}

void F3dCRenderTargetUISprite::Init()
{
	 
}

void F3dCRenderTargetUISprite::Finish()
{
	 
}

void F3dCRenderTargetUISprite::RegUISprite( const F3dCUISprite* pUISprite, int nLayer )
{
	 

	assert( pUISprite );

	if( NULL == pUISprite ) return;

	if( !pUISprite->bShow ) return;

	if( nLayer == LAYER_NORMAL )
		m_vUISprite.push_back( pUISprite );
	else if( nLayer == LAYER_MASK )
		m_vUISpriteMask.push_back( pUISprite );
	else if( nLayer == LAYER_COVER )
		m_vUISpriteCover.push_back( pUISprite );
}

void F3dCRenderTargetUISprite::RegUIWireSprite( const F3dCUISprite* pUISprite )
{
	 

	assert( pUISprite );
	m_vUIWireSprite.push_back( pUISprite );
}

void F3dCRenderTargetUISprite::SetOffSet( int nOffSetX, int nOffSetY )
{
	 

	m_nOffSetX = nOffSetX;
	m_nOffSetY = nOffSetY;
}

void F3dCRenderTargetUISprite::RenderWire()
{
	 
	if( m_vUIWireSprite.empty() )
		return;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();

	// Render State
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ZENABLE, FALSE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );

	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	F3dCDevice::GetSingleton()->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );
	F3dCDevice::GetSingleton()->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	F3dCDevice::GetSingleton()->SetFVF( FVF );

	std::vector< const F3dCUISprite* >::iterator it = m_vUIWireSprite.begin();
	for(; it != m_vUIWireSprite.end(); ++it )
	{
		RenderUIWireSprite( *it );
	}

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	F3dCDevice::GetSingleton()->SetRenderState(D3DRS_ZENABLE, TRUE);
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	m_vUIWireSprite.clear();
}

void F3dCRenderTargetUISprite::Render()
{
	 

	if( m_vUISprite.empty() )
		return;

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();

	// Render State
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_LIGHTING, FALSE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ZENABLE, FALSE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_ALWAYS );

	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );

	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );
	F3dCDevice::GetSingleton()->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE );

	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_GAUSSIANQUAD  );
	F3dCDevice::GetSingleton()->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );
	F3dCDevice::GetSingleton()->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );

	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

//	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
//	F3dCDevice::GetSingleton()->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	F3dCDevice::GetSingleton()->SetFVF( FVF );

	std::vector< const F3dCUISprite* >::iterator it = m_vUISprite.begin();
	for(; it != m_vUISprite.end(); ++it )
	{
		RenderUISprite( *it );
	}

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_DESTALPHA );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_MIN  );

	std::vector< const F3dCUISprite* >::iterator it2 = m_vUISpriteMask.begin();
	for(; it2 != m_vUISpriteMask.end(); ++it2 )
	{
		RenderUISprite( *it2 );
	} 

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ); 
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );

	std::vector< const F3dCUISprite* >::iterator it3 = m_vUISpriteCover.begin();
	for(; it3 != m_vUISpriteCover.end(); ++it3 )
	{
		RenderUISprite( *it3 );
	}

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_LIGHTING, TRUE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ZENABLE, TRUE );
	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	F3dCDevice::GetSingleton()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	m_vUISprite.clear();
	m_vUISpriteMask.clear();
	m_vUISpriteCover.clear();
}

void F3dCRenderTargetUISprite::RenderUIWireSprite( const F3dCUISprite* pUISprite )
{
	 

	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();

	F3dCDevice::GetSingleton()->SetTexture( 0, NULL );

	int sx = pUISprite->rtScreen.left;
	int sy = pUISprite->rtScreen.top;
	int ex = pUISprite->rtScreen.right -1;
	int ey = pUISprite->rtScreen.bottom-1;

	float sx5	= sx - 0.5f;
	float sy5	= sy - 0.5f;
	float ex5	= ex + 0.5f;
	float ey5	= ey + 0.5f;

	//TODO : Control 단위로 찍도록 작업 예정
	VERTEX svQuad[4];
	svQuad[0].p = MakeVector4( sx5,	sy5, 0.5f, 1.0f );	svQuad[0].u = pUISprite->lu;	svQuad[0].v = pUISprite->tv;
	svQuad[1].p = MakeVector4( ex5, sy5, 0.5f, 1.0f );	svQuad[1].u = pUISprite->ru;	svQuad[1].v = pUISprite->tv;
	svQuad[2].p = MakeVector4( sx5,	ey5, 0.5f, 1.0f );	svQuad[2].u = pUISprite->lu;	svQuad[2].v = pUISprite->bv;
	svQuad[3].p = MakeVector4( ex5, ey5, 0.5f, 1.0f );	svQuad[3].u = pUISprite->ru;	svQuad[3].v = pUISprite->bv;
	svQuad[0].color = pUISprite->color_lu;
	svQuad[1].color = pUISprite->color_tv;
	svQuad[2].color = pUISprite->color_ru;
	svQuad[3].color = pUISprite->color_bv;
	F3dCDevice::GetSingleton()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(F3dCRenderTargetUISprite::VERTEX));	
}

void RenderTargetStrip( IDirect3DDevice9* pD3DDevice, const F3dCUISprite* pUISprite, const int nOffSetX, const int nOffSetY )
{
	if( pUISprite->IsUseClip() )
	{
		F3dCTexture* pTexture = NULL;
		if( pUISprite->bText )
			pTexture = F3dCTextTextureManager::GetSingleton()->FindTexture( pUISprite->TextureID );
		else
			pTexture = F3dCTextureManager::GetSingleton()->FindTexture_Render( pUISprite->TextureID );

		int sx = pUISprite->rtScreen.left     + nOffSetX;
		int sy = pUISprite->rtScreen.top      + nOffSetY;
		int ex = pUISprite->rtScreen.right -1 + nOffSetX;
		int ey = pUISprite->rtScreen.bottom-1 + nOffSetY;

		float fLeftU, fTopV, fRightU, fBottomV;
		fLeftU = fTopV = 0.f;	fRightU = fBottomV = 1.f;

		int nTemp = 0;
		nTemp = pUISprite->rtClip.left + nOffSetX - sx;
		if( nTemp > pUISprite->GetWidth() ) {
			return;
		}
		nTemp = pUISprite->rtClip.top + nOffSetY - sy;
		if( nTemp > pUISprite->GetHeight() ) {
			return;
		}
		if( sx > pUISprite->rtClip.right + nOffSetX ) {
			return;
		}
		if( sy > pUISprite->rtClip.bottom + nOffSetY ) {
			return;
		}

		if( pUISprite->bText )
		{
			fRightU  = pUISprite->ru;
			fBottomV = pUISprite->bv;

			if( sx < pUISprite->rtClip.left + nOffSetX ) {
				nTemp	= pUISprite->rtClip.left - sx;
				sx		= pUISprite->rtClip.left;
				fLeftU	= float(nTemp) / pUISprite->ori_rect.GetWidth() * pUISprite->ru;
			}
			if( sy < pUISprite->rtClip.top + nOffSetY ) {
				nTemp	= pUISprite->rtClip.top - sy;
				sy		= pUISprite->rtClip.top;
				fTopV	= float(nTemp) / pUISprite->ori_rect.GetHeight() * pUISprite->bv;
			}

			if( ex > pUISprite->rtClip.right + nOffSetX ) {
				nTemp	= ex - pUISprite->rtClip.right;
				ex		= pUISprite->rtClip.right;
				fRightU = pUISprite->ru - (float(nTemp) / pUISprite->ori_rect.GetWidth() * pUISprite->ru );
			}
			if( ey > pUISprite->rtClip.bottom + nOffSetY ) {
				nTemp	= ey - pUISprite->rtClip.bottom;
				ey		= pUISprite->rtClip.bottom;
				fBottomV= pUISprite->bv - (float(nTemp) / pUISprite->ori_rect.GetHeight() * pUISprite->bv );
			}
		}
		else
		{
			if( sx < (pUISprite->rtClip.left + nOffSetX) ) {
				nTemp	= (pUISprite->rtClip.left + nOffSetX) - sx;
				sx		= pUISprite->rtClip.left + nOffSetX;
				fLeftU	= float(nTemp) / pUISprite->ori_rect.GetWidth();
			}
			if( sy < (pUISprite->rtClip.top + nOffSetY) ) {
				nTemp	= (pUISprite->rtClip.top + nOffSetY) - sy;
				sy		= pUISprite->rtClip.top + nOffSetY;
				fTopV	= float(nTemp) / pUISprite->ori_rect.GetHeight();
			}

			if( ex > (pUISprite->rtClip.right + nOffSetX) ) {
				nTemp	= (ex - (pUISprite->rtClip.right + nOffSetX));
				ex		= pUISprite->rtClip.right + nOffSetX;
				fRightU = 1.f - (float(nTemp) / pUISprite->ori_rect.GetWidth());
			}
			if( ey > (pUISprite->rtClip.bottom + nOffSetY) ) {
				nTemp	= (ey - (pUISprite->rtClip.bottom + nOffSetY));
				ey		= pUISprite->rtClip.bottom + nOffSetY;
				fBottomV= 1.f - (float(nTemp) / pUISprite->ori_rect.GetHeight());
			}
		}

		float sx5	= sx - 0.5f;
		float sy5	= sy - 0.5f;
		float ex5	= ex + 0.5f;
		float ey5	= ey + 0.5f;

		F3dCRenderTargetUISprite::VERTEX svQuad[4];
		svQuad[0].p = MakeVector4( sx5,	sy5, 0.5f, 1.0f );	svQuad[0].u = fLeftU;	svQuad[0].v = fTopV;
		svQuad[1].p = MakeVector4( ex5, sy5, 0.5f, 1.0f );	svQuad[1].u = fRightU;	svQuad[1].v = fTopV;
		svQuad[2].p = MakeVector4( sx5,	ey5, 0.5f, 1.0f );	svQuad[2].u = fLeftU;	svQuad[2].v = fBottomV;
		svQuad[3].p = MakeVector4( ex5, ey5, 0.5f, 1.0f );	svQuad[3].u = fRightU;	svQuad[3].v = fBottomV;
		svQuad[0].color = pUISprite->color_lu;
		svQuad[1].color = pUISprite->color_tv;
		svQuad[2].color = pUISprite->color_ru;
		svQuad[3].color = pUISprite->color_bv;
		F3dCDevice::GetSingleton()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(F3dCRenderTargetUISprite::VERTEX));
	}
	else
	{
		float v_x0 = (float)pUISprite->rtScreen.left + nOffSetX;
		float v_y0 = (float)pUISprite->rtScreen.top  + nOffSetY;

		float v_x1 = (float)pUISprite->rtScreen.right -1 + nOffSetX;
		float v_y1 = (float)pUISprite->rtScreen.top      + nOffSetY;

		float v_x2 = (float)pUISprite->rtScreen.left     + nOffSetX;
		float v_y2 = (float)pUISprite->rtScreen.bottom-1 + nOffSetY;

		float v_x3 = (float)pUISprite->rtScreen.right -1 + nOffSetX;
		float v_y3 = (float)pUISprite->rtScreen.bottom-1 + nOffSetY;

#define _SPR_ROTATE_

#ifdef _SPR_ROTATE_
		F3dVector center;
		center.x = pUISprite->rtScreen.left + nOffSetX + pUISprite->GetWidth()  / 2.f;
		center.y = pUISprite->rtScreen.top  + nOffSetY + pUISprite->GetHeight() / 2.f;

		v_x0 -= center.x;
		v_y0 -= center.y;
		v_x1 -= center.x;
		v_y1 -= center.y;
		v_x2 -= center.x;
		v_y2 -= center.y;
		v_x3 -= center.x;
		v_y3 -= center.y;

		//// Rotate
		//	x' = x cos f - y sin f
		//	y' = x sin f + y cos f

		float cosValue = cos(pUISprite->m_fRadian);
		float sinValue = sin(pUISprite->m_fRadian);

		float o_x0 = v_x0 * cosValue - v_y0 * sinValue + center.x - 0.5f;
		float o_y0 = v_x0 * sinValue + v_y0 * cosValue + center.y - 0.5f;

		float o_x1 = v_x1 * cosValue - v_y1 * sinValue + center.x + 0.5f;
		float o_y1 = v_x1 * sinValue + v_y1 * cosValue + center.y - 0.5f;

		float o_x2 = v_x2 * cosValue - v_y2 * sinValue + center.x - 0.5f;
		float o_y2 = v_x2 * sinValue + v_y2 * cosValue + center.y + 0.5f;

		float o_x3 = v_x3 * cosValue - v_y3 * sinValue + center.x + 0.5f;
		float o_y3 = v_x3 * sinValue + v_y3 * cosValue + center.y + 0.5f;

		//TODO : Control 단위로 찍도록 작업 예정
		F3dCRenderTargetUISprite::VERTEX svQuad[4];
		svQuad[0].p = MakeVector4( o_x0, o_y0, 0.5f, 1.0f );	svQuad[0].u = pUISprite->lu;	svQuad[0].v = pUISprite->tv;
		svQuad[1].p = MakeVector4( o_x1, o_y1, 0.5f, 1.0f );	svQuad[1].u = pUISprite->ru;	svQuad[1].v = pUISprite->tv;
		svQuad[2].p = MakeVector4( o_x2, o_y2, 0.5f, 1.0f );	svQuad[2].u = pUISprite->lu;	svQuad[2].v = pUISprite->bv;
		svQuad[3].p = MakeVector4( o_x3, o_y3, 0.5f, 1.0f );	svQuad[3].u = pUISprite->ru;	svQuad[3].v = pUISprite->bv;
		svQuad[0].color = pUISprite->color_lu;
		svQuad[1].color = pUISprite->color_tv;
		svQuad[2].color = pUISprite->color_ru;
		svQuad[3].color = pUISprite->color_bv;
		F3dCDevice::GetSingleton()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(F3dCRenderTargetUISprite::VERTEX));

#else
		float sx5	= sx - 0.5f;
		float sy5	= sy - 0.5f;
		float ex5	= ex + 0.5f;
		float ey5	= ey + 0.5f;

		//TODO : Control 단위로 찍도록 작업 예정
		F3dCRenderTargetUISprite::VERTEX svQuad[4];
		svQuad[0].p = MakeVector4( sx5,	sy5, 0.5f, 1.0f );	svQuad[0].u = pUISprite->lu;	svQuad[0].v = pUISprite->tv;
		svQuad[1].p = MakeVector4( ex5, sy5, 0.5f, 1.0f );	svQuad[1].u = pUISprite->ru;	svQuad[1].v = pUISprite->tv;
		svQuad[2].p = MakeVector4( sx5,	ey5, 0.5f, 1.0f );	svQuad[2].u = pUISprite->lu;	svQuad[2].v = pUISprite->bv;
		svQuad[3].p = MakeVector4( ex5, ey5, 0.5f, 1.0f );	svQuad[3].u = pUISprite->ru;	svQuad[3].v = pUISprite->bv;
		svQuad[0].color = pUISprite->color_lu;
		svQuad[1].color = pUISprite->color_tv;
		svQuad[2].color = pUISprite->color_ru;
		svQuad[3].color = pUISprite->color_bv;

		F3dCDevice::GetSingleton()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(F3dCRenderTargetUISprite::VERTEX));
#endif


	}
}

namespace
{
	const int c_nFanAlpha = 200;

	const int c_temp_unit00[8][2] = 
	{
		{ 2, 0 },
		{ 2, 1 },
		{ 2, 2 },
		{ 1, 2 },
		{ 0, 2 },
		{ 0, 1 },
		{ 0, 0 },
		{ 1, 0 },
	};

	const int c_temp_uint01[8][2] = 
	{
		{ 0, 0 },
		{ 0, 1 },
		{ 0, 2 },
		{ 1, 2 },
		{ 2, 2 },
		{ 2, 1 },
		{ 2, 0 },
		{ 1, 0 },
	}; 

	const float c_dec_temp_unit00[8][2] = 
	{
		{ -1.f,  0.f },
		{  0.f, -1.f },
		{  0.f, -1.f },
		{  1.f,  0.f },
		{  1.f,  0.f },
		{  0.f,  1.f },
		{  0.f,  1.f },
		{ -1.f,  0.f },	
	};

	const float c_dec_temp_unit01[8][2] = 
	{
		{  1.f,  0.f },
		{  0.f, -1.f },
		{  0.f, -1.f },
		{ -1.f,  0.f },
		{ -1.f,  0.f },
		{  0.f,  1.f },
		{  0.f,  1.f },
		{  1.f,  0.f },	
	};
};

void RenderTargetFan( IDirect3DDevice9* pD3DDevice, const F3dCUISprite* pUISprite, const int nOffSetX, const int nOffSetY )
{
	static F3dCRenderTargetUISprite::VERTEX svFan[10];

	if( pUISprite->nDegree == 0)
		return;

	float sprtx,sprty;
	sprtx = (float)(pUISprite->rtScreen.left + nOffSetX + pUISprite->GetWidth());
	sprty = (float)(pUISprite->rtScreen.top  + nOffSetY + pUISprite->GetHeight());

	F3dVector center;
	center.x = pUISprite->rtScreen.left + nOffSetX + pUISprite->GetWidth()  / 2.f;
	center.y = pUISprite->rtScreen.top  + nOffSetY + pUISprite->GetHeight() / 2.f;

	const float UNIT = 8.f / 360.f;
	float fCurVtx = (float)(pUISprite->nDegree) * UNIT; 

	float fCurVtxOffset = static_cast<float>( static_cast<int>( fCurVtx ) + 1) - fCurVtx;
	int nCurVtxSize = (int)(fCurVtx  + 2 + (fCurVtxOffset >= 1.0f ? 0 : 1 ));

	if( nCurVtxSize > 10 )
		nCurVtxSize = 10;

	fCurVtxOffset = fCurVtxOffset - static_cast<int>( fCurVtxOffset );

	for(int i = 0; i < nCurVtxSize; ++i)	{
		svFan[i].color = (c_nFanAlpha<<24)+(0x00000000 & pUISprite->color_lu);
	}

	svFan[0].p = MakeVector4( center.x, center.y, 0.5f, 1.f );
	svFan[1].p = MakeVector4( center.x, (float)(pUISprite->rtScreen.top + nOffSetY), 0.5f, 1.f ); 

	float fPosX[3] = { (float)pUISprite->rtScreen.left + nOffSetX, center.x, sprtx };
	float fPosY[3] = { (float)pUISprite->rtScreen.top  + nOffSetY, center.y, sprty };

	static int SELECT_UNIT[8][2];
	if( pUISprite->bReverse )	{
		::memcpy( SELECT_UNIT, c_temp_unit00, sizeof(c_temp_unit00) );
	}
	else	{
		::memcpy( SELECT_UNIT, c_temp_uint01, sizeof(c_temp_uint01) );
	}

	nCurVtxSize = std::min( nCurVtxSize, 10);

	for(int i = 2; i < nCurVtxSize; ++i)	{
		svFan[i].p = MakeVector4( fPosX[ SELECT_UNIT[i - 2][0] ], fPosY[ SELECT_UNIT[i - 2][1] ], 0.5f, 1.f );
	}

	/*static*/ float DECEREASE_UNIT[8][2];
	if( pUISprite->bReverse )	{
		::memcpy( DECEREASE_UNIT, c_dec_temp_unit00, sizeof(c_dec_temp_unit00) );
	}
	else {
		::memcpy( DECEREASE_UNIT, c_dec_temp_unit01, sizeof(c_dec_temp_unit01) );
	}

	int nOffsetVtx = std::max( nCurVtxSize - 3, 0 );

	svFan[ nCurVtxSize - 1].p.x += DECEREASE_UNIT[ nOffsetVtx ][0] * fCurVtxOffset * pUISprite->GetWidth()  * .5f;
	svFan[ nCurVtxSize - 1].p.y += DECEREASE_UNIT[ nOffsetVtx ][1] * fCurVtxOffset * pUISprite->GetHeight() * .5f;

	F3dCDevice::GetSingleton()->SetTexture( 0, NULL );
	F3dCDevice::GetSingleton()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nCurVtxSize-2, svFan, sizeof(F3dCRenderTargetUISprite::VERTEX));
}

void F3dCRenderTargetUISprite::RenderUISprite( const F3dCUISprite* pUISprite )
{
	IDirect3DDevice9* pD3DDevice = F3dCDevice::GetSingleton()->GetD3DDevice();
	F3dCTexture* pTexture = NULL;
	if( pUISprite->bText )
		pTexture = F3dCTextTextureManager::GetSingleton()->FindTexture( pUISprite->TextureID );
	else
		pTexture = F3dCTextureManager::GetSingleton()->FindTexture_Render( pUISprite->TextureID );

	if( pUISprite->nDrawType == F3dCUISprite::TRIANGLE_STRIP ) {
		if( pTexture == NULL )
			return;

		F3dCDevice::GetSingleton()->SetTexture( 0, pTexture->GetTexture() );

		RenderTargetStrip( pD3DDevice, pUISprite, m_nOffSetX, m_nOffSetY );
	}
	else if( pUISprite->nDrawType == F3dCUISprite::TRIANGLE_FAN   ) {
		RenderTargetFan( pD3DDevice, pUISprite, m_nOffSetX, m_nOffSetY );
	}
}

#pragma once

class F3dCRenderTargetUISprite
{
public:
	struct VERTEX
	{
		F3dVector4 p;
		F3dColor color;
		float u, v;
	};
	enum
	{
		FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
	};

	enum
	{
		LAYER_NORMAL,
		LAYER_MASK,
		LAYER_COVER,
	};

public:
	F3dCRenderTargetUISprite();
	~F3dCRenderTargetUISprite();

	void		Init();
	void		Finish();
	void		RegUISprite( const F3dCUISprite* pUISprite, int nLayer );
	void		RegUIWireSprite( const F3dCUISprite* pUISprite );

	void		Render();
	void		RenderWire();

	void		SetOffSet( int nOffSetX, int nOffSetY );

private:

	void		RenderUISprite( const F3dCUISprite* pUISprite );
	void		RenderUIWireSprite( const F3dCUISprite* pUISprite );

	std::vector< const F3dCUISprite* > m_vUISprite;
	std::vector< const F3dCUISprite* > m_vUISpriteMask;
	std::vector< const F3dCUISprite* > m_vUISpriteCover;


	std::vector< const F3dCUISprite* > m_vUIWireSprite;

	int         m_nOffSetX;
	int         m_nOffSetY;
};
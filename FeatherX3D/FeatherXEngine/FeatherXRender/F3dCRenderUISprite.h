#pragma once

class F3dCRenderUISprite
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

public:
	F3dCRenderUISprite();
	~F3dCRenderUISprite();

	void		Init();
	void		Finish();
	void		Clear();

	void		RegUISprite( const F3dCUISprite* pUISprite );
	void		RegUIWireSprite( const F3dCUISprite* pUISprite );

	void		Render();
	void		RenderWire();

private:
	void		RenderUISprite( const F3dCUISprite* pUISprite );
	void		RenderUIWireSprite( const F3dCUISprite* pUISprite );

	std::vector< const F3dCUISprite* > m_vUISprite;
	std::vector< const F3dCUISprite* > m_vUIWireSprite;
};

#pragma once 

typedef struct {
	FLOAT       p[4];
	FLOAT       tu, tv;
} TVERTEX;

struct CoordRect
{
	float u0, v0;
	float u1, v1;
};

enum{
	NUM_STAR_TEXTURES =  8, // 광선텍스처 개수
	MAX_SAMPLES       = 16, // 샘플러의 최대수
};

class CFeatherXPostEffect  : public FBSingleton< CFeatherXPostEffect >
{
public:
	CFeatherXPostEffect();
	~CFeatherXPostEffect();
	
	void	Init();
	void	Process();
	void	Render();
	void	Destroy();

	HRESULT Scene_To_SceneScaled();
	HRESULT SceneScaled_To_BrightPass();
	HRESULT GetGaussBlur5x5(DWORD dwD3DTexWidth, DWORD dwD3DTexHeight,
		D3DXVECTOR2* avTexCoordOffset, D3DXVECTOR4* avSampleWeight);
	HRESULT BrightPass_To_StarSource();
	HRESULT RenderStar();
	HRESULT GetTextureCoords( PDIRECT3DTEXTURE9 pTexSrc, RECT* pRectSrc, PDIRECT3DTEXTURE9 pTexDest, RECT* pRectDest, CoordRect* pCoords );
	void DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV);
	void RenderScene();
	bool Begin();

	PDIRECT3DSURFACE9		m_pSurfScene;
private:
	float m_fTime;
	LPDIRECT3DSURFACE9		m_pMapZ;		// 깊이버퍼
	LPDIRECT3DTEXTURE9		m_pOriginalTex;	// 텍스처
	LPDIRECT3DSURFACE9		m_pOriginalSurf;// 표면
	PDIRECT3DSURFACE9		m_pBackBuffer;

	D3DXMATRIX				m_mWorld;
	D3DXMATRIX				m_mView;
	D3DXMATRIX				m_mProj;


	DWORD m_dwCropWidth;
	DWORD m_dwCropHeight;

	PDIRECT3DTEXTURE9		m_pTexScene;	    // HDR화면을 렌더할 곳
	PDIRECT3DTEXTURE9		m_pTexSceneScaled;	// 축소버퍼
	PDIRECT3DSURFACE9		m_pSurfSceneScaled;
	PDIRECT3DTEXTURE9		m_pTexBrightPass;	// 휘도 추출
	PDIRECT3DSURFACE9		m_pSurfBrightPass;
	PDIRECT3DTEXTURE9		m_pTexStarSource;   // 별의 원본이 될 이미지
	PDIRECT3DSURFACE9		m_pSurfStarSource;
	PDIRECT3DTEXTURE9		m_apTexStar[NUM_STAR_TEXTURES];// 임시
	PDIRECT3DSURFACE9		m_apSurfStar[NUM_STAR_TEXTURES];

	// 장면렌더용
	LPDIRECT3DTEXTURE9		m_pNormalMap;	// 법선맵
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// 정점선언
	
	// 셰이더
	LPD3DXEFFECT		    m_pEffect;		// 이펙트
	D3DXHANDLE				m_hTechnique;	// 테크닉
	D3DXHANDLE				m_hmWVP;		// 로컬-투영 변환행렬
	D3DXHANDLE				m_hvLightDir;	// 광원방향
	D3DXHANDLE				m_hvColor;		// 정점색
	D3DXHANDLE				m_hvEyePos;		// 시점
	D3DXHANDLE				m_htDecaleTex;	// 디컬텍스처
	D3DXHANDLE				m_htNormalMap;	// 법선맵


	// 변환행렬
	D3DXVECTOR4				m_vFromPt;

	BOOL					m_bLoadingApp;	// 로드중?
	
	FLOAT                   m_fWorldRotX;   // X축회전
	FLOAT                   m_fWorldRotY;   // Y축회전
	FLOAT                   m_fViewZoom;    // 시점거리

	D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer
};
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
	NUM_STAR_TEXTURES =  8, // �����ؽ�ó ����
	MAX_SAMPLES       = 16, // ���÷��� �ִ��
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
	LPDIRECT3DSURFACE9		m_pMapZ;		// ���̹���
	LPDIRECT3DTEXTURE9		m_pOriginalTex;	// �ؽ�ó
	LPDIRECT3DSURFACE9		m_pOriginalSurf;// ǥ��
	PDIRECT3DSURFACE9		m_pBackBuffer;

	D3DXMATRIX				m_mWorld;
	D3DXMATRIX				m_mView;
	D3DXMATRIX				m_mProj;


	DWORD m_dwCropWidth;
	DWORD m_dwCropHeight;

	PDIRECT3DTEXTURE9		m_pTexScene;	    // HDRȭ���� ������ ��
	PDIRECT3DTEXTURE9		m_pTexSceneScaled;	// ��ҹ���
	PDIRECT3DSURFACE9		m_pSurfSceneScaled;
	PDIRECT3DTEXTURE9		m_pTexBrightPass;	// �ֵ� ����
	PDIRECT3DSURFACE9		m_pSurfBrightPass;
	PDIRECT3DTEXTURE9		m_pTexStarSource;   // ���� ������ �� �̹���
	PDIRECT3DSURFACE9		m_pSurfStarSource;
	PDIRECT3DTEXTURE9		m_apTexStar[NUM_STAR_TEXTURES];// �ӽ�
	PDIRECT3DSURFACE9		m_apSurfStar[NUM_STAR_TEXTURES];

	// ��鷻����
	LPDIRECT3DTEXTURE9		m_pNormalMap;	// ������
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// ��������
	
	// ���̴�
	LPD3DXEFFECT		    m_pEffect;		// ����Ʈ
	D3DXHANDLE				m_hTechnique;	// ��ũ��
	D3DXHANDLE				m_hmWVP;		// ����-���� ��ȯ���
	D3DXHANDLE				m_hvLightDir;	// ��������
	D3DXHANDLE				m_hvColor;		// ������
	D3DXHANDLE				m_hvEyePos;		// ����
	D3DXHANDLE				m_htDecaleTex;	// �����ؽ�ó
	D3DXHANDLE				m_htNormalMap;	// ������


	// ��ȯ���
	D3DXVECTOR4				m_vFromPt;

	BOOL					m_bLoadingApp;	// �ε���?
	
	FLOAT                   m_fWorldRotX;   // X��ȸ��
	FLOAT                   m_fWorldRotY;   // Y��ȸ��
	FLOAT                   m_fViewZoom;    // �����Ÿ�

	D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer
};
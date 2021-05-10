#pragma once

class F3dCTail
{
public:
	enum {
		MAX_VERTEX = 1000,
	};
	F3dCTail();
	~F3dCTail();

	void Init( float fWidth );
	void Finish();
	void AddTail( double dTime, const F3dVertex& vtxV );
	void AddVertexLine( double dTime, F3dVertex* vtxV );
	void MakeMesh();
	void Process( double dTime, double dt );
	void Render( double dTime, int nLayer );
	LPDIRECT3DVERTEXBUFFER9	GetVertexBuffer() const { return m_pVB; }
	int GetVertexNum() const { return m_nVertexNum; }
	void SetTailLengthTime( double dTailLengthTime ) { m_dTailLengthTime = dTailLengthTime; }
	double GetTailLengthTime() const					{ return m_dTailLengthTime; }
	void SetTexture( const WCHAR* szTextureName );

private:
	struct F3dVertexFace
	{
		F3dTexVertex vtxV[3];
	};

	std::vector< F3dVertexLine >	m_vtxControlPointLine;
	std::vector< F3dVertexLine >	m_vtxLine;

	float m_fWidth;
	bool m_bRender;
	int m_nVertexNum;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	double m_dTailLengthTime;
	double m_dOldTailTime;
	F3dVertex m_vtxPreVertex;
	bool m_bFirst;
	UINT64 m_TextureID;
	LPDIRECT3DTEXTURE9 m_pTexture;
};

typedef boost::shared_ptr< F3dCTail > F3dCTailPtr;
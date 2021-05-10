
#pragma once

class ARGBBuffer;

class F3dZUIFreeType : public FBSingleton< F3dZUIFreeType >
{
public:
	F3dZUIFreeType();
	~F3dZUIFreeType();

	bool IsInitialized() const;

	bool Init();
	bool DeInit();

	bool LoadFontBuffer( const WCHAR* pAlias, const void* pBuf, size_t nBufSize );
	bool LoadFont( const WCHAR* pAlias, const char* pFullPath );

	enum FONT_EFFECT_TYPE
	{
		FT_PLAIN			= 1<<0,
		FT_BOLD				= 1<<1,
		FT_GLOW				= 1<<2,
		FT_SHADOW			= 1<<3,
		FT_OUTLINE			= 1<<4,
	};

	int get_x_margin( int nFontSize, int ft );
	int get_y_margin( int nFontSize, int ft );

	const ARGBBuffer* Draw( const WCHAR *szAlias, const WCHAR* wszString, DWORD* pWidth, DWORD* pHeight, int nFontSize = 11, int ft = FT_PLAIN, BOOL bUnderline = FALSE, BOOL bBackGround = FALSE )
	{
		size_t len = wcslen( wszString );
		if( !len ) return NULL;

		return Draw( szAlias, wszString, len, pWidth, pHeight, nFontSize, ft, bUnderline, bBackGround );
	}

	const ARGBBuffer* Draw( const WCHAR *szAlias, const WCHAR * wszString, size_t len, DWORD * pWidth, DWORD * pHeight, int nFontSize = 11, int ft = FT_PLAIN, BOOL bUnderline = FALSE, BOOL bBackGround = FALSE );

	const BOOL GetSize( const WCHAR *szAlias, const WCHAR* wszString, DWORD* pWidth, DWORD* pHeight, int nFontSize, int ft );

	void SetColor  ( DWORD dwColor );
	void SetBGColor( DWORD dwColor );

	void* GetFreeTypeLib();

protected:
	class MFreeType;
	class MFreeType *m_pImplement;

private:

};

//////////////////////////////////////////////////////////////////////////
class ARGBBuffer
{
public:

	enum {
		B     = 256,
		DEPTH = 256,
		MAX_VALUE	  = (DEPTH-1),

		MAX_NEIGHBOR = 8,
	};

	enum ARGB_BUFFER_OP
	{
		OP_OUT_LINE   ,
		OP_GLOW       ,
		OP_SHADOW     ,
	};

	struct Pixel
	{
		Pixel() {}
		Pixel( F3dColor _c ) : c(_c) {}

		union {
			F3dColor c;
			struct {
				unsigned char b;
				unsigned char g;
				unsigned char r;
				unsigned char a;
			};
		};

		Pixel & operator+=( const Pixel & rh )
		{
			int nB = b + rh.b;
			int nG = g + rh.g;
			int nR = r + rh.r;
			int nA = a + rh.a;

			b = ( nB > MAX_VALUE ) ? MAX_VALUE : nB;
			g = ( nG > MAX_VALUE ) ? MAX_VALUE : nG;
			r = ( nR > MAX_VALUE ) ? MAX_VALUE : nR;
			a = ( nA > MAX_VALUE ) ? MAX_VALUE : nA;

			return *this;
		}
	};

	ARGBBuffer( size_t width, size_t height, F3dColor color=0 ) : m_nWidth(width), m_nHeight(height) {
		m_nBufferSize = width*height;
		m_pBuffer = new Pixel[ m_nBufferSize ];

		memset( m_pBuffer, 0, sizeof(Pixel)*m_nBufferSize );
	}

	ARGBBuffer & operator=( const ARGBBuffer & rh )
	{
		m_nWidth	= rh.m_nWidth;
		m_nHeight	= rh.m_nHeight;

		if( m_nBufferSize < rh.m_nBufferSize )
		{
			delete [] m_pBuffer;
			m_pBuffer = new Pixel[ rh.m_nBufferSize ];
		}

		memcpy( m_pBuffer, rh.m_pBuffer, rh.m_nBufferSize*sizeof(Pixel) ); 
		return *this;
	}

	virtual ~ARGBBuffer() {
		delete [] m_pBuffer;
	}

	void Clear()
	{
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				m_pBuffer[ x + y * m_nWidth ].r = 0;
				m_pBuffer[ x + y * m_nWidth ].g = 0;
				m_pBuffer[ x + y * m_nWidth ].b = 0;
				m_pBuffer[ x + y * m_nWidth ].a = 0;
			}
		}
	}

	void SetSize( size_t width, size_t height ) { 
		if( m_nBufferSize >= width*height ) { 
			m_nWidth = width; m_nHeight = height; 
		}
	}

	Pixel* get( const size_t & x, const size_t & y ) { assert( x < m_nWidth ); assert( y < m_nHeight ); return &m_pBuffer[ x + y * m_nWidth ]; }

	void ForceFillColor( unsigned char r, unsigned char g, unsigned char b, unsigned char alpha = 0 )
	{
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				m_pBuffer[ x + y * m_nWidth ].r = (unsigned char )( r * B / 256 );
				m_pBuffer[ x + y * m_nWidth ].g = (unsigned char )( g * B / 256 );
				m_pBuffer[ x + y * m_nWidth ].b = (unsigned char )( b * B / 256 );
				if( alpha ) 
					m_pBuffer[ x + y * m_nWidth ].a = (unsigned char )( alpha * B / 256 );
			}
		}
	}

	void FillOnlyColor( unsigned char r, unsigned char g, unsigned char b )
	{
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				m_pBuffer[ x + y * m_nWidth ].r = (unsigned char )( r * B / 256 );
				m_pBuffer[ x + y * m_nWidth ].g = (unsigned char )( g * B / 256 );
				m_pBuffer[ x + y * m_nWidth ].b = (unsigned char )( b * B / 256 );
			}
		}
	}

	void FillColor( unsigned char r, unsigned char g, unsigned char b, unsigned char alpha = 0 )
	{
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				if( m_pBuffer[ x + y * m_nWidth ].a == 0 ) {
					m_pBuffer[ x + y * m_nWidth ].r = 0;
					m_pBuffer[ x + y * m_nWidth ].g = 0;
					m_pBuffer[ x + y * m_nWidth ].b = 0;
				}
				else {
					m_pBuffer[ x + y * m_nWidth ].r = (unsigned char )( r * B / 256 );
					m_pBuffer[ x + y * m_nWidth ].g = (unsigned char )( g * B / 256 );
					m_pBuffer[ x + y * m_nWidth ].b = (unsigned char )( b * B / 256 );
					if( alpha ) 
						m_pBuffer[ x + y * m_nWidth ].a = (unsigned char )( alpha * B / 256 );
				}
			}
		}
	}

	void SetColor( const size_t & x, const size_t & y, const int & r, const int & g, const int & b )
	{
		Pixel *p = get( x, y );
		p->r = ( r * B / 256 );
		p->g = ( g * B / 256 );
		p->b = ( b * B / 256 );
	}

	void SetAlpha( const size_t & x, const size_t & y, const int & a )	{ get( x, y )->a = ( a * B / 256 ) ;	}

	const size_t GetWidth() const { return m_nWidth; }
	const size_t GetHeight() const { return m_nHeight; }

	const Pixel* GetBuffer() const { return m_pBuffer; }

	//È¿°ú

	Pixel& rget( const size_t & x, const size_t & y )
	{
		static Pixel dummy(0x00000000);
		if( x >= m_nWidth  || y >= m_nHeight ) return dummy;

		return m_pBuffer[ x + y * m_nWidth ];
	}

	void Inverse()
	{
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				m_pBuffer[ x + y * m_nWidth ].a = MAX_VALUE-m_pBuffer[ x + y * m_nWidth ].a;
				m_pBuffer[ x + y * m_nWidth ].r = MAX_VALUE-m_pBuffer[ x + y * m_nWidth ].r;
				m_pBuffer[ x + y * m_nWidth ].g = MAX_VALUE-m_pBuffer[ x + y * m_nWidth ].g;
				m_pBuffer[ x + y * m_nWidth ].b = MAX_VALUE-m_pBuffer[ x + y * m_nWidth ].b;
			}
		}
	}

	unsigned char GetAlphaAverage( const size_t & x, const size_t & y )
	{
		if( rget( x  , y   ).a == (MAX_VALUE) ) return (MAX_VALUE);

		int nA  = 0;
		nA  = rget( x-1, y-1 ).a; nA += rget( x  , y-1 ).a; nA += rget( x+1, y-1 ).a;
		nA += rget( x-1, y   ).a;							nA += rget( x+1, y   ).a; 
		nA += rget( x-1, y+1 ).a; nA += rget( x  , y+1 ).a; nA += rget( x+1, y+1 ).a; 
		
		nA = nA/MAX_NEIGHBOR;

		nA += rget( x  , y   ).a;

		if( nA > (MAX_VALUE) ) return (MAX_VALUE);
		return nA;
	}

	void IncAlpha( int nMinValue = 0 )
	{
		size_t size = m_nWidth*m_nHeight;
		Pixel *pTmpBuffer = new Pixel[ size ];
		memcpy( pTmpBuffer, m_pBuffer, sizeof(Pixel)*size );

		int nAvr = 0;
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				nAvr = GetAlphaAverage( x, y );
				pTmpBuffer[ x + y * m_nWidth ].a = ( nAvr > 0 && nAvr < nMinValue) ? nMinValue : nAvr;
			}
		}

		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				m_pBuffer[ x + y * m_nWidth ].a = pTmpBuffer[ x+y*m_nWidth].a;
			}
		}

		delete [] pTmpBuffer;
	}

	void ModulateAlpha( float f )
	{
		for( size_t x = 0; x < m_nWidth; x++ )
		{
			for( size_t y = 0; y < m_nHeight; y++ )
			{
				if( !m_pBuffer[ x + y * m_nWidth ].a ) continue;

				int i = (int)(m_pBuffer[ x + y * m_nWidth ].a * f);				
				if( i > MAX_VALUE ) i = MAX_VALUE;

				m_pBuffer[ x + y * m_nWidth ].a = i;
			}
		}
	}

	void Append( const ARGBBuffer & rh, int xm = 0, int ym = 0, ARGB_BUFFER_OP mode = OP_OUT_LINE )
	{
		for( UINT x = 0; x < m_nWidth; x++ )
		{
			for( UINT y = 0; y < m_nHeight; y++ )
			{
				if( x + xm < 0 || x + xm >= rh.m_nWidth  ) continue;
				if( y + ym < 0 || y + ym >= rh.m_nHeight ) continue;

				Pixel & pl = m_pBuffer[ x + y * m_nWidth ];
				Pixel   pr = rh.m_pBuffer[ x+xm + (y+ym) * m_nWidth ];

				if( mode == OP_OUT_LINE  ) {
					if( pl.a == 0 && pr.a > 0 ) {
						pl.a = pr.a;

					pl.r = pl.r + pr.r;
					pl.g = pl.g + pr.g;
					pl.b = pl.b + pr.b;
					}
				}
				else if( mode == OP_GLOW  ) {
					if( pl.a == (MAX_VALUE) || pr.a == 0 ) continue;	

					int a = pl.a + pr.a;
					if( a > MAX_VALUE ) {
						pr.a = MAX_VALUE - pl.a;
						a = MAX_VALUE;
					}

					int r = ( pl.r * pl.a ) + ( pr.r * pr.a );
					int g = ( pl.g * pl.a ) + ( pr.g * pr.a );
					int b = ( pl.b * pl.a ) + ( pr.b * pr.a );

					pl.a = a;
					pl.r = r / (pl.a+pr.a);
					pl.g = g / (pl.a+pr.a);
					pl.b = b / (pl.a+pr.a);
				}
				else if( mode == OP_SHADOW      ) { 
					if( pl.a == 0 ) {
						pl.a = pr.a;
						pl.r = pr.r; 
						pl.g = pr.g;
						pl.b = pr.b;
					}	
				}
			}
		}
	}

private:
	size_t m_nWidth;
	size_t m_nHeight;
	size_t m_nBufferSize;

	Pixel* m_pBuffer;
};
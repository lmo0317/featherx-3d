
#pragma once

class F3dZControlNumber : public F3dZControl
{
public:
	F3dZControlNumber();
	virtual ~F3dZControlNumber();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void		SetSize( const int& nWidth, const int& nHeight );

	void			SetCipher( USHORT uCipher );
	void			SetNumber( int nNumber );
	void			SetTimeMode( bool bTime ) { m_bTimeMode = bTime; }
	void			SetAllowOne( bool bFlag ) { m_bAllowOne = bFlag; }


protected:

	USHORT		m_uCipher;

	USHORT		m_uCount;
	float		m_fWidth;
	float		m_fDivWidth;

	int			m_nNumber;

	bool		m_bTimeMode;
	bool		m_bAllowOne;

	const F3dZUITexture* m_pTex;

	std::vector< F3dCUISpritePtr >*	m_vCache;

private:

};


//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlNumber : public F3dZEditableControl
{
public:
	F3dZEditableControlNumber();
	virtual ~F3dZEditableControlNumber() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

protected:
	double		m_dTime;
	USHORT		m_uCount;
	float		m_fWidth;
	float		m_fDivWidth;

private:

};
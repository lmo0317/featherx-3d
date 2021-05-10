

#pragma once

//////////////////////////////////////////////////////////////////////////
class F3dZControlAniStatic : public F3dZControl
{
public:
	F3dZControlAniStatic();
	virtual ~F3dZControlAniStatic();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );
	virtual void	RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	const   bool IsEnd() const;

	void	SetChangeTime( const double fValue );
	void    SetSkipNextFrame( bool bFlag );
	void    SetOneTimeRender( bool bFlag );
	void    SetLoop( bool bFlag );

	void	SetFirst();

protected:
	void setNextFrame();
	const int  getNextFrame();

private:
	double m_fStartTime;
	double m_fChangeTime;
	bool   m_bSkipNext;
	bool   m_bOneTimeRender;
	bool   m_bAutoNextFrame;
	bool   m_bLoop;
};

//////////////////////////////////////////////////////////////////////////
class F3dZControlAniDelayStatic : public F3dZControl
{
public:
	F3dZControlAniDelayStatic();
	virtual ~F3dZControlAniDelayStatic();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	const int		GetTextureWidth() const;
	const int		GetTextureHeight() const;

	void			SetPlayIndex( int nStartIndex, int nEndIndex );
	void			AddFrame( std::vector<const F3dZUISprite*>& vUISpriteList, std::vector<int>& vDelayList );
	void			ChangeFrame( std::vector<const F3dZUISprite*>& vUISpriteList, std::vector<int>& vDelayList );

	void			SetLoop( bool bLoop ) { m_bLoop = bLoop; }
	const bool		IsLoop() const		  { return m_bLoop; }

protected:

private:

	bool				m_bLoop;

	std::vector<int>	m_vDelay;
	int					m_nCurIndex;
	int					m_nFrameCount;

	int					m_nPlayStartIndex;
	int					m_nPlayEndIndex;

	int					m_nTextureWidth;
	int					m_nTextureHeight;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlAniStatic : public F3dZEditableControl
{
public:
	F3dZEditableControlAniStatic() {}
	virtual ~F3dZEditableControlAniStatic() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

protected:

private:

};
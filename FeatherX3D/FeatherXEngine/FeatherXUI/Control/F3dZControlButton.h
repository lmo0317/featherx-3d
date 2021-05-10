
#pragma once

//////////////////////////////////////////////////////////////////////////
//F3dZControlButton
class F3dZControlButton : public F3dZControl
{
public:
	F3dZControlButton();
	virtual ~F3dZControlButton();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer );



protected:

private:

};

//////////////////////////////////////////////////////////////////////////
//F3dZControlPushButton
class F3dZControlPushButton : public F3dZControlButton
{
public:
	F3dZControlPushButton();
	virtual ~F3dZControlPushButton();

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	void SetDown( BOOL bFlag );
	const BOOL IsDown() const { return m_bIsDown; }

	void SetSkipOver( BOOL bFlag ) { m_bSkipOver = bFlag; }
	const BOOL IsSkipOver() { return m_bSkipOver; }

protected:
	BOOL		m_bIsDown;
	BOOL		m_bSkipOver;
private:
};

//////////////////////////////////////////////////////////////////////////
//F3dZControlCheckButton
class F3dZControlCheckButton : public F3dZControlPushButton
{
public:
	F3dZControlCheckButton();
	virtual ~F3dZControlCheckButton();

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

protected:

};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlButton : public F3dZEditableControl
{
public:
	F3dZEditableControlButton();
	virtual ~F3dZEditableControlButton();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );



protected:

private:

};
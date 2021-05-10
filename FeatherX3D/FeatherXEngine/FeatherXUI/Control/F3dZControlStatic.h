
#pragma once

class F3dZControlSimple : public F3dZControl
{
public:
	F3dZControlSimple();

	void			SetInfo( const int in_x, const int in_y, const int out_x, const int out_y );

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

private:
	F3dVector2		m_vStart2d, m_vEnd2d;

	double			m_dCheckTime;
	int				m_nStep;
	enum
	{
		STEP_NONE,
		STEP_INIT,
		STEP_ING,
		STEP_END,
	};
};

//////////////////////////////////////////////////////////////////////////
class F3dZControlStatic : public F3dZControl
{
public:
	F3dZControlStatic();
	virtual ~F3dZControlStatic();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer );

	void			SetOver( bool bFlag );
	void			SetOverBlink( bool bFlag );

protected:
	bool			m_bOverRender;
	bool			m_bOverBlink;

	double			m_dBlinkTime;

private:

};

//////////////////////////////////////////////////////////////////////////
class F3dZControlStaticMultiLine : public F3dZControlStatic
{
public:
	F3dZControlStaticMultiLine();
	virtual ~F3dZControlStaticMultiLine();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Render( double dTime );

	virtual void	SetRenderTopLevel( bool bFlag );
	virtual void	SetCaption( const WCHAR* pStr, BOOL bNew=FALSE );
	virtual void	MovePos( int x, int y );
	virtual void	MovePosOffset( int offsetx, int offsety );
	virtual void	TextMove();

	virtual void	SetAlpha( float fRatio );
	virtual void	SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight);

	void			SetOffSetY( int nOffSetY );
	void			SetLineHeight( int nValue );

	virtual int  GetLineCount() { return m_vTextList.size(); } 

protected:

	void			_setText( MCParser::TEXT_DATA* pData, const F3dColor& textColor, int& nLine, DWORD& curWidth );

	int	m_nOffSetY;
	int m_nMultiLineDefHeight;

	std::vector< F3dZUITextPtr >	 m_vTextList;
	std::vector<MCParser::TEXT_DATA> m_vTextDataList;

private:

};


//////////////////////////////////////////////////////////////////////////
class F3dZControlRollStatic : public F3dZControl
{
public:
	F3dZControlRollStatic();
	virtual ~F3dZControlRollStatic();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );
	virtual void	SetCaption( const WCHAR* pStr, BOOL bNew=FALSE );

private:

	bool		m_bRoll;

	double		m_dRollTime;
	DWORD		m_dwTextWidth;
	DWORD		m_dwTextHeight;
};

//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlStatic : public F3dZEditableControl
{
public:
	F3dZEditableControlStatic() {}
	virtual ~F3dZEditableControlStatic() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

protected:

private:

};
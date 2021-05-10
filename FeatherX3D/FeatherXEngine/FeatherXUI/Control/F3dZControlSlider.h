
#pragma once

class F3dZControlSliderBase : public F3dZControl
{
public:
	F3dZControlSliderBase();
	virtual ~F3dZControlSliderBase();

	void SetRange( int nMinPos, int nMaxPos );
	void SetPos( int nPos );

	int GetPos( int& nPos );
	void GetRange( int& nMinPos, int& nMaxPos );

	virtual void	onEnable( const BOOL bFlag );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	virtual void _EvaluationPosition(int x, int y) = 0;
	virtual void _EvaluationPositionFromThumRect() = 0;
	virtual void _EvaluationThumbRect() = 0;
	virtual void _SetThumbRect(int x, int y) = 0;

protected:
	BOOL m_bMouseDrag;

	int m_nCurrentPos;

	int	m_nMinPos;
	int	m_nMaxPos;

	enum
	{
		_BACK   = 0,
		_THUMB  = 1,

		INDEX_MAX = 2,
	};

	F3dZControl* m_pCtrl[INDEX_MAX];

	POINT	m_ptMovingOffset;
};

//////////////////////////////////////////////////////////////////////////
//VSlider
class F3dZControlVSlider : public F3dZControlSliderBase
{
public:
	F3dZControlVSlider();
	virtual ~F3dZControlVSlider();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	SetSize( const int& nWidth, const int& nHeight );

	virtual void _EvaluationPosition(int x, int y);
	virtual void _EvaluationPositionFromThumRect();
	virtual void _EvaluationThumbRect();
	virtual void _SetThumbRect(int x, int y);

protected:

	void createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	UI_RECT	m_Thumb_rect;

private:

};

//////////////////////////////////////////////////////////////////////////
//HSlider
class F3dZControlHSlider : public F3dZControlSliderBase
{
public:
	F3dZControlHSlider();
	virtual ~F3dZControlHSlider();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );
	
	virtual void	SetSize( const int& nWidth, const int& nHeight );

	virtual void _EvaluationPosition(int x, int y);
	virtual void _EvaluationPositionFromThumRect();
	virtual void _EvaluationThumbRect();
	virtual void _SetThumbRect(int x, int y);


protected:

	void createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	UI_RECT	m_Thumb_rect;

private:

};

//------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlVSlider : public F3dZEditableControl
{
public:
	F3dZEditableControlVSlider() {}
	virtual ~F3dZEditableControlVSlider() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	virtual void	SetSize( const int& nWidth, const int& nHeight );

protected:

	void createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	UI_RECT	m_Thumb_rect;
};

//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlHSlider : public F3dZEditableControl
{
public:
	F3dZEditableControlHSlider() {}
	virtual ~F3dZEditableControlHSlider() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	virtual void	SetSize( const int& nWidth, const int& nHeight );

protected:

	void createBackGround( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	UI_RECT	m_Thumb_rect;

private:

};
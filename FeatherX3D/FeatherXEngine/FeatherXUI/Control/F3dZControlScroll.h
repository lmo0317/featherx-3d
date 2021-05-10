
#pragma once

class F3dZControlScrollBase : public F3dZControl
{
public:
	F3dZControlScrollBase();
	virtual ~F3dZControlScrollBase();

	void SetScrollRange( int nMinPos, int nMaxPos );
	void SetScrollPos( int nPos );

	int GetScrollPos( int& nPos );
	void GetScrollRange( int& nMinPos, int& nMaxPos );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	virtual void _EvaluationPosition(int x, int y) = 0;
	virtual void _EvaluationPositionFromThumRect() = 0;
	virtual void _EvaluationThumbRect() = 0;
	virtual void _SetThumbRect(int x, int y) = 0;
	virtual void _ResizeThumb() = 0;

protected:
	BOOL m_bMouseDrag;

	int m_nCurrentPos;

	int	m_nMinPos;
	int	m_nMaxPos;

	enum
	{

	};

	enum
	{
		_TOP    = 0,
		_BOTTOM = 1,
		_THUMB  = 2,

		_LEFT   = 0,
		_RIGHT  = 1,

		INDEX_MAX = 3,
	};

	F3dZControl* m_pCtrl[INDEX_MAX];

	POINT	m_ptMovingOffset;
};

//////////////////////////////////////////////////////////////////////////
//VScroll
class F3dZControlVScroll : public F3dZControlScrollBase
{
public:
	F3dZControlVScroll();
	virtual ~F3dZControlVScroll();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );
	virtual void	SetSize( const int& nWidth, const int& nHeight );

	virtual void _EvaluationPosition(int x, int y);
	virtual void _EvaluationPositionFromThumRect();
	virtual void _EvaluationThumbRect();
	virtual void _SetThumbRect(int x, int y);
	virtual void _ResizeThumb();
	
protected:
	void createButtonTop   ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonBottom( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	int	m_nMinThumbWidth;
};

//////////////////////////////////////////////////////////////////////////
//HScroll
class F3dZControlHScroll : public F3dZControlScrollBase
{
public:
	F3dZControlHScroll();
	virtual ~F3dZControlHScroll();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );
	virtual void	SetSize( const int& nWidth, const int& nHeight );

	virtual void _EvaluationPosition(int x, int y);
	virtual void _EvaluationPositionFromThumRect();
	virtual void _EvaluationThumbRect();
	virtual void _SetThumbRect(int x, int y);
	virtual void _ResizeThumb();

protected:

	void createButtonLeft ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonRight( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	int	m_nMinThumbWidth;

private:

};


//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlVScroll : public F3dZEditableControl
{
public:
	F3dZEditableControlVScroll() {}
	virtual ~F3dZEditableControlVScroll() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	virtual void	SetSize( const int& nWidth, const int& nHeight );

protected:

	void createButtonTop   ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonBottom( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	UI_RECT	m_Top_rect;
	UI_RECT	m_Thumb_rect;
	UI_RECT	m_Bottom_rect;
};

//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlHScroll : public F3dZEditableControl
{
public:
	F3dZEditableControlHScroll() {}
	virtual ~F3dZEditableControlHScroll() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	virtual void	SetSize( const int& nWidth, const int& nHeight );

protected:

	void createButtonLeft ( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonThumb( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButtonRight( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	UI_RECT	m_Left_rect;
	UI_RECT	m_Thumb_rect;
	UI_RECT	m_Right_rect;

private:

};
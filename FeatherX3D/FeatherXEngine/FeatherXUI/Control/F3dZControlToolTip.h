
#pragma once

//////////////////////////////////////////////////////////////////////////
class F3dZControlToolTip : public F3dZControl
{
public:
	F3dZControlToolTip();
	virtual ~F3dZControlToolTip();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );	

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );
	virtual void	MovePos( int x, int y );
	virtual void	TextMove();

	void LimitResolution( int nLeft=0, int nTop=0, int nRight=0, int nBottom=0 );

	virtual void ParseToolTip( const WCHAR* pStrToolTip );
	virtual void ParseToolTipEx( const WCHAR* pStrToolTip, const WCHAR* pStrToolTipTitle );
	virtual void ParseSubToolTipEx( const WCHAR* pStrToolTip, const WCHAR* pStrToolTipTitle );

	void	SetToolTipEnable( bool bFlag );
	const bool IsToolTipEnable() const { return m_bToolTipEnable; }

	void	SetOwner( F3dZControl* pOwnerControl ) { m_pOwnerControl = pOwnerControl; }
	const F3dZControl* GetOwner() const { return m_pOwnerControl; }

	void   CheckOwnerWindowClose( const WCHAR* pID );

protected:

	F3dZControl*	m_pOwnerControl;

	double	m_dEnableTime;
	double	m_dCurTime;
	bool	m_bToolTipEnable;

	std::vector< F3dZUITextPtr > m_vTextList;
	std::vector< F3dZUITextPtr > m_vSubTextList;
};

typedef boost::shared_ptr< F3dZControlToolTip > F3dZControlToolTipPtr;


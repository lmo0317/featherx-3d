
#pragma once

namespace
{
	const int c_nGaugeSpriteCount = 2;
}

class F3dZControlGauge : public F3dZControl
{
public:
	F3dZControlGauge();
	virtual ~F3dZControlGauge();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	OnPosChange();

	void	SetValue( const double fValue );
	void	SetMaxValue( const double fValue );
	void	Reset(const double fValue);

protected:

	void	SetStep( short step );
	void	SetChange();

	enum
	{
		STEP_NONE,
		STEP_START,
		STEP_ING,
		STEP_WAIT,
		STEP_END,
	};

	short			m_nStep;

	bool			m_bDecMode;

	double			m_fMaxValue;
	double			m_fValue;
	double			m_fPrevValue;

	double			m_fGoal;
	double			m_fBegin;
	double			m_fGap;

	double			m_dCheckTime;
	double			m_dTime;

	UI_RECT			m_backRect;
	F3dCUISprite*	m_pSpr[c_nGaugeSpriteCount];

private:

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class F3dZControlGaugeNoBG : public F3dZControl
{
public:
	F3dZControlGaugeNoBG();
	virtual ~F3dZControlGaugeNoBG();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	void	SetValue( const double fValue );
	void	SetMaxValue( const double fValue );
	void	Reset(const double fValue);

protected:

	void	SetStep( short step );
	void	SetChange();
	void	SetSpr();

	enum
	{
		STEP_NONE,
		STEP_START,
		STEP_ING,
		STEP_END,
	};

	short			m_nStep;

	bool			m_bDecMode;

	double			m_fMaxValue;
	double			m_fValue;
	double			m_fPrevValue;

	double			m_fGoal;
	double			m_fBegin;
	double			m_fGap;

	double			m_dCheckTime;
	double			m_dTime;

	UI_RECT			m_backRect;
	F3dCUISprite*	m_pSpr[c_nGaugeSpriteCount];

private:

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlGauge : public F3dZEditableControl
{
public:
	F3dZEditableControlGauge();
	virtual ~F3dZEditableControlGauge();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );



protected:

private:

};
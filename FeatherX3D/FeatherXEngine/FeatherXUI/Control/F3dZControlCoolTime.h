

#pragma once


class F3dZControlCoolTime : public F3dZControl
{
public:
	F3dZControlCoolTime();
	virtual ~F3dZControlCoolTime();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	//CoolTime
	void			SetDegree( int nDegree );

protected:
	void SetDefCoolInfo();
};


//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlCoolTime : public F3dZEditableControl
{
public:
	F3dZEditableControlCoolTime() {}
	virtual ~F3dZEditableControlCoolTime() {}

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	virtual void	Render( double dTime );

protected:

private:

};
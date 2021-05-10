
#pragma once

class F3dZControlListBox : public F3dZControl
{
public:
	F3dZControlListBox();
	virtual ~F3dZControlListBox();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );


protected:

private:

};
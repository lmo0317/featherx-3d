#pragma once

class F3dZWnd; 
class F3dZControl;

class F3dZUIFactory : public FBSingleton< F3dZUIFactory >
{
public:
	F3dZUIFactory();
	~F3dZUIFactory();

	HRESULT Init();
	void    Finish();

	BOOL			DoParseWnd( const WCHAR* pFileName, WINDOW_ARG& window_arg );

	BOOL			CreateWnd( const WCHAR* pFileName, F3dZWnd* pWnd );
	BOOL			CreateWnd( WINDOW_ARG& window_arg, F3dZWnd* pWnd );

	F3dZControl*	CreateControl( const CONTROL_ARG& control_arg );

	F3dZControl*	createStatic (  );
	F3dZControl*	createButton (  );
	F3dZControl*	createAniDelayStatic(  );

protected:
	
	F3dZControl*	createStaticMultiLine(  );
	F3dZControl*	createStaticClock( );
	F3dZControl*	createStaticRoll( );

	F3dZControl*	createPushButton (  ) ;
	F3dZControl*	createCheckButton ( );

	F3dZControl*	createEdit   (  );
	F3dZControl*	createNumber (  );
	F3dZControl*	createCombo  (  );
	F3dZControl*	createVScroll(  );
	F3dZControl*	createHScroll(  );
	F3dZControl*	createListBox(  );
	F3dZControl*	createHSlider(  );
	F3dZControl*	createVSlider(  );
	F3dZControl*	createTab    (  );
	F3dZControl*	createGauge  (  );
	F3dZControl*	createGaugeNoBG();
	F3dZControl*	createAniStatic();

private:

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class F3dZEditableWnd; 
class F3dZEditableControl;

class F3dZEditableUIFactory : public FBSingleton< F3dZEditableUIFactory >
{
public:
	F3dZEditableUIFactory();
	~F3dZEditableUIFactory();

	HRESULT Init();
	void    Finish();

	BOOL			CreateWnd( const WCHAR* pFileName, F3dZEditableWnd* pWnd );
	F3dZEditableControl*	CreateControl( const CONTROL_ARG& control_arg );

	F3dZEditableControl*	createStatic (  );
	F3dZEditableControl*	createButton (  );

protected:

	F3dZEditableControl*	createPushButton (  ) ;
	F3dZEditableControl*	createCheckButton();

	F3dZEditableControl*	createEdit   (  );
	F3dZEditableControl*	createNumber (  );
	F3dZEditableControl*	createCombo  (  );
	F3dZEditableControl*	createVScroll(  );
	F3dZEditableControl*	createHScroll(  );
	F3dZEditableControl*	createListBox(  );
	F3dZEditableControl*	createHSlider(  );
	F3dZEditableControl*	createVSlider(  );
	F3dZEditableControl*	createTab    (  );
	F3dZEditableControl*	createGauge  (  );
	F3dZEditableControl*	createAniStatic();

private:

};
#include "FeatherXEnginePrecompile.h"
#include "FeatherXEnginePrecompile.h"

//////////////////////////////////////////////////////////////////////////
F3dZUIFactory::F3dZUIFactory()
{  
	F3dZControlEdit::InitEdit();
}

F3dZUIFactory::~F3dZUIFactory()
{  
	F3dZControlEdit::DeInitEdit();
}

HRESULT F3dZUIFactory::Init()
{  
	

	return S_OK;
}

void F3dZUIFactory::Finish()
{  
	
}

BOOL F3dZUIFactory::DoParseWnd( const WCHAR* pFileName, WINDOW_ARG& window_arg )
{  
	if( !F3dZUIParser::GetSingleton()->DoParseWindow( pFileName, window_arg ) )
		return FALSE;

	return TRUE;
}

BOOL F3dZUIFactory::CreateWnd( const WCHAR* pFileName, F3dZWnd* pWnd )
{  
	WINDOW_ARG window_arg;
	if( !F3dZUIParser::GetSingleton()->DoParseWindow( pFileName, window_arg ) )
		return FALSE;

	pWnd->CreateWnd( window_arg );

	int nControlCount = (int)window_arg.vControlList.size();
	for( int i(0); nControlCount>i; i++ )
	{
		F3dZControl* pControl = CreateControl( window_arg.vControlList[i] );
		if( pControl )
			pWnd->AddControl( pControl );
	}

	pWnd->MovePos( window_arg.window_info.x, window_arg.window_info.y );

	return TRUE;
}

BOOL F3dZUIFactory::CreateWnd( WINDOW_ARG& window_arg, F3dZWnd* pWnd )
{  
	pWnd->CreateWnd( window_arg );

	int nControlCount = (int)window_arg.vControlList.size();
	for( int i(0); nControlCount>i; i++ )
	{
		F3dZControl* pControl = CreateControl( window_arg.vControlList[i] );
		if( pControl )
			pWnd->AddControl( pControl );
	}

	pWnd->MovePos( window_arg.window_info.x, window_arg.window_info.y );

	return TRUE;
}

F3dZControl* F3dZUIFactory::CreateControl( const CONTROL_ARG& control_arg )
{  
	const F3dZUISprite* pSprite = F3dZUISpriteManager::GetSingleton()->GetSprite( control_arg.strAni.c_str() );

	F3dZControl* pControl = NULL;

	if( NULL == pSprite ) { pControl = createStatic (  ); goto CONTROL_NSTEP; } //9, 3, 1

	if( !_wcsicmp( pSprite->strType.c_str(), L"static"    ) ) { pControl = createStatic (    ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"button"    ) ) { pControl = createButton (    ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"edit"      ) ) { pControl = createEdit   (    ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"number"    ) ) { pControl = createNumber (    ); goto CONTROL_NSTEP; } //1
	if( !_wcsicmp( pSprite->strType.c_str(), L"gauge"     ) ) { pControl = createGauge  (    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"v_scroll"  ) ) { pControl = createVScroll(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"gauge_nobg") ) { pControl = createGaugeNoBG(    ); goto CONTROL_NSTEP; } //custom
	
	if( !_wcsicmp( pSprite->strType.c_str(), L"static_multiline"    ) ) { pControl = createStaticMultiLine(    ); goto CONTROL_NSTEP; }  //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"static_clock"		) ) { pControl = createStaticClock    (    ); goto CONTROL_NSTEP; }
	if( !_wcsicmp( pSprite->strType.c_str(), L"static_roll"		    ) ) { pControl = createStaticRoll     (    ); goto CONTROL_NSTEP; }

	if( !_wcsicmp( pSprite->strType.c_str(), L"combo"     ) ) { pControl = createCombo  (    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"h_scroll"  ) ) { pControl = createHScroll(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"listbox"   ) ) { pControl = createListBox(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"v_slider"  ) ) { pControl = createVSlider(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"h_slider"  ) ) { pControl = createHSlider(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"tab"       ) ) { pControl = createTab    (    ); goto CONTROL_NSTEP; } //custom

	if( !_wcsicmp( pSprite->strType.c_str(), L"anistatic"  ) ) { pControl = createAniStatic(  ); goto CONTROL_NSTEP; } //1
	if( !_wcsicmp( pSprite->strType.c_str(), L"pushbutton" ) ) { pControl = createPushButton( ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"checkbutton") ) { pControl = createCheckButton( ); goto CONTROL_NSTEP; } //9, 3, 1

CONTROL_NSTEP:
	if( pControl )
	{
		pControl->CreateControl( pSprite, control_arg );

		return pControl;
	}

	return NULL;
}

F3dZControl* F3dZUIFactory::createStatic (  ) 
{ 
	F3dZControlStatic* pControl = new F3dZControlStatic;
	if( pControl ) return pControl;

	return NULL;
}

F3dZControl* F3dZUIFactory::createStaticMultiLine (  ) 
{ 
	F3dZControlStaticMultiLine* pControl = new F3dZControlStaticMultiLine;
	if( pControl ) return pControl;

	return NULL;
}

F3dZControl* F3dZUIFactory::createStaticClock( )
{ 
	 
	F3dZControlCoolTime* pControl = new F3dZControlCoolTime;
	if( pControl ) return pControl;

	return NULL;
}

F3dZControl* F3dZUIFactory::createStaticRoll( )
{
	 
	F3dZControlRollStatic* pControl = new F3dZControlRollStatic;
	if( pControl ) return pControl;

	return NULL;
}

F3dZControl* F3dZUIFactory::createAniDelayStatic(  )
{
	F3dZControlAniDelayStatic* pControl = new F3dZControlAniDelayStatic;
	if( pControl )	return pControl;

	return NULL; 	
}

F3dZControl* F3dZUIFactory::createButton (  ) 
{ 
	F3dZControlButton* pControl = new F3dZControlButton;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createPushButton (  ) 
{
	F3dZControlPushButton* pControl = new F3dZControlPushButton;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createCheckButton ( )
{  
	F3dZControlCheckButton* pControl = new F3dZControlCheckButton;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl*	F3dZUIFactory::createEdit   (  )
{  
	F3dZControlEdit* pControl = new F3dZControlEdit;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZControl* F3dZUIFactory::createNumber (  )
{  
	F3dZControlNumber* pControl = new F3dZControlNumber;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZControl* F3dZUIFactory::createCombo  (  ) 
{ 
	//사각 BG(3 조각 1셋트)	//버튼(1조작 4셋트)	//리스트 BG (9조각)	//리스트 셀렉트(1조각)

	F3dZControlCombo* pControl = new F3dZControlCombo;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createVScroll(  ) 
{ 
	//스크롤 BG	//스크롤 버튼	//스크롤 버튼 상	//스크롤 버튼 하
	F3dZControlVScroll* pControl = new F3dZControlVScroll;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createHScroll(  ) 
{ 
	//스크롤 BG	//스크롤 버튼 	//스크롤 버튼 좌	//스크롤 버튼 우
	F3dZControlHScroll* pControl = new F3dZControlHScroll;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createListBox(  ) 
{ 
	//리스트 BG	//스크롤 BG	//스크롤 버튼 	//스크롤 버튼 상	//스크롤 버튼 하
	F3dZControlListBox* pControl = new F3dZControlListBox;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createHSlider(  ) 
{ 
	//슬라이더 BG(상하)	//슬라이더 버튼
	F3dZControlHSlider* pControl = new F3dZControlHSlider;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createVSlider(  ) 
{ 
	//슬라이더 BG(좌우)	//슬라이더 버튼
	F3dZControlVSlider* pControl = new F3dZControlVSlider;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createTab    (  ) 
{ 
	//탭버튼
	F3dZControlTab* pControl = new F3dZControlTab;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createGauge  (  ) 
{ 
	//BG	//Gauge Color //Effect
	F3dZControlGauge* pControl = new F3dZControlGauge;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createGaugeNoBG()
{  
	//Gauge Color //Effect
	F3dZControlGaugeNoBG* pControl = new F3dZControlGaugeNoBG;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZControl* F3dZUIFactory::createAniStatic()
{  
	F3dZControlAniStatic* pControl = new F3dZControlAniStatic;
	if( pControl )	return pControl;

	return NULL; 
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

F3dZEditableUIFactory::F3dZEditableUIFactory()
{  

}

F3dZEditableUIFactory::~F3dZEditableUIFactory()
{  

}

HRESULT F3dZEditableUIFactory::Init()
{  
	F3dZEditableControlEdit::InitEdit();

	return S_OK;
}

void    F3dZEditableUIFactory::Finish()
{  
	F3dZEditableControlEdit::DeInitEdit();
}

BOOL F3dZEditableUIFactory::CreateWnd( const WCHAR* pFileName, F3dZEditableWnd* pWnd )
{  
	WINDOW_ARG window_arg;
	if( !F3dZUIParser::GetSingleton()->DoParseWindow( pFileName, window_arg ) )
		return FALSE;

	pWnd->CreateWnd( window_arg );

	int nControlCount = (int)window_arg.vControlList.size();
	for( int i(0); nControlCount>i; i++ )
	{
		F3dZEditableControl* pControl = CreateControl( window_arg.vControlList[i] );
		if( pControl )
			pWnd->AddControl( pControl );
	}

	pWnd->MovePos( window_arg.window_info.x, window_arg.window_info.y );

	return TRUE;
}

F3dZEditableControl* F3dZEditableUIFactory::CreateControl( const CONTROL_ARG& control_arg )
{  
	const F3dZUISprite* pSprite = F3dZUISpriteManager::GetSingleton()->GetSprite( control_arg.strAni.c_str() );
	//if( NULL == pSprite )	{	
	//	return NULL;
	//}

	F3dZEditableControl* pControl = NULL;

	//Static 은 NULL 가능
	if( NULL == pSprite ) { pControl = createStatic (  ); goto CONTROL_NSTEP; } //9, 3, 1

	if( !_wcsicmp( pSprite->strType.c_str(), L"static"    ) ) { pControl = createStatic (    ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"button"    ) ) { pControl = createButton (    ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"edit"      ) ) { pControl = createEdit   (    ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"number"    ) ) { pControl = createNumber (    ); goto CONTROL_NSTEP; } //1
	if( !_wcsicmp( pSprite->strType.c_str(), L"gauge"     ) ) { pControl = createGauge  (    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"gauge_nobg") ) { pControl = createGauge  (    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"v_scroll"  ) ) { pControl = createVScroll(    ); goto CONTROL_NSTEP; } //custom

	if( !_wcsicmp( pSprite->strType.c_str(), L"static_multiline"    ) ) { pControl = createStatic(    ); goto CONTROL_NSTEP; }  //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"static_clock"		) ) { pControl = createStatic(    ); goto CONTROL_NSTEP; }
	if( !_wcsicmp( pSprite->strType.c_str(), L"static_roll"		    ) ) { pControl = createStatic(    ); goto CONTROL_NSTEP; }

	if( !_wcsicmp( pSprite->strType.c_str(), L"combo"     ) ) { pControl = createCombo  (    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"h_scroll"  ) ) { pControl = createHScroll(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"listbox"   ) ) { pControl = createListBox(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"v_slider"  ) ) { pControl = createVSlider(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"h_slider"  ) ) { pControl = createHSlider(    ); goto CONTROL_NSTEP; } //custom
	if( !_wcsicmp( pSprite->strType.c_str(), L"tab"       ) ) { pControl = createTab    (    ); goto CONTROL_NSTEP; } //custom

	if( !_wcsicmp( pSprite->strType.c_str(), L"anistatic" ) ) { pControl  = createAniStatic(  ); goto CONTROL_NSTEP; } //1
	if( !_wcsicmp( pSprite->strType.c_str(), L"pushbutton") ) { pControl  = createPushButton( ); goto CONTROL_NSTEP; } //9, 3, 1
	if( !_wcsicmp( pSprite->strType.c_str(), L"checkbutton") ) { pControl = createCheckButton( ); goto CONTROL_NSTEP; } //9, 3, 1

CONTROL_NSTEP:
	if( pControl )
	{
		pControl->CreateControl( pSprite, control_arg );
		return pControl;
	}

	return NULL;
}

F3dZEditableControl*	F3dZEditableUIFactory::createStatic (  ) 
{ 
	F3dZEditableControlStatic* pControl = new F3dZEditableControlStatic;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createButton (  ) 
{ 
	F3dZEditableControlButton* pControl = new F3dZEditableControlButton;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl* F3dZEditableUIFactory::createPushButton (  ) 
{
	F3dZEditableControlButton* pControl = new F3dZEditableControlButton;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZEditableControl* F3dZEditableUIFactory::createCheckButton()
{  
	F3dZEditableControlButton* pControl = new F3dZEditableControlButton;
	if( pControl )	return pControl;

	return NULL; 
}


F3dZEditableControl*	F3dZEditableUIFactory::createEdit   (  )
{  
	F3dZEditableControlEdit* pControl = new F3dZEditableControlEdit;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl* F3dZEditableUIFactory::createNumber (  )
{  
	F3dZEditableControlNumber* pControl = new F3dZEditableControlNumber;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createCombo  (  ) 
{ 
	F3dZEditableControlCombo* pControl = new F3dZEditableControlCombo;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createVScroll(  ) 
{ 
	F3dZEditableControlVScroll* pControl = new F3dZEditableControlVScroll;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createHScroll(  ) 
{ 
	F3dZEditableControlHScroll* pControl = new F3dZEditableControlHScroll;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createListBox(  ) 
{ 
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createHSlider(  ) 
{ 
	F3dZEditableControlHSlider* pControl = new F3dZEditableControlHSlider;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createVSlider(  ) 
{ 
	F3dZEditableControlVSlider* pControl = new F3dZEditableControlVSlider;
	if( pControl ) return pControl;
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createTab    (  ) 
{ 
	return NULL; 
}

F3dZEditableControl*	F3dZEditableUIFactory::createGauge  (  ) 
{ 
	F3dZEditableControlGauge* pControl = new F3dZEditableControlGauge;
	if( pControl )	return pControl;

	return NULL; 
}

F3dZEditableControl* F3dZEditableUIFactory::createAniStatic()
{  
	F3dZEditableControlAniStatic* pControl = new F3dZEditableControlAniStatic;
	if( pControl )	return pControl;

	return NULL; 
}
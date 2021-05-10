#include "FeatherXEnginePrecompile.h"
 

namespace
{
	const int	 c_nMaxValue = 100;

	const double c_fGaugeEffectTime = 0.5f; 
	const double c_fGaugeEffectWaitTime = 0.2f;

	enum
	{
		GAUGE_BG = 0,
		GAUGE_FX,
	};
};

F3dZControlGauge::F3dZControlGauge() :
m_fMaxValue(100.f),
m_fValue(-1.f),
m_fPrevValue(0.f),
m_dCheckTime(0.f),
m_fGoal(1.f),
m_fGap(0.f)
{  
	for( int i(0); c_nGaugeSpriteCount>i; ++i )
		m_pSpr[i] = NULL; 	

	SetStep( STEP_NONE );
}

F3dZControlGauge::~F3dZControlGauge()
{  

}

void F3dZControlGauge::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount < 0 ) return;

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	int l, t, r, b;

	for( int i(0); m_nSetCount>i; ++i )
	{
		for( int n(0); nPieceCount>n; ++n ) 
		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			l = 0;
			t = 0;
			r = l + control_arg.nWidth;
			b = t + control_arg.nHeight;

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

			if( !pSprite->vTexList[i].empty() )
			{
				const F3dZUITexture* pTex = pSprite->vTexList[i][n];
				pRenderSprite->TextureID   = pTex->TextureID;
			}

			m_vSprite[i].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );

	SetMaxValue( c_nMaxValue );

	SetChange();
}

void F3dZControlGauge::	OnPosChange()
{  
	F3dZControl::OnPosChange();

	if( m_fValue < 0.f || m_fPrevValue == m_fValue ) return;

	double fLeft, fRight;
	if( m_fPrevValue > m_fValue ) { //감소
		fLeft  =     m_fValue/m_fMaxValue;
		fRight = m_fPrevValue/m_fMaxValue;
		
	}
	else { //증가
		fLeft  = m_fPrevValue/m_fMaxValue;
		fRight =     m_fValue/m_fMaxValue;
	}

	if( m_pSpr[GAUGE_FX] ) {
		m_pSpr[GAUGE_FX]->SetLeftRatio ( GetRect(), fLeft  );
		m_pSpr[GAUGE_FX]->SetRightRatio( GetRect(), fRight );
	}

	double fRatio;
	double dValue = m_dTime-m_dCheckTime;
	if( c_fGaugeEffectTime <= dValue )	{
		if( m_bDecMode ) 
			fRatio = m_fGoal;
		else 
			fRatio = m_fGoal+m_fGap;
	}
	else	{
		if( m_bDecMode ) 
			fRatio = m_fGoal+m_fGap*(c_fGaugeEffectTime-dValue);//감소
		else 
			fRatio = m_fGoal+m_fGap*dValue;//증가
	}

	if( m_pSpr[GAUGE_BG] ) m_pSpr[GAUGE_BG]->SetWidthRatio( fRatio );
}

void F3dZControlGauge::Process( double dTime, double dMeanDT )
{  
	m_dTime = dTime;
	if( !IsShow() ) return;

	switch( m_nStep )
	{
	case STEP_START : 
		{
			//효과 부분 크기 조정
			double fLeft, fRight;
			if( m_fPrevValue > m_fValue ) { //감소
				m_bDecMode = true;
				fLeft  =     m_fValue/m_fMaxValue;
				fRight = m_fPrevValue/m_fMaxValue;

				m_fGoal  = m_fValue    /m_fMaxValue;
				m_fBegin = m_fPrevValue/m_fMaxValue;
			}
			else { //증가
				m_bDecMode = false;
				fLeft  = m_fPrevValue/m_fMaxValue;
				fRight =     m_fValue/m_fMaxValue;

				//게이지 목표치 설정
				m_fBegin = m_fValue    /m_fMaxValue;
				m_fGoal  = m_fPrevValue/m_fMaxValue;
			}

			if( m_pSpr[GAUGE_FX] ) {
				m_pSpr[GAUGE_FX]->SetLeftRatio ( GetRect(), fLeft  );
				m_pSpr[GAUGE_FX]->SetRightRatio( GetRect(), fRight );
				m_pSpr[GAUGE_FX]->SetAlpha( 255 );
			}

			m_fGap       = m_fBegin - m_fGoal;
			m_dCheckTime = dTime;

			SetStep( STEP_WAIT );
		}
		break;

	case STEP_WAIT  : 
		{
			double dValue = m_dTime-m_dCheckTime;
			if( c_fGaugeEffectWaitTime <= dValue )	{
				m_dCheckTime = dTime;
				SetStep( STEP_ING );
			}
		}
		break;

	case STEP_ING   : 
		{
			double fRatio;
			double dValue = m_dTime-m_dCheckTime;
			if( c_fGaugeEffectTime <= dValue )	{
				if( m_bDecMode ) 
					fRatio = m_fGoal;
				else 
					fRatio = m_fGoal+m_fGap;

				if( m_pSpr[GAUGE_FX] ) { m_pSpr[GAUGE_FX]->SetAlpha( 0 ); m_pSpr[GAUGE_FX]->bShow = false; }
				SetStep( STEP_END );
			}
			else	{
				if( m_bDecMode ) 
					fRatio = m_fGoal+m_fGap*(c_fGaugeEffectTime-dValue);//감소
				else 
					fRatio = m_fGoal+m_fGap*dValue;//증가

				if( m_pSpr[GAUGE_FX] ) m_pSpr[GAUGE_FX]->SetAlpha( (int)(255*(c_fGaugeEffectTime-dValue)) );     //효과 점차 페이드 인
			}

			if( m_pSpr[GAUGE_BG] ) m_pSpr[GAUGE_BG]->SetWidthRatio( fRatio );
		}
		break; 
	case STEP_END   : 
		{
#ifdef _TEST_GAUGE_
			//static int nSel = 0;
			//float nArr[] = { 80.f, 60.f, 50.f, 30.f, 10.f };
			//nSel++;
			//if( nSel >= 5 ) nSel=0;
			//SetValue( nArr[nSel] );
#endif
		}
		break;
	}
}

void F3dZControlGauge::Render( double dTime )
{  
	//TODO
	if( IsShow() == FALSE ) return;

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
		{	
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}

	renderCaption(dTime);
	renderWire();
}

void F3dZControlGauge::SetStep( short step )
{  
	m_nStep = step;
}

void F3dZControlGauge::Reset(const double fValue)
{
	

	m_fPrevValue = fValue;
	m_fValue     = fValue;
	m_fMaxValue  = fValue;

	if( m_pSpr[GAUGE_FX] )
	{
		m_pSpr[GAUGE_FX]->SetAlpha( 0 ); 
		m_pSpr[GAUGE_FX]->bShow = true; 
	}

	SetChange();
}

void F3dZControlGauge::SetValue( const double fValue )
{ 
	 
	if( m_fValue == fValue ) 
		return;

	if( m_fValue < 0.f ) { //처음 셋팅
		m_fPrevValue = fValue;
		m_fValue     = fValue;
	}
	else {
		m_fPrevValue = m_fValue;
		m_fValue     = fValue;
		SetStep( STEP_START );
	}

	if( m_pSpr[GAUGE_FX] ) { m_pSpr[GAUGE_FX]->SetAlpha( 0 ); m_pSpr[GAUGE_FX]->bShow = true; }

	m_fPrevValue = std::max<double>( m_fPrevValue, 0. );
	m_fPrevValue = std::min<double>( m_fPrevValue, m_fMaxValue );

	m_fValue = std::max<double>( m_fValue, 0. );
	m_fValue = std::min<double>( m_fValue, m_fMaxValue );

	double fLevel = m_fValue/m_fMaxValue*100.f;
	
	//TODO Set Change
	if( fLevel >= 100.f )	{	}
	else if( fLevel >= 70.f && fLevel < 100.f ) {	}
	else if( fLevel >= 30.f && fLevel <  70.f ) {	}
	else {	}

	SetChange();
}

void F3dZControlGauge::SetMaxValue( const double fValue )
{  
	if( m_fMaxValue == fValue ) return;

	m_fMaxValue = fValue;
}

void F3dZControlGauge::SetChange()
{  
	int nCnt = 0;
	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ){
			if( nCnt >= c_nGaugeSpriteCount ) break;
			if( i != 0 )
				m_pSpr[nCnt++] = m_vSprite[m_nState][i].get();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

F3dZControlGaugeNoBG::F3dZControlGaugeNoBG() :
m_fMaxValue(100.f),
m_fValue(-1.f),
m_fPrevValue(0.f),
m_dCheckTime(0.f),
m_dTime(0.f),
m_fGoal(1.f),
m_fBegin(0.f),
m_fGap(0.f)
{  
	for( int i(0); c_nGaugeSpriteCount>i; ++i )
		m_pSpr[i] = NULL; 	

	SetStep( STEP_NONE );
}

F3dZControlGaugeNoBG::~F3dZControlGaugeNoBG()
{  

}

void F3dZControlGaugeNoBG::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount < 0 ) return;

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	int l, t, r, b;

	for( int i(0); m_nSetCount>i; ++i )
	{
		for( int n(0); nPieceCount>n; ++n ) 
		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			l = 0;
			t = 0;
			r = l + control_arg.nWidth;
			b = t + control_arg.nHeight;

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

			if( !pSprite->vTexList[i].empty() )
			{
				const F3dZUITexture* pTex = pSprite->vTexList[i][n];
				pRenderSprite->TextureID = pTex->TextureID;
			}

			m_vSprite[i].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );

	SetMaxValue( c_nMaxValue );

	SetChange();
}

void F3dZControlGaugeNoBG::Process( double dTime, double dMeanDT )
{  
	m_dTime = dTime;
	if( !IsShow() ) return;

	double fRatio;
	double dValue = m_dTime-m_dCheckTime;
	if( c_fGaugeEffectTime <= dValue )	{
		if( m_bDecMode ) 
			fRatio = m_fGoal;
		else 
			fRatio = m_fGoal+m_fGap;
		if( m_pSpr[GAUGE_FX] ) m_pSpr[GAUGE_FX]->SetAlpha( 0 );
		SetStep( STEP_END );
	}
	else	{
		if( m_bDecMode ) 
			fRatio = m_fGoal+m_fGap*(c_fGaugeEffectTime-dValue);//감소
		else 
			fRatio = m_fGoal+m_fGap*dValue;//증가
		if( m_pSpr[GAUGE_FX] ) m_pSpr[GAUGE_FX]->SetAlpha( (int)(255*(c_fGaugeEffectTime-dValue)) );
	}

	if( m_pSpr[GAUGE_BG] ) {
		m_pSpr[GAUGE_BG]->SetWidthRatio( fRatio );
		m_pSpr[GAUGE_BG]->SetRightU_Ratio( fRatio );
	}

	F3dZControl::Process( dTime, dMeanDT );
}

void F3dZControlGaugeNoBG::Render( double dTime )
{  
	//TODO
	if( IsShow() == FALSE ) return;

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
		{	
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}

	renderCaption(dTime);
	renderWire();
}

void F3dZControlGaugeNoBG::SetSpr()
{
	//효과 부분 크기 조정
	double fLeft, fRight;
	if( m_fPrevValue > m_fValue ) { //감소
		m_bDecMode = true;
		fLeft  =     m_fValue/m_fMaxValue;
		fRight = m_fPrevValue/m_fMaxValue;

		m_fGoal  = m_fValue    /m_fMaxValue;
		m_fBegin = m_fPrevValue/m_fMaxValue;
	}
	else { //증가
		m_bDecMode = false;
		fLeft  = m_fPrevValue/m_fMaxValue;
		fRight =     m_fValue/m_fMaxValue;

		//게이지 목표치 설정
		m_fBegin = m_fValue    /m_fMaxValue;
		m_fGoal  = m_fPrevValue/m_fMaxValue;
	}

	if( m_pSpr[GAUGE_FX] ) {
		m_pSpr[GAUGE_FX]->SetLeftRatio ( GetRect(), fLeft  );
		m_pSpr[GAUGE_FX]->SetRightRatio( GetRect(), fRight );

		m_pSpr[GAUGE_FX]->SetLeftU_Ratio( fLeft );
		m_pSpr[GAUGE_FX]->SetRightU_Ratio( fRight );
	}

	m_fGap       = m_fBegin - m_fGoal;
	m_dCheckTime = m_dTime;
}

void F3dZControlGaugeNoBG::SetStep( short step )
{  
	m_nStep = step;
}

void F3dZControlGaugeNoBG::Reset(const double fValue)
{
	 

	m_fMaxValue  = fValue;
	m_fPrevValue = fValue;
	m_fValue     = fValue;
	if( m_pSpr[GAUGE_FX] )
	{
		m_pSpr[GAUGE_FX]->SetAlpha( 0 );
	}

	m_fGoal		= 1.f;
	m_fBegin	= 1.f;

	if( m_pSpr[GAUGE_FX] ) {
		m_pSpr[GAUGE_FX]->SetLeftRatio ( GetRect(), 1  );
		m_pSpr[GAUGE_FX]->SetRightRatio( GetRect(), 1 );

		m_pSpr[GAUGE_FX]->SetLeftU_Ratio( 1 );
		m_pSpr[GAUGE_FX]->SetRightU_Ratio( 1 );
	}

	m_fGap       = 0;
	m_dCheckTime = m_dTime;
}

void F3dZControlGaugeNoBG::SetValue( const double fValue )
{ 
	 
	if( m_fValue == fValue ) 
		return;

	if( m_fValue < 0.f ) { //처음 셋팅
		m_fPrevValue = fValue;
		m_fValue     = fValue;
		if( m_pSpr[GAUGE_FX] ) m_pSpr[GAUGE_FX]->SetAlpha( 0 );
	}
	else {
		m_fPrevValue = m_fValue;
		m_fValue     = fValue;
		if( m_pSpr[GAUGE_FX] ) m_pSpr[GAUGE_FX]->SetAlpha( 255 );
	}

	
	m_fPrevValue = std::max<double>( m_fPrevValue, 0. );
	m_fPrevValue = std::min<double>( m_fPrevValue, m_fMaxValue );

	m_fValue = std::max<double>( m_fValue, 0. );
	m_fValue = std::min<double>( m_fValue, m_fMaxValue );

	//효과 부분 크기 조정
	double fLeft, fRight;
	if( m_fPrevValue > m_fValue ) { //감소
		m_bDecMode = true;
		fLeft  =     m_fValue/m_fMaxValue;
		fRight = m_fPrevValue/m_fMaxValue;

		m_fGoal  = m_fValue    /m_fMaxValue;
		m_fBegin = m_fPrevValue/m_fMaxValue;
	}
	else { //증가
		m_bDecMode = false;
		fLeft  = m_fPrevValue/m_fMaxValue;
		fRight =     m_fValue/m_fMaxValue;

		//게이지 목표치 설정
		m_fBegin = m_fValue    /m_fMaxValue;
		m_fGoal  = m_fPrevValue/m_fMaxValue;
	}

	if( m_pSpr[GAUGE_FX] ) {
		m_pSpr[GAUGE_FX]->SetLeftRatio ( GetRect(), fLeft  );
		m_pSpr[GAUGE_FX]->SetRightRatio( GetRect(), fRight );

		m_pSpr[GAUGE_FX]->SetLeftU_Ratio( fLeft );
		m_pSpr[GAUGE_FX]->SetRightU_Ratio( fRight );
	}

	m_fGap       = m_fBegin - m_fGoal;
	m_dCheckTime = m_dTime;

	//double fLevel = m_fValue/m_fMaxValue*100.f;

	////TODO Set Change
	//if( fLevel >= 100.f )	{	}
	//else if( fLevel >= 70.f && fLevel < 100.f ) {	}
	//else if( fLevel >= 30.f && fLevel <  70.f ) {	}
	//else {	}

	//SetChange();
}

void F3dZControlGaugeNoBG::SetMaxValue( const double fValue )
{  
	if( m_fMaxValue == fValue ) return;

	m_fMaxValue = fValue;
	SetSpr();
}

void F3dZControlGaugeNoBG::SetChange()
{  
	int nCnt = 0;
	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ){
			if( nCnt >= c_nGaugeSpriteCount ) break;
			m_pSpr[nCnt++] = m_vSprite[m_nState][i].get();
		}
	}
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZEditableControlGauge::F3dZEditableControlGauge()
{  

}

F3dZEditableControlGauge::~F3dZEditableControlGauge()
{  

}

void F3dZEditableControlGauge::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite->nSetCount < 0 ) return;

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();

	int l, t, r, b;

	for( int i(0); m_nSetCount>i; ++i )
	{
		for( int n(0); nPieceCount>n; ++n ) 
		{
			F3dCUISpritePtr pRenderSprite( new F3dCUISprite );

			l = 0;
			t = 0;
			r = l + control_arg.nWidth;
			b = t + control_arg.nHeight;

			pRenderSprite->rtScreen.left   = l;
			pRenderSprite->rtScreen.top    = t;
			pRenderSprite->rtScreen.right  = r;
			pRenderSprite->rtScreen.bottom = b;

			pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

			if( !pSprite->vTexList[i].empty() )
			{
				const F3dZUITexture* pTex = pSprite->vTexList[i][n];
				pRenderSprite->TextureID = pTex->TextureID;
			}

			m_vSprite[i].push_back( pRenderSprite );
		}
	}

	SetPos( control_arg.x, control_arg.y );

	AfterCreateControl();
}

void F3dZEditableControlGauge::Process( double dTime, double dMeanDT )
{  

}

void F3dZEditableControlGauge::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()-1>i; ++i )
		{	
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}

	renderCaption( dTime );
}
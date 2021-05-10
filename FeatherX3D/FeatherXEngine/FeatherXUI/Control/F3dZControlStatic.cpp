#include "FeatherXEnginePrecompile.h"
 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlSimple::F3dZControlSimple()
{
	 
}

void F3dZControlSimple::SetInfo( const int in_x, const int in_y, const int out_x, const int out_y )
{
	 
	m_vStart2d.x = (float)in_x;
	m_vStart2d.y = (float)in_y;
	m_vEnd2d.x   = (float)out_x;
	m_vEnd2d.y   = (float)out_y;

	m_nStep = STEP_INIT;
	SetAlpha( 0.f );
}

namespace
{
#ifdef _DEBUG
	const float c_fMoveTime  = 1.0;
	const float c_fDelayTime = 1.0;
#else
	const float c_fMoveTime  = 0.3f;
	const float c_fDelayTime = 1.0f;
#endif
}

void F3dZControlSimple::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
	 
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}
}


void F3dZControlSimple::Render( double dTime )
{ 
	 
	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	renderCaption( dTime );

	renderWire();
}

void F3dZControlSimple::Process( double dTime, double dMeanDT )
{
	 
	F3dZControl::Process( dTime, dMeanDT );

	switch( m_nStep ) 
	{	
	case STEP_INIT : 
		{
			m_nStep = STEP_ING;
			m_dCheckTime = m_dTime;
			SetPos( (int)m_vStart2d.x, (int)m_vStart2d.y );
			SetAlpha( 0.f );
		}
		break; 
	case STEP_ING  : 
		{
			float fValue = 0.f;
			fValue = (float)(m_dTime-m_dCheckTime);
			if( fValue >= (c_fMoveTime) ) {
				SetPos( (int)m_vEnd2d.x, (int)m_vEnd2d.y );
				m_dCheckTime = m_dTime;
				m_nStep = STEP_END;
				SetAlpha( 1.f );
			}
			else {
				float fAlpha = getClampRatioPow( fValue, c_fMoveTime );
				SetAlpha( fAlpha );
				F3dVector2 vRet;
				Vec2Lerp( vRet, m_vStart2d, m_vEnd2d, fAlpha );
				MovePos( (int)vRet.x, (int)vRet.y );
			}
		}
		break;
	case STEP_END  : 
		{
			float fValue = 0.f;
			fValue = (float)(m_dTime-m_dCheckTime);
			if( fValue >= (c_fDelayTime) ) {
				m_nStep = STEP_NONE;
				SetShow( FALSE );
				SetAlpha( 0.f );
			}else {
				float fAlpha = getClampRatioPow( fValue, c_fDelayTime );
				SetAlpha( 1.f-fAlpha );
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace
{
	const int c_nMultiLineDefHeight = 18;
	const double c_dBlinkCheck = 0.15f;
};

F3dZControlStatic::F3dZControlStatic() : m_bOverRender(false), m_bOverBlink(false)
{  

}

F3dZControlStatic::~F3dZControlStatic()
{  

}

void F3dZControlStatic::SetOverBlink( bool bFlag )
{  
	m_bOverBlink = bFlag;
}

void F3dZControlStatic::SetOver( bool bFlag )
{  
	m_bOverRender = bFlag;
}

void F3dZControlStatic::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}
}

void F3dZControlStatic::Process( double dTime, double dMeanDT )
{  
	F3dZControl::Process( dTime, dMeanDT );

	if( m_bOverBlink ) {

		if( dTime-m_dBlinkTime >= c_dBlinkCheck ) { 
			m_bOverRender = !m_bOverRender;
			m_dBlinkTime = dTime;
		}
	}
}

void F3dZControlStatic::RenderTarget( double dTime, F3dCRenderTargetUISprite& RenderLayer, int nLayer )
{
	 
	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
			RenderLayer.RegUISprite( m_vSprite[m_nState][i].get(), nLayer );
	}
}

void F3dZControlStatic::Render( double dTime )
{ 
	 
	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	renderCaption( dTime );
	renderWire();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlStaticMultiLine::F3dZControlStaticMultiLine() : m_nOffSetY(0), m_nMultiLineDefHeight( c_nMultiLineDefHeight )
{ 
	 

}

F3dZControlStaticMultiLine::~F3dZControlStaticMultiLine()
{ 
	 

}

void F3dZControlStaticMultiLine::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{ 
	 
	F3dZControlStatic::CreateControl( pSprite, control_arg );
}

void F3dZControlStaticMultiLine::SetAlpha( float fRatio )
{ 
	 
	F3dZControl::SetAlpha( fRatio );

	for( UINT i(0); m_vTextList.size()>i; ++i )
		m_vTextList[i]->SetAlpha( fRatio );
}

void F3dZControlStaticMultiLine::Render( double dTime )
{ 
	 
	if( IsShow() == FALSE ) return;

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );

		if( m_bOverRender && m_nSetCount > F3dZUISprite::SET_OVER ) {
			for( UINT i(0); m_vSprite[F3dZUISprite::SET_OVER].size()>i; ++i )
				F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[F3dZUISprite::SET_OVER][i].get() );
		}
	}

	renderChild( dTime );

	if( !m_vTextList.empty() ) {

		for( UINT i(0); m_vTextList.size()>i; ++i ) {
			if( m_VisibleCount > 0 ) {
				if( (i+m_nCurScrollPos) < m_VisibleCount )
					m_vTextList[i]->Render( dTime );
			}
			else {
				m_vTextList[i]->Render( dTime );
			}
		}
	}

	renderWire();
}

void F3dZControlStaticMultiLine::SetCenter3DRender(const F3dVector& vtxCenter,const int nWidth,const int nHeight)
{
	F3dZControl::SetCenter3DRender( vtxCenter, nWidth,nHeight );

	if( !m_vTextList.empty() ) {

		for( UINT i(0); m_vTextList.size()>i; ++i )
			m_vTextList[i]->SetCenter3DRender( vtxCenter, nWidth,nHeight );
	}
}

void F3dZControlStaticMultiLine::SetRenderTopLevel( bool bFlag )
{
	F3dZControl::SetRenderTopLevel( bFlag );

	if( !m_vTextList.empty() ) {

		for( UINT i(0); m_vTextList.size()>i; ++i )
			m_vTextList[i]->SetRenderTopLevel( bFlag );
	}
}

void F3dZControlStaticMultiLine::_setText( MCParser::TEXT_DATA* pData, const F3dColor& textColor, int& nLine, DWORD& curWidth )
{
	F3dZUITextPtr pText( new F3dZUIText( pData->text.c_str(), GetRect().GetWidth(), m_nMultiLineDefHeight, m_dwTextFlag, pData->font.c_str(), m_nFontSize, m_dwTextAlign, textColor, m_bBillboard  ,m_b3DRender ,m_b3DCursor) );
	pText->SetRenderTopLevel( m_bRenderTopLevel );

	if( nLine != pData->nLine ) {
		nLine = pData->nLine;
		curWidth = 0;
	}

	pText->SetPos( pData->dwOffSetX+curWidth, m_nOffSetY+(pData->nLine*m_nMultiLineDefHeight) );
	m_vTextList.push_back( pText ); 

	curWidth += pData->dwStrWidth;
	curWidth += pData->dwOffSetX;

	if( m_nLastWidth < (int)curWidth )
		m_nLastWidth = curWidth;
}

void F3dZControlStaticMultiLine::SetCaption( const WCHAR* pStr, BOOL bNew/*=FALSE*/ )
{ 
	 
	m_strCaption = pStr; 

	if( wcscmp( m_strCaption.c_str(), m_strPrevCaption.c_str() ) == 0 )
		return;

	m_strPrevCaption = m_strCaption;

	m_vTextDataList.erase( m_vTextDataList.begin(), m_vTextDataList.end() );

	MCParser::DoParse( m_strCaption.c_str(), GetFontName(), GetFontSize(), GetRect().GetWidth(), m_vTextDataList );

	m_vTextList.erase( m_vTextList.begin(), m_vTextList.end() );

	if( m_vTextDataList.empty() ) return;

	m_nLastWidth   = 0;

	int   nLine    = 0;
	DWORD curWidth = 0;
	for( UINT i(0); m_vTextDataList.size()>i; ++i )
	{
		MCParser::TEXT_DATA* pData = &m_vTextDataList[i];

		if( pData->color != 0xFFFFFFFF ) 
		{
			_setText( pData, pData->color, nLine, curWidth );
		}
		else
		{
			_setText( pData, m_TextColor, nLine, curWidth );
		}
	}

	TextMove();

	m_nLastLine  = nLine+1;

//	int nHeight = (nLine+1)*m_nMultiLineDefHeight;
//	SetSize( GetRect().GetWidth(), nHeight );
}

void F3dZControlStaticMultiLine::SetLineHeight( int nValue ) 
{ 
	m_nMultiLineDefHeight = nValue; 
	TextMove();
}

void F3dZControlStaticMultiLine::SetOffSetY( int nOffSetY )
{ 
	 
	m_nOffSetY = nOffSetY;
}

void F3dZControlStaticMultiLine::MovePos( int x, int y )
{ 
	 
	F3dZControl::MovePos( x, y );
	TextMove();
}

void F3dZControlStaticMultiLine::MovePosOffset( int offsetx, int offsety )
{ 
	 
	F3dZControl::MovePosOffset( offsetx, offsety );
	TextMove();
}

void F3dZControlStaticMultiLine::TextMove()
{ 
	 
	int   nLine    = 0;
	DWORD curWidth = 0;
	for( UINT i(0); m_vTextDataList.size()>i; ++i ) {

		MCParser::TEXT_DATA* pData = &m_vTextDataList[i];

		if( nLine != pData->nLine ) {
			nLine = pData->nLine;
			curWidth = 0;
		}

		m_vTextList[i]->SetPos( m_nPosX+pData->dwOffSetX+curWidth, m_nPosY+m_nOffSetY+(pData->nLine*m_nMultiLineDefHeight) );

		curWidth += pData->dwStrWidth;
		curWidth += pData->dwOffSetX;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
F3dZControlRollStatic::F3dZControlRollStatic() : m_dRollTime(0.f), m_dwTextWidth(0), m_dwTextHeight(0), m_bRoll(false)
{
	 
}

F3dZControlRollStatic::~F3dZControlRollStatic()
{
	 
}

void F3dZControlRollStatic::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{
	 
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}
}

void F3dZControlRollStatic::Process( double dTime, double dMeanDT )
{
	 

	F3dZControl::Process( dTime, dMeanDT );

	if( m_Caption.get() && m_bRoll ) {

		if( dTime-m_dRollTime >= 0.03f ) {
			m_dRollTime = dTime;
			--m_nPosX;
		}

		m_Caption->SetPos( m_nPosX, m_nPosY );

		if( m_rect.left - m_nPosX >= (int)m_dwTextWidth )
			m_nPosX = m_rect.right;
	}
}

void F3dZControlRollStatic::Render( double dTime )
{
	 

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
	}

	renderCaption( dTime );

	renderWire();
}

void F3dZControlRollStatic::SetCaption( const WCHAR* pStr, BOOL bNew/*=FALSE*/ )
{
	 

	m_strCaption = pStr; 

	int nWidth  = m_rect.GetWidth();
	int nHeight = m_rect.GetHeight();

	F3dZUIText::GetStringSize( m_strFontName.c_str(), m_nFontSize, FALSE, m_strCaption.c_str(), (int)m_strCaption.size(), m_dwTextWidth, m_dwTextHeight );

	if( m_rect.GetWidth() <  (int)m_dwTextWidth ) {
		m_bRoll = true;

		nWidth  = m_dwTextWidth;
		nHeight = m_dwTextHeight;

		bNew = TRUE;
	}
	else
	{
		if( m_bRoll ) {
			m_bRoll = false;
			bNew = TRUE;
		}
	}

	if( bNew ){
		m_Caption.reset( new F3dZUIText( m_strCaption.c_str(), nWidth, nHeight, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor, m_bBillboard ,m_b3DRender,m_b3DCursor, m_bTextRoll ) );
		m_Caption->SetParent( this );
		m_Caption->onRefreshText();
		m_Caption->SetRenderTopLevel( m_bRenderTopLevel );
	}
	else {
		if( m_Caption.get() )
			m_Caption->SetText( m_strCaption.c_str(), m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor );
		else {
			m_Caption.reset( new F3dZUIText( m_strCaption.c_str(), nWidth, nHeight, m_dwTextFlag, m_strFontName.c_str(), m_nFontSize, m_dwTextAlign, m_TextColor, m_bBillboard ,m_b3DRender,m_b3DCursor, m_bTextRoll ) );
			m_Caption->SetParent( this );
			m_Caption->onRefreshText();
			m_Caption->SetRenderTopLevel( m_bRenderTopLevel );
		}
	}

	m_Caption->SetPos( m_nPosX, m_nPosY );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void F3dZEditableControlStatic::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZEditableControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		     if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
		else if( 3 == nPieceCount ) createControl_3( pSprite, control_arg );
		else if( 9 == nPieceCount ) createControl_9( pSprite, control_arg );
	}

	AfterCreateControl();
}

void F3dZEditableControlStatic::Render( double dTime )
{  
	if( !IsShow() ) return;

	F3dZEditableControl::Render( dTime );

	if( m_vSprite ) {
		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i ) {
			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
		}
	}
	else {
		renderWire();
	}

	renderCaption( dTime );
}
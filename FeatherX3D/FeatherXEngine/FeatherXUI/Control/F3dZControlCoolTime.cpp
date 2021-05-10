#include "FeatherXEnginePrecompile.h"
 

F3dZControlCoolTime::F3dZControlCoolTime()
{  

}

F3dZControlCoolTime::~F3dZControlCoolTime()
{  

}

void F3dZControlCoolTime::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  
	F3dZControl::CreateControl( pSprite, control_arg );

	if( pSprite && pSprite->nSetCount > 0 ) {
		int nPieceCount = (int)pSprite->vTexList[F3dZUISprite::SET_NORMAL].size();
		if( 1 == nPieceCount ) createControl_1( pSprite, control_arg );
	}

	SetDefCoolInfo();
}

void F3dZControlCoolTime::SetDefCoolInfo()
{  
	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->nDrawType = F3dCUISprite::TRIANGLE_FAN;
			}
		}
	}
}

void F3dZControlCoolTime::SetDegree( int nDegree )
{  
	if( m_vSprite ) {
		for( short j(0); m_nSetCount>j; ++j )
		{
			for( UINT i(0); m_vSprite[j].size()>i; ++i )
			{
				F3dCUISprite* pUISprite = m_vSprite[j][i].get();
				pUISprite->nDegree = nDegree;
			}
		}
	}
}

void F3dZControlCoolTime::Process( double dTime, double dMeanDT )
{  

}

void F3dZControlCoolTime::Render( double dTime )
{  
	if( IsShow() == FALSE ) return;

 	if( m_vSprite ) {
 		for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
 			F3dCRenderMgr::GetSingleton()->RegUISprite( m_vSprite[m_nState][i].get() );
 	}
}


//////////////////////////////////////////////////////////////////////////
void F3dZEditableControlCoolTime::CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg )
{  

}

void F3dZEditableControlCoolTime::Render( double dTime )
{  

}
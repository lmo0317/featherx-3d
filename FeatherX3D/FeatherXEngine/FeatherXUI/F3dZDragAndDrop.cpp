#include "FeatherXEnginePrecompile.h"
 

//////////////////////////////////////////////////////////////////////////
const WCHAR* ZUIDragAndDropObject::GetParentID()
{ 
	  
	if( NULL != m_pSenderParent)
		return m_pSenderParent->GetID(); 

	return L"Unknown";
}
const WCHAR* ZUIDragAndDropObject::GetControlID()
{ 
	  
	if( NULL != m_pSenderControl)
		return m_pSenderControl->GetID(); 

	return L"Unknown";
}

void ZUIDragAndDropObject::OnDragObjectDroped()
{
	 

	if( m_pSenderControl ) 
		m_pSenderControl->OnDragObjectDroped();
}

POINT ZUIDragAndDropRender::m_ptPos;
BOOL ZUIDragAndDropRender::m_bUpdate = FALSE;

//////////////////////////////////////////////////////////////////////////
ZUIDragAndDropIconRender::ZUIDragAndDropIconRender() : m_nSetCount(1), m_vSprite(NULL)
{ 
	 

}

ZUIDragAndDropIconRender::~ZUIDragAndDropIconRender()
{ 
	 
	clearSprite();
}

void ZUIDragAndDropIconRender::clearSprite()
{ 
	 
	if( m_vSprite ) {
		for( int i(0); m_nSetCount>i; i++ )
			m_vSprite[i].clear();
		delete [] m_vSprite;
		m_vSprite = NULL;
	}	
}

void ZUIDragAndDropIconRender::SetIcon( const WCHAR* pSpriteName )
{ 
	 
	const F3dZUISprite* pSprite = F3dZUISpriteManager::GetSingleton()->GetSprite( pSpriteName );
	if( m_nSetCount <= 0 ) {
		_MBASSERT(0);
		return;
	}

	clearSprite();

	if( pSpriteName == NULL ) return;
	if( pSprite == NULL ) return;

	m_vSprite = new std::vector< F3dCUISpritePtr >[m_nSetCount];

	int l, t, r, b, nW = 0, nH = 0;

	for( int i(0); m_nSetCount>i; ++i )	{
		F3dCUISpritePtr pRenderSprite( new F3dCUISprite );


		if( !pSprite->vTexList[i].empty() )		{
			const F3dZUITexture* pTex = pSprite->vTexList[i][0];
			pRenderSprite->TextureID = pTex->TextureID;
			nW = pTex->nWidth;
			nH = pTex->nHeight;
		}

		l = 0;
		t = 0;
		r = l + nW;
		b = t + nH;

		pRenderSprite->rtScreen.left   = l;
		pRenderSprite->rtScreen.top    = t;
		pRenderSprite->rtScreen.right  = r;
		pRenderSprite->rtScreen.bottom = b;

		pRenderSprite->ori_rect = UI_RECT( l, t, r, b );

		m_vSprite[i].push_back( pRenderSprite );
	}

	SetState( ST_NORMAL );
}

void ZUIDragAndDropIconRender::SetState( int nState )
{ 
	 
	m_nState = nState;
}

void ZUIDragAndDropIconRender::Render( double dTime )
{ 
	 
	if( m_vSprite == NULL ) return;

	for( UINT i(0); m_vSprite[m_nState].size()>i; ++i )
	{
		F3dCUISprite* pSpr = m_vSprite[m_nState][i].get();
		if( m_bUpdate ) {
			pSpr->SetPosition( m_ptPos.x, m_ptPos.y );
		}

		F3dCRenderMgr::GetSingleton()->RegUISprite( pSpr );
	}

	if( m_bUpdate ) m_bUpdate = FALSE;
}




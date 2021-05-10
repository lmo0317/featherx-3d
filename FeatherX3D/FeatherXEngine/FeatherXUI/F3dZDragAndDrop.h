

#pragma once

class F3dZWnd;
class F3dZControl;

//////////////////////////////////////////////////////////////////////////
//Drag Object : 부모 윈도우, Drag 컨트롤
class ZUIDragAndDropObject 
{
public:
	ZUIDragAndDropObject()  {}
	~ZUIDragAndDropObject() {}

	void Set( F3dZWnd* pSenderParent, F3dZControl* pSenderControl )
	{
		m_pSenderParent  = pSenderParent;
		m_pSenderControl = pSenderControl;
	}

	F3dZWnd*	 GetParent()	{ return m_pSenderParent; }
	F3dZControl* GetControl()	{ return m_pSenderControl;}

	const WCHAR* GetParentID();
	const WCHAR* GetControlID();		

	void	OnDragObjectDroped();

protected:
	F3dZWnd*			m_pSenderParent;
	F3dZControl*		m_pSenderControl;
};

//////////////////////////////////////////////////////////////////////////
//Drag 랜더 객체
class ZUIDragAndDropRender
{
public:
	ZUIDragAndDropRender() {}
	virtual ~ZUIDragAndDropRender() {}

	virtual void Render( double dTime ) = 0;
	static void SetPosition(int x, int y)
	{
		m_ptPos.x = x;
		m_ptPos.y = y;
		m_bUpdate = TRUE;
	}

protected:
	static BOOL	 m_bUpdate;
	static POINT m_ptPos;
};

//////////////////////////////////////////////////////////////////////////
//Drag Icon Type
class ZUIDragAndDropIconRender : public ZUIDragAndDropRender
{
public:
	ZUIDragAndDropIconRender();
	virtual ~ZUIDragAndDropIconRender();

	void SetIcon( const WCHAR* pSpriteName );
	void SetState( int nState );

	virtual void Render( double dTime );

protected:
	enum 
	{
		ST_NORMAL = 0,
	};
	void clearSprite();
	short			m_nSetCount;
	int				m_nState;			
	std::vector< F3dCUISpritePtr >*	m_vSprite;
};


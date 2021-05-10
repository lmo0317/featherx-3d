
#pragma once

class F3dZControlCombo : public F3dZControl
{
public:
	F3dZControlCombo();
	virtual ~F3dZControlCombo();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	//Combo	Attribute
	void			AddItem( const WCHAR* pString );
	const int		GetItemCount() { return (int)m_vItem.size(); }

	void			OnItemButton();

	int				GetCurrentIndex();
	int				SetItemIndex( int nIndex );

	void			ClearItem();
	void			SetEnableComboBtn( bool bFlag );

protected:

	bool	m_bItemSelect;

	void createText( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButton( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createSelect( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createItemBG( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	void createItemList( const WCHAR* pString );

	int							m_nCurrentPos;

	std::vector< std::wstring > m_vItemList;

	F3dZControl* m_pText;
	F3dZControl* m_pBtn;

	F3dZControl* m_pItemBG;
	std::vector< F3dZControl* > m_vItem;
	F3dZControl* m_pSelect;

private:

};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class F3dZEditableControlCombo : public F3dZEditableControl
{
public:
	F3dZEditableControlCombo();
	virtual ~F3dZEditableControlCombo();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual void	SetSize( const int& nWidth, const int& nHeight );

protected:

	void createText( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createButton( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createItemList( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );
	void createSelect( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	F3dZEditableControl* m_pText;
	F3dZEditableControl* m_pBtn;
	F3dZEditableControl* m_pItem;
	F3dZEditableControl* m_pSelect;

private:

};


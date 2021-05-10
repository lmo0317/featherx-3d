
#pragma once

class CInput;
class F3dZControlEdit;
typedef std::map<HWND, F3dZControlEdit*> EditHandleMap;

class F3dZControlEdit : public F3dZControl
{
public:
	F3dZControlEdit();
	virtual ~F3dZControlEdit();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	virtual int		OnMouseMsg( UINT uMsg, const int& x, const int& y, int& nPrevRetValue );

	virtual void	SetCaption( const WCHAR* pStr, BOOL bNew=FALSE );

	virtual void	MovePos( int x, int y );
	virtual void	SetAlpha( float fRatio );
	virtual void	SetRenderTopLevel( bool bFlag );

	static void		InitEdit();
	static void		DeInitEdit();
	static bool		IsAnyFocus();

	void	SetFocus( BOOL bFocus );
	const BOOL	IsFocus() const { return m_bFocus; }
	const WCHAR* GetSay() const { return m_strSay.c_str(); }
	const int	GetLength() const { return m_strSay.length(); }

	void SetMultiLine( BOOL bMultiLine ) { m_bMultiLine = bMultiLine; }
	void SetLimit( int nLimit );
	void ResetComp()	{ m_pIMEInput->ResetComp();	}
	void GetComp( std::wstring& wszComp )	{ m_pIMEInput->GetComp(wszComp);	}
	const bool IsLimit()		{ return m_pIMEInput->IsLimit() != 0;	}

	void SetNumberAndCapital( bool bFlag );
	void SetOnlyNumber( bool bFlag );
	void SetPrivateMode( bool bFlag );
	void SetCaretStopMove(bool bFlag);

	void SetSkipEnterKeyClear( bool bFlag );

	const std::vector< std::wstring >& GetMultiLine() { return m_vMultiLine; }

	void SetDummyCaption( const WCHAR* pStr );

	void SetItemLink( bool bFlag );
	void SetLimitWide(bool bFlag);

	void SayClear();
	void SayOneErase();
	void SayAddOne( const WCHAR* pStr );

protected:

	const bool IsPrivateMode() const { return m_bPrivateMode; }

	const BOOL IsMultiLine() const { return m_bMultiLine; }
	void RefreshMultiLine();

	void SetIndicator( const WCHAR* pStr );
	void SetComposition( const WCHAR* pStr );
	void SetReading( const WCHAR* pStr );
	void SetCandidate( const WCHAR* pStr );
	void SetCaret( const WCHAR* pStr );

	void renderIndicator  ( double dTime );
	void renderComposition( double dTime );
	void renderReading    ( double dTime );
	void renderCandidate  ( double dTime );
	void renderMultiLine  ( double dTime );
	void renderCaret      ( double dTime );

	void copyClipboard();
	void pasteClipboard();

	static LRESULT CALLBACK EditBoxProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	static EditHandleMap* s_mapEdit;
	static HWND s_hWndCurrentFocus;

	LONG OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND		m_hWnd;	
	WNDPROC		m_oldProc;
	CInput*		m_pIMEInput;
	BOOL		m_bUpdate;
	BOOL		m_bFocus;

	F3dZUITextPtr	m_Indicator;
	F3dZUITextPtr	m_Composition;
	F3dZUITextPtr	m_Reading;
	F3dZUITextPtr	m_Candidate;
	F3dZUITextPtr	m_Caret;
	F3dColor		m_CompositionColor;
	F3dColor		m_IndicatorColor;
	F3dColor		m_ReadingColor;
	F3dColor		m_CandidateColor;
	F3dColor		m_CaretColor;
	std::wstring	m_strSay;

	unsigned short	m_nLimitLength;

	BOOL			m_bMultiLine;
	int				m_nOldLine;
	int				m_nCurLine;
	std::vector< std::wstring > m_vMultiLine;

	std::vector< F3dZUITextPtr > m_vTextList;
	UI_RECT			m_defRect;

	double			m_CheckCaret;
	BOOL			m_bStateCaret;

	bool			m_bPrivateMode;
	
	bool			m_bCtrl;
	std::wstring	m_strDummyText;

	bool			m_bCaretStopMove;

	bool			m_bItemLink;
	int				m_nItemLinkEndPosX;
	int				m_nItemLinkEndPosY;

	std::wstring	m_cmp_text;
private:

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class F3dZEditableControlEdit;
typedef std::map<HWND, F3dZEditableControlEdit*> EditableHandleMap;

class F3dZEditableControlEdit : public F3dZEditableControl
{
public:
	F3dZEditableControlEdit();
	virtual ~F3dZEditableControlEdit();

	virtual void	CreateControl( const F3dZUISprite* pSprite, const CONTROL_ARG& control_arg );

	virtual void	Process( double dTime, double dMeanDT );
	virtual void	Render( double dTime );

	static void		InitEdit();
	static void		DeInitEdit();

protected:

	static LRESULT CALLBACK EditBoxProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	static EditableHandleMap* s_mapEdit;

	HWND		m_hWnd;	
	WNDPROC		m_oldProc;

private:

};
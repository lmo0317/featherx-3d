// CInput.h - part if Multi-Language input class
//
// Copyright (C) 2000-2005, Kwon-il Lee
//
// Kwon-il Lee
// zupet@hitel.net


#if !defined(_C_INPUT_H_)
#define _C_INPUT_H_


/*----------------------------------------------------------------------------*/ 
int GetCharsetFromLang(LANGID);
int	GetCodePageFromLang( LANGID langid );

/*----------------------------------------------------------------------------*/ 
class CInput
{
public:
	CInput();
	~CInput();

	/* messages */ 
	void		OnInputLanguageChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam, std::wstring& cut_text );
	const bool	IsEmpty() const;
	void		SetLimit( int nLimit ) { m_nLimit = nLimit; }
	const int   GetLimit() const       { return m_nLimit; }

	void		SetLimitWide(bool bFlag) {m_bLimitWide = bFlag;}

	void        AddInput( const wchar_t* pStr );
	void		SetInput( const wchar_t* pStr );
//	int			GetInput(char* text, int len);
	int			GetInput(std::wstring& text );
//	int			GetComp(char* text, int len);
	int			GetComp(std::wstring& text );
	void		ResetComp()	{ m_comp = L""; };
		
//	int			GetReading(char* text, int len);
	int			GetReading(std::wstring& text);

	int			GetCandidate(DWORD index, char* text, int len);
	int			GetCandidateCount();
	int			GetCandidateSelection();
	int			GetCandidatePageSize();
	int			GetCandidatePageStart();
	void		GetUnderLine(int* start, int* end);

	/* property */ 
	bool		IsVerticalReading(){ return m_bVerticalReading; }
	bool		IsVerticalCandidate(){ return m_bVerticalCandidate; }
	WORD		GetCodePage() { return m_codePage; }
	LANGID		GetLangId() { return m_langId; }
	int			GetCharSet() { return GetCharsetFromLang(m_langId); }
	wchar_t*	GetIndicator() { return m_wszCurrIndicator; }
	int			GetImeState() { return m_ImeState; }

	//Utility
	void		SetOnlyNumber(bool bFlag)		{ m_bOnlyNumber = bFlag;		}
	void		SetNumberAndCapital(bool bFlag)	{ m_bNumberAndCapital = bFlag;	}

	void		SetSkipEnterKeyClear(bool bFlag)	{ m_bSkipEnterKey = bFlag;		}
	bool		IsSkipEnterKeyClear() { return m_bSkipEnterKey; }

	const BOOL	IsLimit() const;

protected:
	void		SetupImeApi(HWND hWnd);
	void		GetImeId();
	bool		GetReadingWindowOrientation();
	void		GetPrivateReadingString(HWND hWnd);
	void		CheckToggleState(HWND hWnd);

protected:
	std::wstring		m_input;
	std::wstring		m_comp;
	std::wstring		m_reading;
	std::vector<BYTE>	m_candidate;
	int					m_nLimit;

	int					m_ulStart;
	int					m_ulEnd;

	HKL					m_hkl;
	LANGID				m_langId;
	WORD				m_codePage;

	bool				m_bUnicodeIME;
	bool				m_bVerticalReading;
	bool				m_bVerticalCandidate;
	int					m_ImeState;
	wchar_t*			m_wszCurrIndicator;

	DWORD				m_dwId[2];

	HINSTANCE			m_hDllIme;

	bool				m_bOnlyNumber;

	bool				m_bSkipEnterKey;

	UINT				(WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	BOOL				(WINAPI * _ShowReadingWindow)( HIMC, BOOL );

	bool				m_bLimitWide;
	bool				m_bNumberAndCapital;
};

/*----------------------------------------------------------------------------*/ 


#endif //_C_INPUT_H_
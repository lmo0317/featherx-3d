
#pragma once

namespace UI_DEFINE
{
	const int c_nImpControlCount = 18;
	const char* c_pImpControl[];
};

//===============
//Window
struct WINDOW_INFO_ARG 
{
	WINDOW_INFO_ARG()
	{
		nCaptionID = -1;
		nToolTipID = -1;
		x          = 0;
		y          = 0;
		nWidth     = 0;
		nHeight    = 0;
	};

	std::wstring ID;
	std::wstring strStyle;
	int nCaptionID;
	int nToolTipID;
	int x;
	int y;
	int nWidth;
	int nHeight;
};

struct CONTROL_ARG 
{
	CONTROL_ARG()
	{
		nCaptionID    = -1;
		nToolTipID    = -1;
		nFontSize     = 10;
		x             = 0;
		y             = 0;
		nWidth        = 0;
		nHeight       = 0;
		dwTextColor   = 0xFFFFFFFF;
		dwTextColorBG = 0;
	};
	std::wstring ID;
	std::wstring strStyle;
	std::wstring strFontName;
	std::wstring strFontFlag;
	std::wstring strAni;
	int nCaptionID;
	int nToolTipID;
	int nFontSize;
	int x;
	int y;
	int nWidth;
	int nHeight;
	DWORD dwTextColor;
	DWORD dwTextColorBG;
};

enum _CONTROL_NOTIFY_MSG_
{
	//컨트롤
	INVALID_MSG        = 0,
	LBUTTON_CLICK      = 1,
	RBUTTON_CLICK      = 2,
	MBUTTON_CLICK      = 3,
	LBUTTON_DBL_CLICK  = 4,
	RBUTTON_DBL_CLICK  = 5,
	MBUTTON_DBL_CLICK  = 6,
	LBUTTON_DOWN_PRESS = 7,

	MOUSE_OVER		   = 8,

	EDIT_ENTER        = 20,
	EDIT_LINE_CHANGE  = 21,
	EDIT_TAB          = 22,
	EDIT_ENTER_EMPTY  = 23,
	EDIT_ARROW_DN     = 24,
	EDIT_ARROW_UP     = 25,
	EDIT_SPACE		  = 26,
	EDIT_BACK		  = 27,
	EDIT_ENDCOMPOSITION	= 28,
	EDIT_ITEM_LINK_BACK = 29,

	SCROLL_CHANGE     = 30,
	SCROLL_SELECT     = 31,

	PUSH_TYPE_CHANGE  = 40,

	COMBO_CHANGE      = 50,
	COMBO_SELECT      = 51,

	SLIDER_CHANGE     = 60,
	SLIDER_SELECT     = 61,

	//아이템 링크 툴팁
	CLICK_ITEM_TOOLTIP= 70,

	//윈도우
	WINDOW_IN         = 1000,
	WINDOW_OUT        = 1001,
};


struct WINDOW_ARG
{
	WINDOW_INFO_ARG			 window_info;
	std::vector<CONTROL_ARG> vControlList;
};

//===============
//Sprite
struct TEX_ARG
{
	std::wstring strName;
	int			nWidth;
	int			nHeight;
};

struct ANI_ARG
{
	int		     nSetCount;
	std::wstring strType;
	std::wstring strName;
	std::vector< TEX_ARG >	vTextList;
};

struct SPRITE_ARG
{
	std::vector<ANI_ARG>	vAniList;
};



//===============
//Window
struct UI_RECT
{
	UI_RECT() { left = top = 0; right = bottom = 1; };
	UI_RECT( int _left, int _top, int _right, int _bottom ) { left=_left; top=_top; right=_right; bottom=_bottom; };

	int left, top, right, bottom;

	UI_RECT operator + ( const UI_RECT& r ) const { return UI_RECT( left + r.left, top + r.top, right + r.right, bottom + r.bottom ); }
	UI_RECT operator - ( const UI_RECT& r ) const { return UI_RECT( left - r.left, top - r.top, right - r.right, bottom - r.bottom ); }
	bool operator == ( const UI_RECT& r ) const { return (left == r.left && top == r.top && right == r.right && bottom == r.bottom); }
	bool operator != ( const UI_RECT& r ) const { return (left != r.left || top != r.top || right != r.right || bottom != r.bottom); }

	bool IsInRect(int x, int y) const	{
		if(left <= x && right > x)		{
			if(top <= y && bottom > y)
				return true;
		}
		return false;		
	}

	bool IsIntersect(const UI_RECT& rcRect) const
	{
		return IsInRect(rcRect.left , rcRect.top) || IsInRect(rcRect.left , rcRect.bottom) || 
	   		   IsInRect(rcRect.right, rcRect.top) || IsInRect(rcRect.right, rcRect.bottom);
	}
	void Intersect(const UI_RECT& rcRect)
	{
		left	= std::max( left  , rcRect.left   );
		top		= std::max( top   , rcRect.top    );
		right	= std::min( right , rcRect.right  );
		bottom	= std::min( bottom, rcRect.bottom );
	}
	void Union(const UI_RECT& rcRect)
	{
		left	= std::min( left  , rcRect.left   );
		top		= std::min( top   , rcRect.top    );
		right	= std::max( right , rcRect.right  );
		bottom	= std::max( bottom, rcRect.bottom );
	}

	int GetWidth() const  { return right - left; }
	int GetHeight() const { return bottom - top; }

	static void SetPosRect( RECT& rect, const UI_RECT& ui_rect, const int& nPosX, const int& nPosY )
	{
		UI_RECT val_rect = ui_rect + UI_RECT( nPosX, nPosY, nPosX, nPosY );
		rect.left   = val_rect.left;
		rect.top    = val_rect.top;
		rect.right  = val_rect.right;
		rect.bottom = val_rect.bottom;	
	}

	static void SetScale( const UI_RECT& in_rect, UI_RECT& out_rect, float fRatio )
	{
		out_rect = in_rect;

		out_rect.right  = (int)((in_rect.right  - in_rect.left)*fRatio);
		out_rect.bottom = (int)((in_rect.bottom - in_rect.top )*fRatio);
	}

	bool IsInReSizeRect(int x, int y) const	{
		if(right - 20 <= x && right + 20 > x) {
			if(top - 20 <= y && top + 20 > y)
				return true;
		}
		return false;		
	}
};
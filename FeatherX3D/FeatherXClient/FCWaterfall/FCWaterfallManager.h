#pragma once
struct WATERFALL_ELEMENT
{
	std::wstring strFileName;
	int nTime;
	F3dVector vecPostion;
	F3dVector vecRotation;
};

struct WATERFALL_INFO
{
	std::vector<WATERFALL_ELEMENT> vecElement;
};

class FCWaterfallManager
{
public:
	FCWaterfallManager(){ };
	~FCWaterfallManager(){};

	void Init(WCHAR* strFile);
	void CreateWaterfall(WATERFALL_INFO &wf_Info);

private:

	F3dVector m_vPosition;
	F3dVector m_vRotation;
	std::vector<WATERFALL_INFO> m_vecWaterfall;
};

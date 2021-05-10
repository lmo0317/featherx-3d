#pragma once

class FCEventBoxManager : public FBSingleton<FCEventBoxManager>
{
public:
	FCEventBoxManager();
	~FCEventBoxManager();

	void CreateEventBox(WCHAR* strModel,const int nID,F3dVector& vecPos,F3dVector& vecRot);
	void Init(WCHAR* strFile);
	void Process(double dTime,double dt);
	void Render(double dTime,double dt);
	void Destroy();

	F3dVector GetStartBoxPosition();
	int getType(WCHAR* str);

private:

	std::vector< FCEventBox*> m_vecEventBox;
	std::map<int,_EVENT_BOX_INFO_*> m_hashEventBoxInfo;

};
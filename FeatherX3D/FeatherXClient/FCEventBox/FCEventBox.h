#pragma once

class FCEventBox
{
public:
	FCEventBox();
	~FCEventBox();

	virtual int getType() = 0;
	virtual void setJump(int nJump) {};

	virtual void Init(WCHAR* strModel,F3dVector& vecPos,F3dVector& vecRot);
	virtual void Process();
	virtual void Render();
	virtual void Destroy();
	virtual void CB_Contact(NxActor **actors);

	F3dVector getPos();

protected:

	F3dCModel* m_pModel;
};
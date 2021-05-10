#pragma once

class FCEventBoxJump : public FCEventBox
{
public:
	FCEventBoxJump();
	~FCEventBoxJump();

	virtual int getType() {return _EVENT_BOX_JUMP_;}
	virtual void Init(WCHAR* strModel,F3dVector& vecPos,F3dVector& vecRot);
	virtual void Process();
	virtual void Render();
	virtual void Destroy();

	virtual void CB_Contact(NxActor **actors);
	virtual void setJump(int nJump) {m_nJump = nJump;}

private:

	int m_nJump;
};
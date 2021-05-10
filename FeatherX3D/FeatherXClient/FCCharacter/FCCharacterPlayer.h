#pragma once

class FCCharacterPlayer
{
public:
	FCCharacterPlayer();
	~FCCharacterPlayer();

	void Init(const F3dVector& pos,const F3dVector& dir);
	void Process(double dTime,double dt);
	void Render(double dTime,double dt);
	void Destroy();
	void ForWard();
	void BackWard();
	void Left();
	void Right();
	void Jump();
	void Stop();
	void Break();


	F3dCModel* GetModel() {return m_pModel;}
	virtual void CB_Contact(NxActor **actors);

private:

	F3dCModel* m_pModel;
	F3dCTail* m_pTail;
	int m_nJump;
};
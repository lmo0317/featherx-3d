#pragma once

class FCCharacterManager : public FBSingleton<FCCharacterManager>
{
public:
	FCCharacterManager();
	~FCCharacterManager();

	void Init();
	void Process(double dTime,double dt);
	void Render(double dTime,double dt);
	void Destroy();
	FCCharacterPlayer* GetPlayer() {return m_pPlayer;}
	void CreatePlayer(F3dVector& vecPos,F3dVector& vecDir);

private: 

	FCCharacterPlayer* m_pPlayer;
};
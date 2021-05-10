#pragma once

class FCGameMain
{
public:
	FCGameMain();
	~FCGameMain();

	void Init();
	void Process(double dTime,double dt);
	void Render(double dTime,double dt);

	void GameStart();
	void GameEnd();
	bool IsGameStart() {return m_bStart;}

private:

	bool m_bStart;
	double m_dTime;
};
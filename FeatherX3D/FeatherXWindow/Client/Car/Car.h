#pragma once

class CCar
{
public:
	NxVehicle* vehicle;

	CCar();
	~CCar();
	void Init(const NxVec3& pos);
	void Render();
	void Process();
	void Destroy();

	void CreateCar(const NxVec3& pos);
	F3dCModel* GetBody() {return m_pBodyModel;}

private:

	F3dCModel* m_pBodyModel;
	std::vector<F3dCModel*> m_vecWheelModel;

	float m_fWheelAngle;
};
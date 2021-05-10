#include "FeatherXWindowPrecompile.h"

CCar::CCar():
m_fWheelAngle(0.f)
{

}

CCar::~CCar()
{

}

void CCar::Init(const NxVec3& pos)
{
	F3dMatrix matWorld;
	Identity(matWorld);
	m_pBodyModel = CFeatherXModelManager::GetSingleton()->LoadXML("models\\car_body.xml",matWorld);

	for(int i=0;i<4;++i)
	{
		F3dCModel* pModel = CFeatherXModelManager::GetSingleton()->LoadXML("models\\car_wheel.xml",matWorld);
		m_vecWheelModel.push_back(pModel);
	}

	CreateCar(pos);
}

void CCar::Render()
{
	vehicle->draw();
}

void CCar::Process()
{
	vehicle->updateVehicle(1.0f/60.0f);
	//vehicle 의 physic 정보와 자동차 모델정보를 update 시켜줌

	NxActor* pActor = vehicle->getActor();
	F3dMatrix mtxWorldTM;
	F3dMatrix mtxScale;
	Scaling(mtxScale,0.015,0.015,0.015);

	GetGlobalTM( mtxWorldTM, pActor );
	mtxWorldTM = mtxScale * mtxWorldTM;
	m_pBodyModel->SetWorldTransform( mtxWorldTM );

	for(int i=0;i<4;++i)
	{
		const NxWheel* pWheel = vehicle->getWheel(i);
		NxWheelShape* pShape = pWheel->getWheelShape();
		GetGlobalTM(mtxWorldTM,pShape);

		NxReal fSteerAngle = pShape->getSteerAngle();
		F3dMatrix mtxRotY;
		RotateY(mtxRotY,fSteerAngle);
		Scaling(mtxScale,0.015,0.015,0.015);

		mtxWorldTM._42 -= pWheel->getRadius()/4;
		mtxWorldTM = mtxRotY * mtxWorldTM;
		mtxWorldTM = mtxScale * mtxWorldTM;
		m_vecWheelModel[i]->SetWorldTransform(mtxWorldTM);
	}
}

void CCar::CreateCar(const NxVec3& pos)
{
	NxVehicleDesc vehicleDesc;
	NxBoxShapeDesc boxShapes;
	NxU32 nbGears = 10;

	boxShapes.dimensions.set(2.4f, 0.4f, 1.2f);
	vehicleDesc.carShapes.pushBack(&boxShapes);

	vehicleDesc.position				= pos;
	vehicleDesc.mass					= 1200.f;
	vehicleDesc.digitalSteeringDelta	= 0.04f;
	vehicleDesc.steeringMaxAngle		= 30.f;
	vehicleDesc.motorForce				= 3500.f;
	vehicleDesc.maxVelocity				= 300.f;
	vehicleDesc.cameraDistance			= 8.0f;

	NxVehicleMotorDesc motorDesc;
	NxVehicleGearDesc gearDesc;
	NxReal wheelRadius = 0.4f;

	vehicleDesc.centerOfMass.set(0,-1,0);
	motorDesc.minRpmToGearDown	= 1500;
	motorDesc.maxRpmToGearUp	= 4000;
	motorDesc.minRpm			= 1000;
	motorDesc.maxRpm			= 6000;
	
	motorDesc.torqueCurve.insert(1000.f, 800.f);
	motorDesc.torqueCurve.insert(2000.f, 1000.f);
	motorDesc.torqueCurve.insert(3000.f, 1500.f);
	vehicleDesc.motorDesc = &motorDesc;

	if(nbGears > gearDesc.getMaxNumOfGears())
	{
		return;
	}

	for(NxU32 i = 1; i <= nbGears; i++)
	{
		gearDesc.forwardGearRatios[i-1] = nbGears / (NxReal)i;
	}
	gearDesc.nbForwardGears = nbGears;
	gearDesc.backwardGearRatio = -11;
	vehicleDesc.gearDesc = &gearDesc;
	vehicleDesc.differentialRatio = 3.42f;

	NxWheelDesc wheelDesc[4];
	for(NxU32 i=0;i<4;i++)
	{
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = wheelRadius;
		wheelDesc[i].wheelWidth = 0.1f;
		wheelDesc[i].wheelSuspension = 0.2f;
		wheelDesc[i].springRestitution = 7000;
		wheelDesc[i].springDamping = 800;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 1.f;
		wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
		vehicleDesc.carWheels.pushBack(&wheelDesc[i]);
	}

	NxReal widthPos =1.09f;
	NxReal heightPos = -0.4f;
	NxReal fFlat = 1.6;
	wheelDesc[0].position.set( fFlat, heightPos, widthPos);
	wheelDesc[1].position.set( fFlat, heightPos,-widthPos);
	wheelDesc[2].position.set(-fFlat, heightPos, widthPos);
	wheelDesc[3].position.set(-fFlat, heightPos,-widthPos);

	NxU32 flags = NX_WF_BUILD_LOWER_HALF;
	wheelDesc[0].wheelFlags |= NX_WF_ACCELERATED | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[1].wheelFlags |= NX_WF_ACCELERATED | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[2].wheelFlags |= NX_WF_ACCELERATED | NX_WF_AFFECTED_BY_HANDBRAKE | flags;
	wheelDesc[3].wheelFlags |= NX_WF_ACCELERATED | NX_WF_AFFECTED_BY_HANDBRAKE | flags;

	vehicleDesc.steeringSteerPoint.set(1.8, 0, 0);
	vehicleDesc.steeringTurnPoint.set(-1.5, 0, 0);

	NxScene* pScene = mpManager::GetSingleton()->GetScene();
	vehicle = NxVehicle::createVehicle(pScene, &vehicleDesc);
	
	NxQuat q;
	q.fromAngleAxis(180.0f, NxVec3(0.0f, 1.0f, 0.0f));
	vehicle->getActor()->setGlobalOrientationQuat(q);

	//carbody 를 이용해서 actor 를 생성 하는 부분
	//m_pBodyModel->CreateActor(vehicle->getActor(),L"car_body");
	//vehicle->getWheel()->get
}
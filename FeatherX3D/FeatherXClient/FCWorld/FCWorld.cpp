#include "FeatherXClientPrecompile.h"

FCWorld::FCWorld()
{

}

FCWorld::~FCWorld()
{

}

F3dCModel* FCWorld::CreateObject(WCHAR * strFile)
{
	F3dCModel* pModel = CFeatherXModelManager::GetSingleton()->CreateModel(strFile);
	m_vecObject.push_back(pModel);
	return pModel;
}

void FCWorld::Init(MAP_INFO& mapInfo)
{
	m_pWorld = new F3dCWorld;
	m_pWorld->Init();

	F3dMatrix matWorld;
	F3dMatrix matRot;
	Identity(matWorld);
	Identity(matRot);

	for(auto it=mapInfo.vecElement.begin(); it!=mapInfo.vecElement.end(); ++it)
	{
		switch(it->type)
		{
		case TYPE_EFFECT:
		case TYPE_BOX:
			::SetPos(matWorld,it->vecPostion);
			::RotateEuler(matRot,it->vecRotation);
			matWorld = matRot * matWorld;
			CreateObject((WCHAR*)it->strFileName.c_str());	
			break;

		case TYPE_WALL_X :
			for(int i=0;i<3;++i) {
				for(int j=0;j<3;++j) {
					F3dVector vtxPos = F3dVector(it->vecPostion.x + (j*2.5),it->vecPostion.y + (i*2.5),it->vecPostion.z);
					::SetPos(matWorld,vtxPos);
					::RotateEuler(matRot,it->vecRotation);
					matWorld = matRot * matWorld;

					CreateObject((WCHAR*)it->strFileName.c_str());	
				}
			}
			break;

		case TYPE_WALL_Z :
			for(int i=0;i<3;++i) {
				for(int j=0;j<3;++j) {
					F3dVector vtxPos = F3dVector(it->vecPostion.x,it->vecPostion.y + (i*2.5),it->vecPostion.z + (j*2.5));
					::SetPos(matWorld,vtxPos);
					::RotateEuler(matRot,it->vecRotation);
					matWorld = matRot * matWorld;
					CreateObject((WCHAR*)it->strFileName.c_str());	
				}
			}
			break;

		case TYPE_EVENT_BOX:
			FCEventBoxManager::GetSingleton()->CreateEventBox((WCHAR*)it->strFileName.c_str(),it->nEventBoxID,it->vecPostion,it->vecRotation);
			break;
		}		
	}
}

void FCWorld::Process(double dTime,double dt)
{
	m_pWorld->Process();
}

void FCWorld::Render(double dTime,double dt)
{
	m_pWorld->Render();
}
#include "FeatherXEnginePrecompile.h"

F3dCModel::F3dCModel():
m_pNodeMgr(NULL),
m_fFrame(0.f),
m_vecDirection(1,0,0),
m_vecPosition(0,0,0)
{
	m_nDrawMode = ZNode::NODE_MESH;
	m_nDrawOption = 0;
	bAnimate = true;
	m_fFrame = 0.f;

	Identity(m_mtxWorldTrans);
}

F3dCModel::~F3dCModel()
{

}

void F3dCModel::Init()
{

}

void F3dCModel::Process()
{
	F3dCPhysics::Process();
}

void F3dCModel::Render()
{
	CFeatherXLight::GetSingleton()->SetupLights();
	SetMat();
	Animate();

	m_pNodeMgr->Draw(m_nDrawMode);
	if(m_nDrawOption & DO_BOX)
	{
		m_pNodeMgr->DrawBBox(m_nDrawMode);	
	}
}

void F3dCModel::Destroy()
{
	S_DEL(m_pNodeMgr);
}

void F3dCModel::SetMat()
{
	D3DXMATRIXA16	matTM; 
	matTM = m_mtxWorldTrans;
	m_pNodeMgr->SetTM( &matTM );
}

void F3dCModel::Animate()
{
	ZObjectInfo*	pInfo = m_pNodeMgr->GetInfo();
	m_pNodeMgr->Animate( m_fFrame );

	if( bAnimate )
	{
		m_fFrame += 1.0f;
		if( m_fFrame > pInfo->fAnimationEnd ) 
		{
			m_fFrame = pInfo->fAnimationStart;
		}
	}

	if(m_pPhysicsActor && m_pPhysicsActor->cCurType == CONTROL_STATIC)
	{
		F3dMatrix mat;
		Identity(mat);
		D3DXMATRIXA16* mat16 = m_pNodeMgr->GetNodeMat(m_physicResultAni.strLinkedNode);
		if(mat16) {
			mat = *mat16;
		}

		mat = mat * m_mtxWorldTrans;
		mat._41 += m_physicResultAni.vecOffset.x;
		mat._42 += m_physicResultAni.vecOffset.y;
		mat._43 += m_physicResultAni.vecOffset.z;
		NxActor* nxActor = mpManager::GetSingleton()->FindActor( m_pPhysicsActor->ActorID );		
		SetGlobalTM(nxActor,mat);
	}
}

void F3dCModel::Create(WCHAR* strFile)
{
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;
	IXMLDOMNodeList*  pNodeList  = NULL;
	WCHAR str[128];
	long nCount = 0;

	CFeatherXXml* pXml = CFeatherXXmlParser::GetSingleton()->GetXML();
	pXml->Open(strFile);
	pXml->GetElementText(L"Body/Info/Mesh",str);
	
	//mesh ºÎºÐ
	ZCParsedData* pData = new ZCParsedData();
	ZCParser* pParser = CFeatherXModelManager::GetSingleton()->GetParser();
	if( !pParser->Parse( pData, str ) ) {
		printf("MODEL = [%s]",strFile);
		return;
	}

	m_pNodeMgr = new ZNodeMgr( F3dCDevice::GetSingleton()->GetD3DDevice(), pData, ZNodeMgr::SKINMETHOD_SW );
	m_fFrame = m_pNodeMgr->GetInfo()->fAnimationStart;
	S_DEL( pData );

	//physic
	CONTROL_TYPE controlType;
	pXml->GetElementText(L"Body/Info/Physic/ControlType",str);
	if(_wcsicmp(str,L"CONTROL_DYNAMIC") == 0) {
		controlType = CONTROL_DYNAMIC;
	}
	else if(_wcsicmp(str,L"CONTROL_KINEMATIC") == 0) {
		controlType = CONTROL_KINEMATIC;
	}
	else if(_wcsicmp(str,L"CONTROL_STATIC") == 0) {
		controlType = CONTROL_STATIC;
	}

	pXml->GetElementText(L"Body/Info/Physic/ShapeType",str);
	int nShapeType = 0;
	if(_wcsicmp(str,L"SHAPE_BOX") == 0) {
		nShapeType = SHAPE_BOX;
	}
	else if(_wcsicmp(str,L"SHAPE_SPHERE") == 0) {
		nShapeType = SHAPE_SPHERE;
	}
	else if(_wcsicmp(str,L"SHAPE_CAPSULE") == 0) {
		nShapeType = SHAPE_CAPSULE;
	}

	switch(nShapeType)
	{
	case SHAPE_BOX:
		{
			mpBoxShapeDesc* pShapeDesc = new mpBoxShapeDesc;
			pShapeDesc->SetToDefault();
			pXml->GetElementText(L"Body/Info/Physic/Extent",str);
			pShapeDesc->vecExtent = pXml->_StrToVector3f(str);
			m_vShapeDesc.push_back(pShapeDesc);
		}
		break;
	case SHAPE_SPHERE:
		{
			mpSphereShapeDesc* pShapeDesc = new mpSphereShapeDesc;
			pShapeDesc->SetToDefault();

			pXml->GetElementText(L"Body/Info/Physic/Radius",str);
			pShapeDesc->fRadius = _wtof(str);
			m_vShapeDesc.push_back(pShapeDesc);
		}
		break;
	case SHAPE_CAPSULE:
		{
			mpCapsuleShapeDesc* pShapeDesc = new mpCapsuleShapeDesc;
			pShapeDesc->SetToDefault();

			pXml->GetElementText(L"Body/Info/Physic/Radius",str);
			pShapeDesc->fRadius = _wtof(str);

			pXml->GetElementText(L"Body/Info/Physic/Height",str);
			pShapeDesc->fHeight = _wtof(str);
			m_vShapeDesc.push_back(pShapeDesc);
		}
		break;
	}

	m_ActorDesc.SetToDefault();
	pXml->GetElementText(L"Body/Info/Physic/Density",str);
	m_ActorDesc.fDensity = _wtof(str);
	m_ActorDesc.bBody = true;
	m_ActorDesc.DominanceGroup = 1;
	m_ActorDesc.cType = controlType;
	m_ActorDesc.uContactReportFlags = NX_NOTIFY_ON_START_TOUCH;

	pXml->GetElementText(L"Body/Info/Physic/Node",str);
	m_physicResultAni.strLinkedNode = str;

	pXml->GetElementText(L"Body/Info/Physic/Offset",str);
	m_physicResultAni.vecOffset = pXml->_StrToVector3f(str);

	F3dActor* pActor = CreatePhysicsActor();
	if(pActor)
	{
		NxActor* nxActor = mpManager::GetSingleton()->FindActor( pActor->ActorID );
		pActor->cCurType = controlType;
		if(controlType == CONTROL_STATIC) {
			nxActor->raiseBodyFlag( NX_BF_KINEMATIC );
		}
	}

	//effect
	pListChild = pXml->FindElement(L"Body/Info/Effect");
	pListChild->get_length( &nCount );

	for(int i=0;i<nCount;++i)
	{
		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Node");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		std::wstring strLinkedNode = str;

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"Offset");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );
		F3dVector vecOffset = pXml->_StrToVector3f(str);

		pListChild->get_item( i, &pNodeChild );
		pNodeList = pXml->FindElement(pNodeChild,L"File");
		pNodeList->get_item( 0, &pNodeChild );
		pXml->GetElementText( pNodeChild, str );

		if(_wcsicmp(str,L"") != 0) {
			CreateFx(str,strLinkedNode,vecOffset);
		}
	}
}

void F3dCModel::CreateFx(WCHAR* strFile,std::wstring& strLinkedNode,F3dVector& vecOffset)
{
	F3dCParticle* pParticle = F3dCFxManager::GetSingleton()->Create(strFile);
	m_vecFx.push_back(pParticle);

	F3dParticleParentInfo ParentInfo;
	ParentInfo.pParentModel = this;

	ParentInfo.strLinkedNode = strLinkedNode;
	ParentInfo.vecOffset = vecOffset;

	pParticle->SetParentInfo(ParentInfo);
}

void F3dCModel::SetPos(F3dVector vecPos)
{
	m_vecPosition = vecPos;
	Update();
}

void F3dCModel::Update()
{
	F3dVector vtxUP( 0.0f, 1.0f, 0.0f );
	Identity( m_mtxWorldTrans );
	SetN(m_mtxWorldTrans,  VecNormalize(-F3dVector(m_vecDirection)));
	SetU( m_mtxWorldTrans, VecNormalize(vtxUP^GetN(m_mtxWorldTrans)) );
	SetV( m_mtxWorldTrans, VecNormalize(GetN(m_mtxWorldTrans)^GetU(m_mtxWorldTrans)) );
	::SetPos( m_mtxWorldTrans, m_vecPosition );
}

void F3dCModel::SetDir(F3dVector vecDir)
{
	m_vecDirection = vecDir;
	Update();
}

F3dVector F3dCModel::GetPos()
{
	return m_vecPosition;
}

F3dVector F3dCModel::GetDir()
{
	return m_vecDirection;
}

D3DXMATRIXA16* F3dCModel::GetNodeMat(std::wstring strName)
{
	 return m_pNodeMgr->GetNodeMat(strName);
}

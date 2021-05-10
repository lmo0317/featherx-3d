#include "FeatherXEnginePrecompile.h"

F3dCParticleTemplate::F3dCParticleTemplate()
{

}

F3dCParticleTemplate::~F3dCParticleTemplate()
{

}

void F3dCParticleTemplate::Init(WCHAR* strFile)
{
	CFeatherXXml* pXml = CFeatherXXmlParser::GetSingleton()->GetXML();
	pXml->Open(strFile);

	WCHAR str[128];
	pXml->GetElementText(L"Body/Info/Filename",str);
	strTextureFileName = str;

	pXml->GetElementText(L"Body/Info/Random",str);
	bRandom = _wtoi(str) != 0;
	
	pXml->GetElementText(L"Body/Info/BillboardLifeTime",str);
	fBillBoardLifeTime = _wtof(str);

	pXml->GetElementText(L"Body/Info/EmitterShootTimeInterval",str);
	fEmitterShootTimeInterval = _wtof(str);

	pXml->GetElementText(L"Body/Info/EmitterLifeTime",str);
	fEmitterLifeTime = _wtof(str);

	pXml->GetElementText(L"Body/Info/EmitterNumberOfShoot",str);
	nEmitterNumberOfShoot = _wtoi(str);

	pXml->GetElementText(L"Body/Info/StartColor",str);
	vec4StartColor = pXml->_StrToVector4f(str);

	pXml->GetElementText(L"Body/Info/MiddleColor",str);
	vec4MiddleColor = pXml->_StrToVector4f(str);

	pXml->GetElementText(L"Body/Info/EndColor",str);
	vec4EndColor = pXml->_StrToVector4f(str);

	pXml->GetElementText(L"Body/Info/Direction",str);
	vecDirection = pXml->_StrToVector3f(str);

	pXml->GetElementText(L"Body/Info/Gravity",str);
	vecGravity = pXml->_StrToVector3f(str);

	pXml->GetElementText(L"Body/Info/StartSize",str);
	vecStartSize = pXml->_StrToVector2f(str);

	pXml->GetElementText(L"Body/Info/EndSize",str);
	vecEndSize = pXml->_StrToVector2f(str);

	pXml->GetElementText(L"Body/Info/ShootType",str);
	nShootType = _wtoi(str);

	pXml->GetElementText(L"Body/Info/AlphaType",str);
	nAlphaType = _wtoi(str);

	pXml->GetElementText(L"Body/Info/EmitterType",str);
	nEmitterType = _wtoi(str);

	pXml->GetElementText(L"Body/Info/EmitterAxisType",str);
	nEmitterAxisType = _wtoi(str);

	pXml->GetElementText(L"Body/Info/FrameNum",str);
	nFrameNum = _wtoi(str);

	pXml->GetElementText(L"Body/Info/WidthFrameNum",str);
	nWidthFrameNum = _wtoi(str);

	pXml->GetElementText(L"Body/Info/HeightFrameNum",str);
	nHeightFrameNum = _wtoi(str);

	pXml->GetElementText(L"Body/Info/NumberOfRoutine",str);
	fNumberOfRoutine = _wtof(str);

	pXml->GetElementText(L"Body/Info/ConeAngle",str);
	fConeAngle = _wtof(str);

	pXml->GetElementText(L"Body/Info/MinRot",str);
	fMinRot = _wtof(str);

	pXml->GetElementText(L"Body/Info/MaxRot",str);
	fMaxRot = _wtof(str);

	pXml->GetElementText(L"Body/Info/MinSpeed",str);
	fMinSpeed = _wtof(str);

	pXml->GetElementText(L"Body/Info/MaxSpeed",str);
	fMaxSpeed = _wtof(str);

	pXml->GetElementText(L"Body/Info/EmitterXLength",str);
	fEmitterXLength = _wtof(str);

	pXml->GetElementText(L"Body/Info/EmitterXLength",str);
	fEmitterYLength = _wtof(str);

	pXml->GetElementText(L"Body/Info/EmitterXLength",str);
	fEmitterZLength = _wtof(str);
}
#pragma once

/************************************************************************/
/* STANDARD                                                             */
/************************************************************************/
#include <d3dx9.h>
#include <d3d9.h>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <atlstr.h>
#include <dinput.h>
#include <time.h>
#include <string>
#include <assert.h>
#include <msxml2.h>
#include <comdef.h>
#include <cmath>

/************************************************************************/
/* BOOST                                                                */
/************************************************************************/
#include <boost/unordered/unordered_map.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/

#include "MagiFileDef.h"
#include "FeatherXBasePrecompile.h"
#include "FeatherXEngineCommonPrecompile.h"
#include "clock.h"
#include "Timer.h"
#include "TimeSourceHRWin32.h"
#include "MagiPhysXDef.h"
#include "FeatherXUtility/d3dutil.h"
#include "FeatherXUtility/dxutil.h"
#include "FeatherXEngineDef.h"
#include "FeatherXAnimation/FeatherXAnimation.h"
#include "FeatherXCamera/FeatherXCamera.h"
#include "FeatherXInput/CKeyboard.h"
#include "FeatherXInput/CMouse.h"
#include "FeatherXInput/FeatherXInput.h"
#include "FeatherXLight/FeatherXLight.h"
#include "FeatherXWater/F3dCWater.h"
#include "FeatherXWater/F3dCWaterManager.h"
#include "FeatherXEffect/F3dCTail/F3dCTail.h"
#include "FeatherXEffect/F3dCTail/F3dCTailManager.h"
#include "FeatherXEffect/F3dCParticleDef.h"
#include "FeatherXEffect/F3dCParticleTemplate.h"
#include "FeatherXEffect/F3dCParticle.h"
#include "FeatherXEffect/F3dCFxManager.h"
#include "FeatherXModel/F3dCPhysics.h"
#include "FeatherXModel/xml/define.h"
#include "FeatherXModel/xml/ZCamera.h"
#include "FeatherXModel/xml/ZCMesh.h"
#include "FeatherXModel/xml/zcparseddata.h"
#include "FeatherXModel/xml/ZDefine.h"
#include "FeatherXModel/xml/ZTrack.h"
#include "FeatherXModel/xml/ZNode.h"
#include "FeatherXModel/xml/ZNodeMgr.h"
#include "FeatherXModel/xml/ZMesh.h"
#include "FeatherXModel/xml/ZRigidMesh.h"
#include "FeatherXModel/xml/ZSkinnedMesh.h"
#include "FeatherXModel/xml/ZSWSkinnedMesh.h"
#include "FeatherXModel/xml/ZXML.h"
#include "FeatherXModel/xml/ZBone.h"
#include "FeatherXModel/xml/ZCParser.h"
#include "FeatherXModel/xml/ZFFSkinnedMesh.h"
#include "FeatherXModel/F3dCModel.h"
#include "FeatherXModel/FeatherXModelManager.h"
#include "FeatherXWorld/F3dCWorldDef.h"
#include "FeatherXWorld/F3dCWorld.h"
#include "FeatherXOutputWindow/FeatherXOutputWindow.h"
#include "FeatherXPostEffect/FeatherXPostEffect.h"
#include "FeatherXShaderManager/FeatherXShader.h"
#include "FeatherXShaderManager/FeatherXShaderManager.h"
#include "FeatherXStream/FeatherXStream.h"
#include "FeatherXTexture/FeatherXTexture.h"
#include "FeatherXTexture/FeatherXTextureManager.h"
#include "FeatherXXml/FeatherXXml.h"
#include "FeatherXXml/FeatherXXmlParser.h"
#include "FeatherXEngineManager/FeatherXEngineManager.h"

#include "FeatherXTexture/F3dCTexture.h"
#include "FeatherXtexture/F3dCTextureManager.h"
#include "FeatherXTexture/F3dCTextTextureManager.h"

#include <XInput.h> // Header for XInput APIs
#include <commctrl.h>
#include <dimm.h>
#include <usp10.h>
#include <limits>
#include <fstream>
#include "tinyxml.h"
#include "FeatherXUI/F3dZUIFreeType.h"
#include "FeatherXUI/F3dZUIDef.h"
#include "FeatherXUI/F3dZUISpriteManager.h"
#include "FeatherXUI/F3dCRenderTargetUISprite.h"
#include "FeatherXUI/F3dZUIText.h"
#include "FeatherXUI/F3dZDragAndDrop.h"
#include "FeatherXUI/F3dZControl.h"
#include "FeatherXUI/Control/F3dZControlToolTip.h"
#include "FeatherXUI/F3dZWnd.h"
#include "FeatherXUI/F3dZWndManager.h"
#include "FeatherXUI/F3dZEditableControl.h"
#include "FeatherXUI/F3dZEditableWnd.h"
#include "FeatherXUI/F3dZEditableWndManager.h"
#include "FeatherXUI/F3dZUIParser.h"
#include "FeatherXUI/Control/CImm.h"
#include "FeatherXUI/Control/CInput.h"
#include "FeatherXUI/Control/F3dZControlEdit.h"
#include "FeatherXUI/Control/F3dZControlButton.h"
#include "FeatherXUI/Control/F3dZControlNumber.h"
#include "FeatherXUI/Control/F3dZControlCombo.h"
#include "FeatherXUI/Control/F3dZControlGauge.h"
#include "FeatherXUI/Control/F3dZControlListBox.h"
#include "FeatherXUI/Control/F3dZControlScroll.h"
#include "FeatherXUI/Control/F3dZControlSlider.h"
#include "FeatherXUI/Control/F3dZControlStatic.h"
#include "FeatherXUI/Control/F3dZControlTab.h"
#include "FeatherXUI/Control/F3dZControlAniStatic.h"
#include "FeatherXUI/Control/F3dZControlCoolTime.h"
#include "FeatherXUI/F3dZUIFactory.h"

#include "FeatherXRender/F3dCRenderUISprite.h"
#include "FeatherXRender/Debug/DebugRenderer.h"
#include "FeatherXRender/FeatherXRender.h"


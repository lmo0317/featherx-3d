#pragma once
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

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

/************************************************************************/
/* BOOST                                                                */
/************************************************************************/
#include "FeatherXBasePrecompile.h"

#include <boost/unordered/unordered_map.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>

#include "FeatherXEngineCommonDef.h"
#include "FeatherXDevice/F3dCDisplay.h"
#include "FeatherXDevice/F3dCDevice.h"

#include "geometry/fcoGeometry.h"
#include "util/fcoUtil.h"

#include "world/fcoWorld.h"
#include "world/Terrain.h"

#include "deviceinfo/fcoDeviceInfo.h"
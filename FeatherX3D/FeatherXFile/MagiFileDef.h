// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP �̻󿡼��� ����� ����� �� �ֽ��ϴ�.                   
#define _WIN32_WINNT 0x0501	// �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <windows.h>

/*
#include "FBSingleton.h"
#include "MBCriticalSection.h"
#include "MBMultiThreadSync.h"
*/

#include "FeatherXBasePrecompile.h"

#include "FolderAnalyse.h"
#include "MagiStream.h"
#include "MagiFileSystem.h"
#include "SimpleCodec.h"
#include "MagiFile_Singleton.h"
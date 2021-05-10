#pragma once

/************************************************************************/
/* DEFINE                                                               */
/************************************************************************/

enum 
{
	DO_BOX = 1,
};

#define IS_USEKEYBOARD  1
#define IS_USEMOUSE     2
#define IS_USEOPENCV	3
#define LBUTTON 0
#define RBUTTON 1
#define WIN32_LEAN_AND_MEAN

#define ZF_LOG_TARGET_CONSOLE			0x00000001
#define ZF_LOG_TARGET_FILE				0x00000002
#define ZF_LOG_TARGET_WINDOW			0x00000004
#define ZF_LOG_TARGET_ALL				(ZF_LOG_TARGET_CONSOLE|ZF_LOG_TARGET_FILE|ZF_LOG_TARGET_WINDOW)
#define ZF_LOG_WINDOW_TITLE				L"ZEngine F-Killer"
#define ZF_LOG_WINDOW_CX				300
#define ZF_LOG_WINDOW_CY				600

#define WIDTH 600
#define HEIGHT 600

#define RS   F3dCDevice::GetSingleton()->GetD3DDevice()->SetRenderState
#define TSS  F3dCDevice::GetSingleton()->GetD3DDevice()->SetTextureStageState
#define SAMP F3dCDevice::GetSingleton()->GetD3DDevice()->SetSamplerState

/************************************************************************/
/* LIB                                                                  */
/************************************************************************/

#pragma comment(lib,"msxml2")

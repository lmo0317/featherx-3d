
#pragma once

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// Name: DXUtil_Timer()
// Desc: Performs timer opertations. Use the following commands:
//          TIMER_RESET           - to reset the timer
//          TIMER_START           - to start the timer
//          TIMER_STOP            - to stop (or pause) the timer
//          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//          TIMER_GETABSOLUTETIME - to get the absolute system time
//          TIMER_GETAPPTIME      - to get the current time
//          TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
//                                  TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------

enum TIMER_COMMAND { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
                     TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };



class CDxUtil  
{
public:
	CDxUtil();
	virtual ~CDxUtil();
//-----------------------------------------------------------------------------
// Name: DXUtil_GetDXSDKMediaPath() and DXUtil_FindMediaFile() 
// Desc: Returns the DirectX SDK path, as stored in the system registry
//       during the SDK install.
//-----------------------------------------------------------------------------
	const TCHAR* GetDXSDKMediaPath();
	HRESULT      FindMediaFile( TCHAR* strPath, TCHAR* strFilename );

	FLOAT __stdcall Timer( TIMER_COMMAND command );


//-----------------------------------------------------------------------------
// Name: DXUtil_Read*RegKey() and DXUtil_Write*RegKey()
// Desc: Helper functions to read/write a string registry key 
//-----------------------------------------------------------------------------
	HRESULT WriteStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue );
	HRESULT WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue );
	HRESULT WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue );
	HRESULT WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue );

	HRESULT ReadStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue, DWORD dwLength, TCHAR* strDefault );
	HRESULT ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwValue, DWORD dwDefault );
	HRESULT ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidValue, GUID& guidDefault );
	HRESULT ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbValue, BOOL bDefault );

//-----------------------------------------------------------------------------
// UNICODE support for converting between CHAR, TCHAR, and WCHAR strings
//-----------------------------------------------------------------------------
	VOID ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar = -1 );
	VOID ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar = -1 );
	VOID ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
	VOID ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
	VOID ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar = -1 );
	VOID ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar = -1 );

};

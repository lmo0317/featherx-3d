// DxUtil.cpp: implementation of the CDxUtil class.
//
//////////////////////////////////////////////////////////////////////

#define STRICT

#include "FeatherXEnginePrecompile.h"

#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <stdio.h> 
#include <stdarg.h>
#include "DXUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDxUtil::CDxUtil()
{

}

CDxUtil::~CDxUtil()
{

}

const TCHAR* CDxUtil::GetDXSDKMediaPath()
{
    static TCHAR strNull[2] = _T("");
    static TCHAR strPath[MAX_PATH];
    DWORD dwType;
    DWORD dwSize = MAX_PATH;
    HKEY  hKey;

    // Open the appropriate registry key
    LONG lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                _T("Software\\Microsoft\\DirectX"),
                                0, KEY_READ, &hKey );
    if( ERROR_SUCCESS != lResult )
        return strNull;

    lResult = RegQueryValueEx( hKey, _T("DX8SDK Samples Path"), NULL,
                              &dwType, (BYTE*)strPath, &dwSize );
    RegCloseKey( hKey );

    if( ERROR_SUCCESS != lResult )
        return strNull;

    _tcscat( strPath, _T("\\Media\\") );

    return strPath;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::FindMediaFile()
// Desc: Returns a valid path to a DXSDK media file
//-----------------------------------------------------------------------------
HRESULT CDxUtil::FindMediaFile( TCHAR* strPath, TCHAR* strFilename )
{
    HANDLE file;

    if( NULL==strFilename || NULL==strPath )
        return E_INVALIDARG;

    // Check if the file exists in the current directory
    _tcscpy( strPath, strFilename );

    file = CreateFile( strPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        CloseHandle( file );
        return S_OK;
    }
    
    // Check if the file exists in the current directory
    _stprintf( strPath, _T("%s%s"), CDxUtil::GetDXSDKMediaPath(), strFilename );

    file = CreateFile( strPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        CloseHandle( file );
        return S_OK;
    }

    // On failure, just return the file as the path
    _tcscpy( strPath, strFilename );
    return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ReadStringRegKey()
// Desc: Helper function to read a registry key string
//-----------------------------------------------------------------------------
HRESULT CDxUtil::ReadStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue, 
                                 DWORD dwLength, TCHAR* strDefault )
{
    DWORD dwType;

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)strValue, &dwLength ) )
    {
        _tcscpy( strValue, strDefault );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::WriteStringRegKey()
// Desc: Helper function to write a registry key string
//-----------------------------------------------------------------------------
HRESULT CDxUtil::WriteStringRegKey( HKEY hKey, TCHAR* strRegName,
                                  TCHAR* strValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_SZ, 
                                        (BYTE*)strValue, 
                                        (_tcslen(strValue)+1)*sizeof(TCHAR) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ReadIntRegKey()
// Desc: Helper function to read a registry key int
//-----------------------------------------------------------------------------
HRESULT CDxUtil::ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwValue, 
                              DWORD dwDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)pdwValue, &dwLength ) )
    {
        *pdwValue = dwDefault;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::WriteIntRegKey()
// Desc: Helper function to write a registry key int
//-----------------------------------------------------------------------------
HRESULT CDxUtil::WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
                                        (BYTE*)&dwValue, sizeof(DWORD) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ReadBoolRegKey()
// Desc: Helper function to read a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT CDxUtil::ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbValue, 
                              BOOL bDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(BOOL);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)pbValue, &dwLength ) )
    {
        *pbValue = bDefault;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::WriteBoolRegKey()
// Desc: Helper function to write a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT CDxUtil::WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
                                        (BYTE*)&bValue, sizeof(BOOL) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ReadGuidRegKey()
// Desc: Helper function to read a registry key guid
//-----------------------------------------------------------------------------
HRESULT CDxUtil::ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidValue, 
                               GUID& guidDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(GUID);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (LPBYTE) pGuidValue, &dwLength ) )
    {
        *pGuidValue = guidDefault;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::WriteGuidRegKey()
// Desc: Helper function to write a registry key guid
//-----------------------------------------------------------------------------
HRESULT CDxUtil::WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_BINARY, 
                                        (BYTE*)&guidValue, sizeof(GUID) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::Timer()
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
FLOAT __stdcall CDxUtil::Timer( TIMER_COMMAND command )
{
    static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF         = FALSE;
    static LONGLONG m_llQPFTicksPerSec  = 0;

    // Initialize the timer
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
        // not supported, we will timeGetTime() which returns milliseconds.
        LARGE_INTEGER qwTicksPerSec;
        m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
        if( m_bUsingQPF )
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if( m_bUsingQPF )
    {
        static LONGLONG m_llStopTime        = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime        = 0;
        double fTime;
        double fElapsedTime;
        LARGE_INTEGER qwTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {
            fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
            m_llLastElapsedTime = qwTime.QuadPart;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
            return (FLOAT) fAppTime;
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_llBaseTime        = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            m_llBaseTime += qwTime.QuadPart - m_llStopTime;
            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            m_llStopTime = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_llStopTime += m_llQPFTicksPerSec/10;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
    else
    {
        // Get the time using timeGetTime()
        static double m_fLastElapsedTime  = 0.0;
        static double m_fBaseTime         = 0.0;
        static double m_fStopTime         = 0.0;
        double fTime;
        double fElapsedTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            fTime = m_fStopTime;
        else
            fTime = timeGetTime() * 0.001;
    
        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {   
            fElapsedTime = (double) (fTime - m_fLastElapsedTime);
            m_fLastElapsedTime = fTime;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            return (FLOAT) (fTime - m_fBaseTime);
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_fBaseTime         = fTime;
            m_fLastElapsedTime  = fTime;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            m_fBaseTime += fTime - m_fStopTime;
            m_fStopTime = 0.0f;
            m_fLastElapsedTime  = fTime;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            m_fStopTime = fTime;
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_fStopTime += 0.1f;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ConvertAnsiStringToWide()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID CDxUtil::ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar )
{
    if( wstrDestination==NULL || strSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = strlen(strSource)+1;

    MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
                         wstrDestination, cchDestChar-1 );

    wstrDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID CDxUtil::ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
                         cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ConvertGenericStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID CDxUtil::ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( strDestination==NULL || tstrSource==NULL )
        return;

#ifdef _UNICODE
    CDxUtil::ConvertWideStringToAnsi( strDestination, tstrSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    	strcpy( strDestination, tstrSource );
    else
    	strncpy( strDestination, tstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ConvertGenericStringToWide()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID CDxUtil::ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( wstrDestination==NULL || tstrSource==NULL )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
	    wcscpy( wstrDestination, tstrSource );
    else
	    wcsncpy( wstrDestination, tstrSource, cchDestChar );
#else
    CDxUtil::ConvertAnsiStringToWide( wstrDestination, tstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID CDxUtil::ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || strSource==NULL )
        return;
        
#ifdef _UNICODE
    CDxUtil::ConvertAnsiStringToWide( tstrDestination, strSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    	strcpy( tstrDestination, strSource );
    else
    	strncpy( tstrDestination, strSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: CDxUtil::ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID CDxUtil::ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || wstrSource==NULL )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
	    wcscpy( tstrDestination, wstrSource );
    else
	    wcsncpy( tstrDestination, wstrSource, cchDestChar );
#else
    CDxUtil::ConvertWideStringToAnsi( tstrDestination, wstrSource, cchDestChar );
#endif
}

#include "FeatherXBasePrecompile.h"
#include "mbdef.h"
#include <algorithm>
#include <cctype>       // std::tolower
#include "MBStringUtill.h"

#pragma warning ( disable : 4996 )

namespace MBStringUtil
{
	int WC2AC( char *mbstr, int fRadiusInBytes, const wchar_t *wcstr, int count )
	{
		 
		WideCharToMultiByte( CP_ACP,				// code page
			0,						// performance and mapping flags
			wcstr,					// wide-character string
			count,					// number of chars in string.
			mbstr,					// buffer for new string
			fRadiusInBytes,         // size of buffer
			NULL,					// default for unmappable chars
			NULL );					// set when default char used
		return 0;
	}

	int AC2WC( wchar_t *wcstr, int nRadiusInWords, const char *mbstr, int count )
	{
		 
		//	size_t ConvertedChars;
		//	errno_t err = mbstowcs_s( &ConvertedChars, wcstr, fRadiusInWords, mbstr, count );
		//  return err;

		MultiByteToWideChar(CP_ACP,					// code page
			MB_PRECOMPOSED,         // character-type options
			mbstr,					// string to map
			count,					// number of bytes in string
			wcstr,					// wide-character buffer
			nRadiusInWords			// fRadius of buffer
			);
		return 0;
	}

	std::wstring GUID2WString( const GUID& guid )
	{
		wchar_t szStr[50] = { 0, };

		//01111111-0222-0333-0455-055555555555 : 테스트 꼭필요
		//11111111-2222-3333-4455-555555555555
		swprintf_s( &szStr[0], 50, L"%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X", guid.Data1, guid.Data2, guid.Data3,guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );
		return std::wstring( szStr );
	}

	std::string GUID2String( const GUID& guid )
	{
		char szStr[50] = { 0, };

		//01111111-0222-0333-0455-055555555555 : 테스트 꼭필요
		//11111111-2222-3333-4455-555555555555
		sprintf_s( &szStr[0], 50, "%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X", guid.Data1, guid.Data2, guid.Data3,guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );
		return std::string( szStr );
	}

	void TransformLower( std::wstring & str )
	{
		std::transform( str.begin(), str.end(), str.begin(), (int(*)(int)) std::tolower);
	}

	void TransformUpper( std::wstring & str )
	{
		std::transform( str.begin(), str.end(), str.begin(), (int(*)(int)) std::toupper);
	}

	void ToLower( std::wstring & str )
	{
		size_t pos = 0;
		size_t len = str.length();

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= L'A' && str[ pos ] <= L'Z' ) str[ pos ] -= ( L'A' - L'a' );
		}
	}

	void ToUpper( std::wstring & str )
	{
		size_t pos = 0;
		size_t len = str.length();

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= L'a' && str[ pos ] <= L'z' ) str[ pos ] += ( L'A' - L'a' );
		}
	}

	void ToLower( std::string & str )
	{
		size_t pos = 0;
		size_t len = str.length();

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= 'A' && str[ pos ] <= 'Z' ) str[ pos ] -= ( 'A' - L'a' );
		}
	}

	void ToUpper( std::string & str )
	{
		size_t pos = 0;
		size_t len = str.length();

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= 'a' && str[ pos ] <= 'z' ) str[ pos ] += ( 'A' - L'a' );
		}
	}

	void ToLower( wchar_t* str, int nLen )
	{
		size_t pos = 0;
		size_t len = nLen;

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= L'A' && str[ pos ] <= L'Z' ) str[ pos ] -= ( L'A' - L'a' );
		}
	}
	void ToUpper( wchar_t* str, int nLen )
	{
		size_t pos = 0;
		size_t len = nLen;

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= L'a' && str[ pos ] <= L'z' ) str[ pos ] += ( L'A' - L'a' );
		}
	}

	void ToLower( char* str, int nLen )
	{
		size_t pos = 0;
		size_t len = nLen;

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= 'A' && str[ pos ] <= 'Z' ) str[ pos ] -= ( 'A' - 'a' );
		}
	}
	void ToUpper( char* str, int nLen )
	{
		size_t pos = 0;
		size_t len = nLen;

		for( pos = 0; pos < len; ++pos )
		{
			if( str[ pos ] >= 'a' && str[ pos ] <= 'z' ) str[ pos ] += ( 'A' - 'a' );
		}
	}
	bool IsLower( const WCHAR* str, int nLen )
	{
		for( int i = 0; i < nLen; ++i )
		{
			if( str[i] >= L'A' && str[i] <= L'Z' ) {
				return false;
			}
		}
		return true;
	}

void SplitLine( std::vector<std::wstring>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar )
{ 
	 
	unsigned len = (unsigned)wcslen(pBuf)+1;
	int nSplitCharCnt = (int)wcslen( pSplitChar );

	std::wstring strTmp;
	int i = 0;
	for( int j(0); (int)len>j; ++j )
	{
		for( i=0; nSplitCharCnt>i; ++i )
		{
			if( pBuf[j] == pSplitChar[i] )
			{
				if( strTmp.size() ) { 
					strlist.push_back( strTmp );
					strTmp = L"";
				}
				break;
			}
		}

		if( i == nSplitCharCnt && pBuf[j] != NULL ) {
			if( strTmp.empty() == false || pBuf[j] != L' ' ) {
				strTmp += pBuf[j];
			}
		}
	}


	//나머지
	if( strTmp.size() ) { 
		strlist.push_back( strTmp );
		strTmp = L"";
	}
}

void SplitByCount(std::wstring& pDest,std::wstring& pSrc,int nCount)
{
	wchar_t szTempFile[512];
	ZeroMemory( szTempFile, sizeof(szTempFile) );
	size_t nIndex = 0;

	for( int i = 0; i < nCount; ++i )
	{
		szTempFile[i] = pSrc[i];
	}
	pDest = szTempFile;
}

void SplitLine2( std::vector<std::wstring>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar, int nSplitCount )
{ 
	 
	unsigned len = (unsigned)wcslen(pBuf)+1;
	int nSplitCharCnt = (int)wcslen( pSplitChar );

	std::wstring strTmp;
	int i = 0;
	for( int j(0); (int)len>j; ++j )
	{
		for( i=0; nSplitCharCnt>i; ++i )
		{
			if( pBuf[j] == pSplitChar[i] )
			{
				if( (int)strlist.size() < nSplitCount )
				{
					if( strTmp.size() ) { 
						strlist.push_back( strTmp );
						strTmp = L"";
					}
					break;
				}
			}
		}

		if( i == nSplitCharCnt && pBuf[j] != NULL ) {
			if( strTmp.empty() == false || pBuf[j] != L' ' ) {
				strTmp += pBuf[j];
			}
		}
	}

	//나머지
	if( strTmp.size() ) { 
		strlist.push_back( strTmp );
		strTmp = L"";
	}
}

int GetSplitCount( const wchar_t* pBuf, const wchar_t* pSplitChar )
{ 
	 
	unsigned len = (unsigned)wcslen(pBuf)+1;
	int nSplitCharCnt = (int)wcslen( pSplitChar );
	int nSpaceCount = 0;

	int i = 0;
	for( int j(0); (int)len>j; ++j )
	{
		for( i=0; nSplitCharCnt>i; ++i )
		{
			if( pBuf[j] == pSplitChar[i] )
			{
				nSpaceCount++;
				break;
			}
		}
	}

	return nSpaceCount;
}

//String
void SplitBuf( std::vector<std::string>& strlist, const char* pBuf, const int nLen, const char* pSplitChar )
{ 
	 
	int len = nLen;
	int nSplitCharCnt = (int)strlen( pSplitChar );

	std::string strTmp;

	int i = 0;
	for( int j(0); (int)len>j; ++j )
	{
		for( i=0; nSplitCharCnt>i; ++i )
		{
			if( pBuf[j] == pSplitChar[i] )
			{
				if( strTmp.size() ) {
					strlist.push_back( strTmp );
					strTmp.erase( strTmp.begin(), strTmp.end() );
				}
				break;
			}
		}

		if( i == nSplitCharCnt )
			strTmp += pBuf[j];
	}

	if( strTmp.size() ) {
		strlist.push_back( strTmp );
		strTmp.erase( strTmp.begin(), strTmp.end() );
	}
}

void SplitInt( std::vector<int>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar )
{
	std::vector< std::wstring > vwszSplit;
	MBStringUtil::SplitLine( vwszSplit, pBuf, pSplitChar );
	for( UINT i = 0; i < vwszSplit.size(); ++i )
	{
		int n = _wtoi( vwszSplit[i].c_str() );
		strlist.push_back( n );
	}
}

void StringFormat( std::wstring& out_string, const wchar_t* pFormat, ... )
{ 
	 
	va_list argptr;
	wchar_t szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );

	out_string = szStr;
}

std::string StringFormat( const char* pFormat, ... )
{ 
	 
	va_list argptr;
	char szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscprintf( pFormat, argptr )+1;
	vsprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );

	return std::string( szStr );
}

std::wstring StringFormat( const wchar_t* pFormat, ... )
{ 
	 
	va_list argptr;
	wchar_t szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );

	return std::wstring( szStr );
}

void GetFileName( std::wstring& out_string, const wchar_t* pFullName )
{ 
	 
	static const std::wstring::size_type npos = -1;
	std::wstring strTemp = pFullName;
	std::wstring::size_type pos;

	pos = strTemp.rfind( L"\\" );
	if( pos != npos ) {
		strTemp.erase( 0, pos+1 );
	}

	out_string = strTemp;
}

void SetFileExtW( std::wstring& out_string, const wchar_t* szFileName, const wchar_t* szExt )
{
	 
	size_t Length = wcslen( szFileName );
	if( Length < 2 )
		return;

	wchar_t szTempFile[260];
	ZeroMemory( szTempFile, sizeof(szTempFile) );
	size_t nIndex = 0;
	for( size_t i = 0; i < Length; ++i )
	{
		if( szFileName[i] == L'.' ) {
			break;
		}
		szTempFile[i] = szFileName[i];
	}

	out_string = szTempFile;
	out_string += szExt;
}

void GetFileName_WithoutExt( std::string& out_string, const char* szFileName )
{
	 
	size_t Length = strlen( szFileName );
	if( Length < 2 )
		return;

	char szTempFile[260];
	ZeroMemory( szTempFile, sizeof(szTempFile) );
	size_t nIndex = 0;
	for( size_t i = 0; i < Length; ++i )
	{
		if( szFileName[i] == '.' ) {
			break;
		}
		szTempFile[i] = szFileName[i];
	}

	out_string = szTempFile;
}

void SetFileExt( std::string& out_string, const char* szFileName, const char* szExt )
{
	 

	GetFileName_WithoutExt( out_string, szFileName );
	out_string += szExt;
}

void ParserLine( std::vector<std::wstring>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar )
{
	 
	strlist.clear();
	WCHAR szTemp[256];
	std::vector<std::wstring> vComma, vWave;
	SplitLine( vComma, pBuf, pSplitChar );
	std::vector<std::wstring>::iterator it = vComma.begin();
	for(; it != vComma.end(); ++it )
	{
		vWave.clear();
		SplitLine( vWave, it->c_str(), L"~" );
		switch( vWave.size() )
		{
		case 0:
			continue;
		case 1:
			strlist.push_back( vWave[0] );
			break;
		case 2:
			{
				int nLength = (int)vWave[0].length();
				int nLength2 = (int)vWave[1].length();
				assert( nLength == nLength2 );
				std::wstring strS = vWave[0];
				strS.erase( nLength-2, 2 );
				int nStart = (vWave[0].at( nLength-2 )-L'0')*10+(vWave[0].at( nLength-1 ) - L'0');
				int nEnd = (vWave[1].at( nLength-2 )-L'0')*10+(vWave[1].at( nLength-1 ) - L'0');
				for( int i = nStart; i <= nEnd; ++i )
				{
					swprintf_s( szTemp, L"%02d", i );
					std::wstring strT = strS;
					strT += szTemp;
					strlist.push_back( strT );
				}
			}
			break;
		default:
			continue;
		};
	}
}

std::wstring mbs_to_wcs(std::string const& str)
{
	WCHAR szTemp[1024] = {0,};

	const char* pTemp = str.c_str();
	int nLength = (int)strlen( pTemp )+1;

	MBStringUtil::AC2WC( szTemp, 1024, pTemp, nLength );

	return std::wstring(&szTemp[0]);
}

std::string wcs_to_mbs(std::wstring const& str)
{
	char szTemp[1024] = { 0, };			

	const WCHAR* wszTemp = str.c_str();
	int nLength = (int)wcslen( wszTemp )+1;

	WC2AC( szTemp, 1024, wszTemp, nLength );

	return std::string(&szTemp[0]);
}

};


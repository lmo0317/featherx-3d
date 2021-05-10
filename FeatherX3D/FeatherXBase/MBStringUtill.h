
#pragma once

#include <string>
#include <vector>
#include <tchar.h>

//String Util
namespace MBStringUtil
{
	int WC2AC( char *mbstr, int sizeInBytes, const wchar_t *wcstr, int count );
	int AC2WC( wchar_t *wcstr, int sizeInWords, const char *mbstr, int count );

	void SplitLine( std::vector<std::wstring>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar );
	void SplitLine2( std::vector<std::wstring>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar, int nSplitCount );
	void SplitBuf( std::vector<std::string>& strlist, const char* pBuf, const int nLen, const char* pSplitChar );
	int GetSplitCount( const wchar_t* pBuf, const wchar_t* pSplitChar );

	void SplitInt( std::vector<int>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar );
	void SplitByCount(std::wstring& pDest,std::wstring& pSrc,int nCount);

	void StringFormat( std::wstring& out_string, const wchar_t* pFormat, ... );
	std::wstring StringFormat( const wchar_t* pFormat, ... );
	std::string StringFormat( const char* pFormat, ... );

	std::wstring GUID2WString( const GUID& guid );
	std::string GUID2String( const GUID& guid );

	void GetFileName( std::wstring& out_string, const wchar_t* pFullName );

	void TransformLower( std::wstring & str );
	void TransformUpper( std::wstring & str );

	void ToLower( std::wstring & str );
	void ToUpper( std::wstring & str );

	void ToLower( std::string & str );
	void ToUpper( std::string & str );

	void ToLower( wchar_t* str, int nLen );
	void ToUpper( wchar_t* str, int nLen );

	void ToLower( char* str, int nLen );
	void ToUpper( char* str, int nLen );

	bool IsLower( const WCHAR* str, int nLen );

	void GetFileName_WithoutExt( std::string& out_string, const char* szFileName );
	void SetFileExtW( std::wstring& out_string, const wchar_t* szFileName, const wchar_t* szExt );
	void SetFileExt( std::string& out_string, const char* szFileName, const char* szExt );

#ifdef UNICODE
	inline int Replace( std::wstring& str, const TCHAR* pszFrom, size_t unFromLen, const TCHAR* pszTo, size_t unToLen )
#else
	inline int Replace( std::string& str, const TCHAR* pszFrom, size_t unFromLen, const TCHAR* pszTo, size_t unToLen )
#endif
	{
		int cnt  = 0;
		size_t nPos = 0;

		while( true )	{
			nPos = str.find( pszFrom, nPos );
#ifdef UNICODE
			if( nPos == std::wstring::npos ) break;
#else
			if( nPos == std::string::npos ) break;
#endif
			str.replace( nPos, unFromLen, pszTo );
			nPos += unToLen;

			cnt++;
		}
		return cnt;
	}
/*
	inline int Replace( std::wstring& str, const wchar_t* pszFrom, size_t unFromLen, const wchar_t* pszTo, size_t unToLen )
	{
		int cnt  = 0;
		size_t nPos = 0;
		
		while( true )	{
			nPos = str.find( pszFrom, nPos );
			if( nPos == std::wstring::npos ) break;
		
			str.replace( nPos, unFromLen, pszTo );
			nPos += unToLen;
		
			cnt++;
		}
		return cnt;
	}
*/
#ifdef UNICODE
	inline int Replace( std::wstring& str, const std::wstring& strFrom, const std::wstring& strTo )	{
#else
	inline int Replace( std::string& str, const std::string& strFrom, const std::string& strTo )	{
#endif
		return Replace( str, strFrom.c_str(), strFrom.size(), strTo.c_str(), strTo.size() );
	}

#ifdef UNICODE
	inline int Replace( std::wstring& str, const TCHAR* pszFrom, const TCHAR* pszTo )
#else
	inline int Replace( std::string& str, const TCHAR* pszFrom, const TCHAR* pszTo )
#endif
	{
		return Replace( str, pszFrom, _tcslen( pszFrom ), pszTo, _tcslen( pszTo ) );
	}
/*
	inline int Replace( std::wstring& str, const wchar_t* pszFrom, const wchar_t* pszTo )	{
		return Replace( str, pszFrom, wcslen( pszFrom ), pszTo, wcslen( pszTo ) );
	inline int Replace( wchar_t* pszBuf, unsigned unBufCount, const wchar_t* pszFrom, const wchar_t* pszTo )	{
	std::wstring strOriginal( pszBuf );

	int nCnt = Replace( strOriginal, pszFrom, wcslen( pszFrom ), pszTo, wcslen( pszTo ) );

	if( strOriginal.size() > unBufCount-1 ) return -1;

	memcpy( pszBuf, strOriginal.c_str(), strOriginal.size() );

	return nCnt;
	}
}
*/
	inline int Replace( TCHAR* pszBuf, unsigned unBufCount, const TCHAR* pszFrom, const TCHAR* pszTo )	{
#ifdef UNICODE
		std::wstring strOriginal( pszBuf );
#else
		std::string strOriginal( pszBuf );
#endif

		int nCnt = Replace( strOriginal, pszFrom, _tcslen( pszFrom ), pszTo, _tcslen( pszTo ) );

		if( strOriginal.size() > unBufCount-1 ) return -1;

		memcpy( pszBuf, strOriginal.c_str(), strOriginal.size() );

		return nCnt;
	}

	inline int Replace( TCHAR* pszBuf, const TCHAR* pszFrom, const TCHAR* pszTo )	{
		size_t unFromLen = _tcslen( pszFrom );
		size_t unToLen	= _tcslen( pszTo );

		if( unToLen > unFromLen ) return -1;
#ifdef UNICODE
		std::wstring strOriginal( pszBuf );
#else
		std::string strOriginal( pszBuf );
#endif
			
		int nCnt = Replace( strOriginal, pszFrom, unFromLen, pszTo, unToLen );

		memcpy( pszBuf, strOriginal.c_str(), strOriginal.size() );

		return nCnt;
	}
	void ParserLine( std::vector<std::wstring>& strlist, const wchar_t* pBuf, const wchar_t* pSplitChar = L"," );

	std::wstring mbs_to_wcs(std::string const& str);
	std::string wcs_to_mbs(std::wstring const& str);
};

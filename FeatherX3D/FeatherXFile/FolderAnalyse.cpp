
#include "MagiFileDef.h"

#include <process.h>
#include <assert.h>

#include <algorithm>
#include <cctype>       // std::tolower

//#include <new>
#include <iostream>
#include <TIME.H>

#include "MBStringUtill.h"
#include "MBfasttime.h"
#include "MagiZlib.h"
#include "MBCLog.h"
#include "MagiResDef.h"

using namespace LOG_UTIL;
using namespace MBStringUtil;

namespace
{
	const int	c_nUnitMegaSize = 1024*1024;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void FolderAnalyse::CreateSubDirectory( TCHAR* pFullPath )
{  
	TCHAR* pCheckPath = pFullPath;

	while( true )
	{
		if( *pCheckPath == TEXT('\0') )
			break;

		if( *pCheckPath == TEXT('\\') )
		{
			*pCheckPath = TEXT('\0');

			//Check Dir & Make Dir
			DWORD dwAttr = GetFileAttributes( pFullPath );

			if( dwAttr == INVALID_FILE_ATTRIBUTES )
			{
				if( !CreateDirectory( pFullPath, NULL ) )
				{
					DWORD dwErr = GetLastError();
					assert( 0 && "CreateDirectory Failed" );
				}
			}

			*pCheckPath = TEXT('\\');
		}

		++pCheckPath;
	}
}

void StringFormat( char* pBuf, const char *str, ... )
{  
	char szBuf[1024];
	va_list va;
	va_start( va, str );
	vsnprintf_s( szBuf, sizeof(szBuf), _TRUNCATE, str, va );
	va_end( va );

	strncpy_s( pBuf, 1024, szBuf, 1024 );
}

void _debug_printA( const char *str, ... )
{  
	char szBuf[1024];
	va_list va;
	va_start( va, str );
	vsnprintf_s( szBuf, sizeof(szBuf), _TRUNCATE, str, va );
	va_end( va );

	OutputDebugStringA( szBuf );
}

void _debug_print( const TCHAR *str, ... )
{  
	TCHAR szBuf[2048];
	va_list va;
	va_start( va, str );
	_vsntprintf_s( szBuf, sizeof(szBuf), str, va );
	va_end( va );

	OutputDebugString( szBuf );

#ifdef _CONSOLE
	_tprintf( szBuf );
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

FolderWorkList::FolderWorkList()
{  

}
FolderWorkList::~FolderWorkList()
{  

}

void FolderWorkList::AddFolder( TCHAR* pTStr )
{ 
	 
#ifdef UNICODE
	std::wstring strFolder = pTStr;
#else
	std::string  strFolder = pTStr;
#endif

	m_vFolder.push_back( strFolder );
}
void FolderWorkList::AddFile( const TCHAR* pDir, int nDirLen, const TCHAR* pFile, int nFileLen )
{ 
	 
	FILE_DATA file_data;

	char szDir[_MAX_PATH]   = {0,};
	char szFile[_MAX_FNAME] = {0,};

#ifdef UNICODE
	WC2AC( szDir , _MAX_PATH , pDir , nDirLen  );
	WC2AC( szFile, _MAX_FNAME, pFile, nFileLen );
#endif

	file_data.strDir  = szDir; 
	file_data.strFile = szFile;
	
	m_vFile.push_back( file_data );
}

void FolderWorkList::MakeFile()
{  
	WCHAR szBuf[32] = {0,};

	struct tm   _newtime;
	time_t      szClock; 

	// Get time in seconds
	time( &szClock );

	// Convert time to struct tm form 
	if( 0 != localtime_s( &_newtime, &szClock ) )
		return;

	int nYear = 1900 + _newtime.tm_year;
	int nMon  = _newtime.tm_mon+1;
	int nDay  = _newtime.tm_mday;
	int nH    = _newtime.tm_hour;
	int nM    = _newtime.tm_min;
	int nS    = _newtime.tm_sec;

	MagiFileStream stream( MBStringUtil::StringFormat( _T("Packing_List_%04d_%02d_%02d_%02d_%02d_%02d.txt"), nYear, nMon, nDay, nH, nM, nS ).c_str(), _T("wt") );
	if( stream.IsValid() ) {
		
		std::vector< FILE_DATA >::iterator it = m_vFile.begin();
		for( ; it != m_vFile.end(); ++it ) {
			
			stream.Write( (*it).strDir.c_str() , (*it).strDir.length()  );
			stream.Write( (*it).strFile.c_str(), (*it).strFile.length() );
			stream.Write( "\n", strlen("\n") );
		}
	}		
}
#ifdef UNICODE
void FolderWorkList::GetReNameDir( const TCHAR* pFromDir, const TCHAR* pToDir, std::vector<std::wstring>& updirList )
#else
void FolderWorkList::GetReNameDir( const TCHAR* pFromDir, const TCHAR* pToDir, std::vector<std::string>& updirList )
#endif
{  
	for( UINT i(0); m_vFolder.size()>i; ++i ) {

#ifdef UNICODE
		std::wstring dir = m_vFolder[i];
#else
		std::string dir = m_vFolder[i];
#endif

		MBStringUtil::Replace( dir, pFromDir, pToDir );

		MBStringUtil::Replace( dir, _T("\\"), _T("/") );

		updirList.push_back( dir );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef UNICODE
bool FolderAnalyse::IsExceptionList( std::wstring& str, TCHAR* pFind )
#else
bool FolderAnalyse::IsExceptionList( std::string& str, TCHAR* pFind )
#endif
{  
	size_t nPos = 0;
	while( true )	{
		nPos = str.find( pFind, nPos );
		if( nPos == std::wstring::npos ) break;
		return true;
	}

	return false;
}


DWORD AddFolder( const TCHAR* pDir, FolderAnalyse* pAnalyse, int& nAddFileCount, int& nFolderCount )
{  
	TCHAR szDir[_MAX_PATH] = {0,};

	pAnalyse->AddDir( pDir );

	_tcsncpy_s( szDir, pDir, sizeof(TCHAR)*_MAX_PATH );
	_tcscat_s( szDir, TEXT("*.*") );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;

	hFind = FindFirstFile( szDir, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		dwError = GetLastError();
		return dwError;
	} 

	TCHAR szTemp[_MAX_PATH] = {0,};
	while (FindNextFile(hFind, &FindFileData) != 0) 
	{
		if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( _tcsicmp( FindFileData.cFileName, TEXT("." ) )   == 0 ||
				_tcsicmp( FindFileData.cFileName, TEXT("..") )   == 0 ||
				_tcsicmp( FindFileData.cFileName, TEXT(".svn") ) == 0 )
				continue;
			
			_tcsncpy_s( szTemp, pDir, sizeof(TCHAR)*_MAX_PATH );
			_tcscat_s( szTemp, FindFileData.cFileName );
			_tcscat_s( szTemp, TEXT("\\") );

			++nFolderCount;
			DWORD dwErr = AddFolder( szTemp, pAnalyse, nAddFileCount, nFolderCount );
			if( dwErr != ERROR_NO_MORE_FILES )
			{
				_MBASSERT(0);
			}

			FolderWorkList::GetSingleton()->AddFolder( szTemp );

			LOG_UTIL::_out_console( TEXT("FOLDER : %s\n"), szTemp );
		}
		else
		{
			if( pAnalyse->IsUseFilter() )
			{
				//압축 화일 아님
				if( _tcsicmp( &FindFileData.cFileName[_tcsnlen( FindFileData.cFileName, _MAX_PATH ) - 4], TEXT(".mip") ) )
					continue;				
			}

			pAnalyse->AddFile( pDir, FindFileData.cFileName );
			++nAddFileCount;

			FolderWorkList::GetSingleton()->AddFile( pDir, (int)_tcsclen(pDir), FindFileData.cFileName, (int)_tcsclen(FindFileData.cFileName) );

			LOG_UTIL::_out_console( TEXT("FILE : %s %s\n"), pDir, FindFileData.cFileName );
		}
	}

	dwError = GetLastError();
	FindClose(hFind);
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		pAnalyse->SetError( dwError );
		return dwError;
	}

	return dwError;
}



unsigned __stdcall analyse_folder( void* pArg )
{  
	DWORD start_time = getMilliSecTime();

	FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);
	int nFolderCount, nFileCount;
	nFolderCount = nFileCount = 0;

	DWORD dwError = AddFolder( pAnalyse->GetRoot(), pAnalyse, nFileCount, nFolderCount );

	LOG_UTIL::_out_console( TEXT("FILE : %d, FOLDER : %d\n"), nFileCount, nFolderCount );

	pAnalyse->OnAnalyseComplete();

//	FolderWorkList::GetSingleton()->MakeFile();

	DWORD end_time = getMilliSecTime() - start_time;
	LOG_UTIL::_out_console( TEXT("ANALYSE TIME : %d\n"), end_time );

	return nFileCount;
}

unsigned __stdcall packing_folder( void* pArg )
{  
	unsigned char* pSrcBuf = NULL;
	unsigned char* pDstBuf = NULL;

	try
	{
		DWORD start_time = getMilliSecTime();

		FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);
		const TCHAR* pRoot = pAnalyse->GetRoot();
		const TCHAR* pOut  = pAnalyse->GetOut();

		std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();

		TCHAR xor_fullPath[_MAX_PATH] = {0,};
		TCHAR fullPath[_MAX_PATH] = {0,};
		TCHAR tempPath[_MAX_PATH] = {0,};

		TCHAR package_path[_MAX_PATH] = {0,};
		TCHAR package_name[_MAX_PATH] = {0,};

		//File List 저장용
//		char  strTmp[1024] = {0,};
//		char  strFullPath[_MAX_PATH] = {0,};

		_tcscpy_s( fullPath, _MAX_PATH, pOut );
		_tcscat_s( fullPath, _MAX_PATH, TEXT(c_ResNormalIdxFile) );
		
		_tcscpy_s( xor_fullPath, _MAX_PATH, pOut );
		_tcscat_s( xor_fullPath, _MAX_PATH, TEXT(c_ResIdxFile) );

		MagiFileStream file_list( fullPath, _T("wb+") );
		MagiFileStream xor_file_list( xor_fullPath, _T("wb") );

		MagiFileStream package_file[c_nPackageCount];
		unsigned __int64 uOffSet[c_nPackageCount];

		int nCount[c_nPackageCount];
		for( int i(0); c_nPackageCount>i; i++ )
		{
			nCount[i] = 0;
			uOffSet[i] = 0;
			_stprintf_s( package_name, _T("%03d.pck"), i );
			_tcscpy_s( package_path, _MAX_PATH, pOut );
			_tcscat_s( package_path, _MAX_PATH, package_name );

			package_file[i].Open( package_path, _T("wb") );
		}
		int nFileCount  = 0;
		int nTotalCount = (int)filelist.size()-1;
		for( ; it != filelist.end(); ++it )
		{
			++nFileCount;

			//exe, dll, miles 파일은 폴더에 바로 복사
			if( FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".exe") ) ||
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".dll") ) ||
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".flt") ) ||  
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".asi") ) ||
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".ico") ) ||
				!_tcsicmp( (*it).strFileName.c_str(), _T("Service.dat.mip") ) ||
				!_tcsicmp( (*it).strFileName.c_str(), _T("steam_appid.txt.mip") ) )
			{
				_tcscpy_s( fullPath, _MAX_PATH, (*it).strDirName.c_str() );
				_tcscat_s( fullPath, _MAX_PATH, (*it).strFileName.c_str() );
#ifdef UNICODE
				std::wstring strSubDir = (*it).strDirName.substr( wcslen(pRoot), (*it).strDirName.length() );
#else
				std::string strSubDir = (*it).strDirName.substr( strlen(pRoot), (*it).strDirName.length() );
#endif

				_tcscpy_s( tempPath, _MAX_PATH, pOut );
				if( strSubDir.length() )
					_tcscat_s( tempPath, _MAX_PATH, strSubDir.c_str() );
				_tcscat_s( tempPath, _MAX_PATH, (*it).strFileName.c_str() );

				//하위 폴더 생성
				FolderAnalyse::CreateSubDirectory( tempPath );

				CopyFile( fullPath, tempPath, FALSE );
				continue;
			}

			//소문자 변경
			std::transform((*it).strDirName.begin() , (*it).strDirName.end() , (*it).strDirName.begin() , (int(*)(int)) std::tolower);
			std::transform((*it).strFileName.begin(), (*it).strFileName.end(), (*it).strFileName.begin(), (int(*)(int)) std::tolower);

			_tcscpy_s( fullPath, _MAX_PATH, (*it).strDirName.c_str() );
			_tcscat_s( fullPath, _MAX_PATH, (*it).strFileName.c_str() );

			LOG_UTIL::_out_console( _T("Packing... %d/%d = %s\n"), nFileCount, nTotalCount, fullPath );

			MagiFileStream it_file( fullPath, TEXT("rb") );
			size_t file_size = (size_t)it_file.GetLength();

			pSrcBuf = new unsigned char[file_size];
			_AUTO_ARRAY_<unsigned char> autobomb( pSrcBuf );

			it_file.Read( pSrcBuf, sizeof(unsigned char)*file_size );

			unsigned long dwCrc32 = SimpleXORCodec::GetCRC( pSrcBuf, (int)file_size );

			TCHAR * pCheckRoot = fullPath;
			pCheckRoot += _tcslen( pRoot );
			short nLength = (short)_tcslen( pCheckRoot );

			unsigned char uPackageIdx = MagiFileSystem::GetPackageIndex(pCheckRoot);
			nCount[uPackageIdx]++;
			
			//File Idx Info
			file_list.Write( &nLength             , sizeof(short)            );  //Dir
			file_list.Write( pCheckRoot           , sizeof(TCHAR)*nLength    );  //Dir Length
			file_list.Write( &uPackageIdx         , sizeof(unsigned char)    );  //Package Index
			file_list.Write( &file_size           , sizeof(size_t)           );  //File Size
			file_list.Write( &dwCrc32             , sizeof(unsigned long)    );  //CRC Data
			file_list.Write( &uOffSet[uPackageIdx], sizeof(unsigned __int64) );  //OffSet

			uOffSet[uPackageIdx] += file_size; //OffSet

			package_file[uPackageIdx].Write( pSrcBuf, sizeof(unsigned char)*file_size );
		}

		for( int i(0); c_nPackageCount>i; i++ )
		{
			LOG_UTIL::_out_console( _T("PACKAGE IDX %d: %d\n"), i, nCount[i] );
		}

		size_t flen = (size_t)file_list.GetLength();
		unsigned long dwDestLen = (unsigned long)(flen + Magi_compressBufSize( (unsigned long)flen ));

		pSrcBuf = new unsigned char[flen];
		pDstBuf = new unsigned char[dwDestLen];

		_AUTO_ARRAY_<unsigned char> autoSrc( pSrcBuf );
		_AUTO_ARRAY_<unsigned char> autoDst( pDstBuf );

		memset( pSrcBuf, 0, sizeof(unsigned char)*flen );
		memset( pDstBuf, 0, sizeof(unsigned char)*dwDestLen );
		
		file_list.Read( pSrcBuf, flen );

		//Make Packing Info File 
		//화일 리스트 압축 
		Magi_compress( pDstBuf, &dwDestLen, pSrcBuf, sizeof(unsigned char)*(unsigned long)flen );
		//화일 리스트 XOR
		for( unsigned int i(0); dwDestLen>i; i++ ) pDstBuf[i] ^= SimpleXORCodec::GetEncodeKeyChar( i );
		xor_file_list.Write( pDstBuf, sizeof(unsigned char)*dwDestLen );

		DWORD end_time = getMilliSecTime() - start_time;

		pAnalyse->OnPackingComplete();

		LOG_UTIL::_out_console( _T("PACKING TIME : %d Second\n"), end_time/1000 );

		return end_time;

	}
	catch(std::bad_alloc &ba)
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );

		return 0;
	}

	//
}

void funcCompress( FolderAnalyse::LINFO_FILE& info )
{  
	TCHAR SrcfullPath[_MAX_PATH] = {0,};
	TCHAR DstfullPath[_MAX_PATH] = {0,};

	FILE* pSrcF = NULL;
	FILE* pDstF = NULL;

	//원본
	_tcscpy_s( SrcfullPath, _MAX_PATH, info.strDirName.c_str()  );
	_tcscat_s( SrcfullPath, _MAX_PATH, info.strFileName.c_str() );

	//압축본
	_tcscpy_s( DstfullPath, _MAX_PATH, SrcfullPath );
	_tcscat_s( DstfullPath, _MAX_PATH, _TEXT(".mip") );

	_tfopen_s( &pSrcF, SrcfullPath, TEXT("rb") );
	_tfopen_s( &pDstF, DstfullPath, TEXT("wb") );

	unsigned char* pSrcBuf = NULL;
	unsigned char* pDstBuf = NULL;

	unsigned long dwDestLen = 0;
	int nRetCompress = 0;

	if( !pSrcF ) {
		_MBASSERT(0);
		return;
	}
	if( !pDstF ) {
		_MBASSERT(0);
		return;
	}

	try
	{
		// 데이터 파일 크기 세팅
		unsigned long fsize = 0;
		int nErr = fseek( pSrcF, 0, SEEK_END );
		if( nErr == 0  )
		{
			fsize = ftell( pSrcF );
			if( fsize == 0 ) { 
				throw "File Size 0";
			//	_MBASSERT(0 && "Size 0 인 파일"); //Size 0 인 파일
			}

			nErr = fseek( pSrcF, 0, SEEK_SET );
			assert( fsize != 0 );
		}

		info.uOriginSize = fsize;

		unsigned long dwTemp = Magi_compressBufSize( fsize );

		dwDestLen = fsize+dwTemp; 

		pSrcBuf = new unsigned char[fsize];
		pDstBuf = new unsigned char[dwDestLen];

		_AUTO_ARRAY_<unsigned char> autoSrc( pSrcBuf );
		_AUTO_ARRAY_<unsigned char> autoDst( pDstBuf );

		memset( pSrcBuf, 0, sizeof(unsigned char)*fsize );
		memset( pDstBuf, 0, sizeof(unsigned char)*dwDestLen );

		//원본
		fread( pSrcBuf, sizeof(unsigned char)*fsize, 1, pSrcF );

		//압축
		nRetCompress = Magi_compress( pDstBuf, &dwDestLen, pSrcBuf, sizeof(unsigned char)*fsize );

		if( nRetCompress == 0/*Z_OK*/ )
		{
			DWORD dwCrc32 = 0xFFFFFFFF;

			//XOR, CRC 코드 생성
			for( unsigned int i(0); dwDestLen>i; i++ ) 
			{
				pDstBuf[i] ^= SimpleXORCodec::GetEncodeKeyChar( i );
				dwCrc32 = (dwCrc32 >> 8) ^ SimpleXORCodec::GetCrc32TableIndex( (dwCrc32 & 0xFF) ^ pDstBuf[i] );
			}

			info.uCrc32 = dwCrc32;

			LOG_UTIL::_out_console( TEXT("%s%s\t%d\t%x %x\n"), info.strDirName.c_str(), info.strFileName.c_str(), dwDestLen, dwCrc32 );

			//압축본 생성
			fwrite( pDstBuf, sizeof(unsigned char)*dwDestLen, 1, pDstF );

			info.uCompressSize = dwDestLen;
		}
		else
		{
			assert( 0 && "압축 실패" );
		}

		if( pDstF ) fclose( pDstF );
		if( pSrcF ) fclose( pSrcF );
	}
	catch ( char *str ) {
		LOG_UTIL::_out_consoleA( "%s\n", str );
	}
	catch (std::bad_alloc &ba) {
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );

		if( pDstF ) fclose( pDstF );
		if( pSrcF ) fclose( pSrcF );
	}
}

void funcUnCompress( FolderAnalyse::LINFO_FILE& info )
{  
	TCHAR SrcfullPath[_MAX_PATH] = {0,};
	TCHAR DstfullPath[_MAX_PATH] = {0,};

	FILE* pSrcF = NULL;
	FILE* pDstF = NULL;

	//압축본
	_tcscpy_s( SrcfullPath, _MAX_PATH, info.strDirName.c_str()  );
	_tcscat_s( SrcfullPath, _MAX_PATH, info.strFileName.c_str() );

	//압축 화일 아님
	if( _tcsicmp( &SrcfullPath[_tcsnlen( SrcfullPath, _MAX_PATH ) - 4], TEXT(".mip") ) )
		return;

	//원본
	_tcscpy_s( DstfullPath, _MAX_PATH, SrcfullPath );
	DstfullPath[_tcsnlen( DstfullPath, _MAX_PATH ) - 4] = '\0'; 

	_tfopen_s( &pSrcF, SrcfullPath, TEXT("rb") );
	_tfopen_s( &pDstF, DstfullPath, TEXT("wb") );

	unsigned char* pSrcBuf = NULL;
	unsigned char* pDstBuf = NULL;

	unsigned long dwDestLen = 0;
	int nRetUnCompress = 0;

	if( !pSrcF ) {
		_MBASSERT(0);
		return;
	}
	if( !pDstF ) {
		_MBASSERT(0);
		return;
	}

	try
	{
		// 데이터 파일 크기 세팅
		unsigned long fsize = 0;
		int nErr = fseek( pSrcF, 0, SEEK_END );
		if( nErr == 0 )
		{
			fsize = ftell( pSrcF );
			nErr = fseek( pSrcF, 0, SEEK_SET );
			assert( fsize != 0 );
		}

		if( fsize < c_nUnitMegaSize )
			dwDestLen = fsize*15;  //15배
		else
			dwDestLen = fsize*5;   //5배

		pSrcBuf = new unsigned char[fsize];
		_AUTO_ARRAY_<unsigned char> autoSrc( pSrcBuf );
		
		memset( pSrcBuf, 0, sizeof(unsigned char)*fsize );

		//원본
		fread( pSrcBuf, sizeof(unsigned char)*fsize, 1, pSrcF );

		//XOR
		for( unsigned int i(0); fsize>i; i++ ) pSrcBuf[i] ^= SimpleXORCodec::GetEncodeKeyChar( i );

		while( true )
		{
			pDstBuf = new unsigned char[dwDestLen];
			_AUTO_ARRAY_<unsigned char> autoDst( pDstBuf );

			memset( pDstBuf, 0, sizeof(unsigned char)*dwDestLen );
			//압축
			nRetUnCompress = Magi_uncompress( pDstBuf, &dwDestLen, pSrcBuf, sizeof(unsigned char)*fsize );

			if( nRetUnCompress == 0/*Z_OK*/ )
			{
				LOG_UTIL::_out_console( TEXT("INFO COMPRESS RATE : %d\n"), dwDestLen/fsize );

				//압축본 생성
				fwrite( pDstBuf, sizeof(unsigned char)*dwDestLen, 1, pDstF );
				break;
			}
			else if( nRetUnCompress != -5/*Z_BUF_ERROR*/ )
			{
				assert( 0 && "압축 실패" );
				break;
			}

			dwDestLen += fsize;  
		}

		if( pDstF ) fclose( pDstF );
		if( pSrcF ) fclose( pSrcF );
	}
	catch(std::bad_alloc &ba)
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );

		if( pDstF ) fclose( pDstF );
		if( pSrcF ) fclose( pSrcF );
	}
}


unsigned __stdcall compress_folder( void* pArg )
{  
	DWORD start_time = getMilliSecTime();

	FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);

	std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();

	std::for_each( filelist.begin(), filelist.end(), funcCompress );

	DWORD end_time = getMilliSecTime() - start_time;

	pAnalyse->OnCompressingComplete();

	LOG_UTIL::_out_console( TEXT("COMPRESS TIME : %d\n"), end_time );

	return end_time;
}

unsigned __stdcall uncompress_folder( void* pArg )
{  
	DWORD start_time = getMilliSecTime();

	FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);

	std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();

	std::for_each( filelist.begin(), filelist.end(), funcUnCompress );

	DWORD end_time = getMilliSecTime() - start_time;

	pAnalyse->OnUnCompressingComplete();

	LOG_UTIL::_out_console( TEXT("UNCOMPRESS TIME : %d\n"), end_time );

	return end_time;
}

unsigned __stdcall move_folder( void* pArg )
{  
	unsigned char* pSrcBuf = NULL;

	try
	{

		DWORD start_time = getMilliSecTime();

		FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);

		const TCHAR* pRoot = pAnalyse->GetRoot(); //Root
		const TCHAR* pOut  = pAnalyse->GetOut();  //Out

		std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();
		
		TCHAR szDir[_MAX_PATH] = {0,};
		TCHAR szFile[_MAX_PATH] = {0,};

		TCHAR szOut[_MAX_PATH] = {0,};
		TCHAR szOri[_MAX_PATH] = {0,};

		//File List 저장용
		TCHAR text_fullPath[_MAX_PATH] = {0,};
//		char  strTmp[1024] = {0,};
//		char  strFullPath[_MAX_PATH] = {0,};
		_tcscpy_s( text_fullPath, _MAX_PATH, pOut );
		_tcscat_s( text_fullPath, _MAX_PATH, TEXT(c_ResIdxTextFile) );
		//MagiFileStream file_list_text( text_fullPath, TEXT("wt+") ); 

		TCHAR* pCurDir = szDir;

		for( ; it != filelist.end(); ++it  )
		{
			_tcscpy_s( szDir , _MAX_PATH, (*it).strDirName.c_str()  );
			_tcscpy_s( szFile, _MAX_PATH, (*it).strFileName.c_str() );

			_tcscpy_s( szOri, _MAX_PATH, (*it).strDirName.c_str()  );
			_tcscat_s( szOri, _MAX_PATH, (*it).strFileName.c_str() );

			TCHAR* pSubDir = pCurDir + _tcslen( pRoot );

			_tcscpy_s( szOut, _MAX_PATH, pOut );
			_tcscat_s( szOut, _MAX_PATH, pSubDir );
		
			MagiFileStream it_file( szOri, TEXT("rb") );
			size_t file_size = (size_t)it_file.GetLength();
			pSrcBuf = new unsigned char[file_size];
			_AUTO_ARRAY_<unsigned char> autoSrc( pSrcBuf );

			it_file.Read( pSrcBuf, sizeof(unsigned char)*file_size );
			it_file.Close();

			_tcscpy_s( text_fullPath, _MAX_PATH, pSubDir );
			_tcscat_s( text_fullPath, _MAX_PATH, (*it).strFileName.c_str() );

			unsigned long dwCrc32 = SimpleXORCodec::GetCRC( pSrcBuf, (unsigned long)file_size );

			//File List
//			WideCharToMultiByte( CP_ACP, 0, text_fullPath, -1, strFullPath, _MAX_PATH, NULL, NULL );
//			StringFormat( strTmp, "%s\t%d\t%x\n", strFullPath, file_size, dwCrc32 );
//			file_list_text.Write( strTmp, strlen(strTmp)*sizeof(char) );

			FolderAnalyse::CreateSubDirectory( szOut );

			_tcscat_s( szOut, _MAX_PATH, szFile );

			//Move File
			if( !MoveFile( szOri, szOut ) )
			{
				DWORD dwErr = GetLastError();
				assert( 0 && "MoveFile Failed" );
			}
		}

		DWORD end_time = getMilliSecTime() - start_time;

		pAnalyse->OnMoveComplete();

		LOG_UTIL::_out_console( TEXT("MOVE TIME : %d\n"), end_time );

		return end_time;
	}
	catch( std::bad_alloc& ba )
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );
		
		return 0;
	}
}

unsigned __stdcall make_lower( void* pArg )
{
	 
	unsigned char* pSrcBuf = NULL;

	try
	{
		DWORD start_time = getMilliSecTime();

		FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);

		const TCHAR* pRoot = pAnalyse->GetRoot(); //Root
		const TCHAR* pOut  = pAnalyse->GetOut();  //Out

	//dir
	{
#ifdef UNICODE
		std::vector< std::wstring >& dirlist = pAnalyse->GetDirList();
		std::vector< std::wstring >::iterator it = dirlist.begin();
#else
		std::vector< std::string >& dirlist = pAnalyse->GetDirList();
		std::vector< std::string >::iterator it = dirlist.begin();
#endif

		LOG_UTIL::_out_console( TEXT("DIR LOWER : %d\n"), dirlist.size() );

		for( ; it != dirlist.end(); ++it  )
		{

#ifdef UNICODE
			std::wstring strLower = (*it).c_str();
#else
			std::string strLower = (*it).c_str();
#endif
			//파일 이름 변경
			MBStringUtil::ToLower( strLower );

#ifdef UNICODE
			_wrename( (*it).c_str(), strLower.c_str() );
#else
			rename( (*it).c_str(), strLower.c_str() );
#endif
		}
	}

	//file
		std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();

		TCHAR szDir[_MAX_PATH] = {0,};

		TCHAR szOut[_MAX_PATH] = {0,};
		TCHAR szOri[_MAX_PATH] = {0,};

		TCHAR* pCurDir = szDir;

		LOG_UTIL::_out_console( TEXT("FILE LOWER : %d\n"), filelist.size() );

		for( ; it != filelist.end(); ++it  )
		{
			_tcscpy_s( szOri, _MAX_PATH, (*it).strDirName.c_str()  );
			_tcscat_s( szOri, _MAX_PATH, (*it).strFileName.c_str() );

			TCHAR* pSubDir = pCurDir + _tcslen( pRoot );

			_tcscpy_s( szOut, _MAX_PATH, pOut );
			_tcscat_s( szOut, _MAX_PATH, pSubDir );

#ifdef UNICODE
			std::wstring strLower = szOri;
#else
			std::string  strLower = szOri;
#endif
			//파일 이름 변경
			MBStringUtil::ToLower( strLower );

#ifdef UNICODE
			_wrename( szOri, strLower.c_str() );
#else
			rename( szOri, strLower.c_str() );
#endif
		}

		DWORD end_time = getMilliSecTime() - start_time;

		pAnalyse->OnMoveComplete();

		LOG_UTIL::_out_console( TEXT("MOVE TIME : %d\n"), end_time );

		return end_time;
	}
	catch( std::bad_alloc& ba )
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );

		return 0;
	}
}

unsigned __stdcall make_upper( void* pArg )
{
	 
	unsigned char* pSrcBuf = NULL;

	try
	{
		DWORD start_time = getMilliSecTime();

		FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);

		const TCHAR* pRoot = pAnalyse->GetRoot(); //Root
		const TCHAR* pOut  = pAnalyse->GetOut();  //Out
	//dir
	{
#ifdef UNICODE
		std::vector< std::wstring >& dirlist = pAnalyse->GetDirList();
		std::vector< std::wstring >::iterator it = dirlist.begin();
#else
		std::vector< std::string >& dirlist = pAnalyse->GetDirList();
		std::vector< std::string >::iterator it = dirlist.begin();
#endif

		LOG_UTIL::_out_console( TEXT("DIR UPPER : %d\n"), dirlist.size() );

		for( ; it != dirlist.end(); ++it  )
		{

#ifdef UNICODE
			std::wstring strUpper = (*it).c_str();
#else
			std::string strUpper = (*it).c_str();
#endif
			//파일 이름 변경
			MBStringUtil::ToUpper( strUpper );

#ifdef UNICODE
			_wrename( (*it).c_str(), strUpper.c_str() );
#else
			rename( (*it).c_str(), strUpper.c_str() );
#endif
		}
	}

	//file
		std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();

		TCHAR szDir[_MAX_PATH] = {0,};

		TCHAR szOut[_MAX_PATH] = {0,};
		TCHAR szOri[_MAX_PATH] = {0,};

		TCHAR* pCurDir = szDir;

		LOG_UTIL::_out_console( TEXT("FILE UPPER : %d\n"), filelist.size() );

		for( ; it != filelist.end(); ++it  )
		{
			_tcscpy_s( szOri, _MAX_PATH, (*it).strDirName.c_str()  );
			_tcscat_s( szOri, _MAX_PATH, (*it).strFileName.c_str() );

			TCHAR* pSubDir = pCurDir + _tcslen( pRoot );

			_tcscpy_s( szOut, _MAX_PATH, pOut );
			_tcscat_s( szOut, _MAX_PATH, pSubDir );

#ifdef UNICODE
			std::wstring strUpper = szOri;
#else
			std::string  strUpper = szOri;
#endif
			//파일 이름 변경
			MBStringUtil::ToUpper( strUpper );

#ifdef UNICODE
			_wrename( szOri, strUpper.c_str() );
#else
			rename( szOri, strUpper.c_str() );
#endif
		}

		DWORD end_time = getMilliSecTime() - start_time;

		pAnalyse->OnMoveComplete();

		LOG_UTIL::_out_console( TEXT("MOVE TIME : %d\n"), end_time );

		return end_time;
	}
	catch( std::bad_alloc& ba )
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );

		return 0;
	}
}

unsigned __stdcall search_0byte( void* pArg )
{  
	unsigned char* pSrcBuf = NULL;

	try
	{
		DWORD start_time = getMilliSecTime();

		FolderAnalyse* pAnalyse = ((FolderAnalyse*)pArg);

		const TCHAR* pRoot = pAnalyse->GetRoot(); //Root
		const TCHAR* pOut  = pAnalyse->GetOut();  //Out

		std::vector< FolderAnalyse::LINFO_FILE >& filelist = pAnalyse->GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();

		TCHAR szDir[_MAX_PATH] = {0,};
		TCHAR szFile[_MAX_PATH] = {0,};

		TCHAR szOut[_MAX_PATH] = {0,};
		TCHAR szOri[_MAX_PATH] = {0,};

		//File List 저장용
		TCHAR text_fullPath[_MAX_PATH] = {0,};
//		char  strTmp[1024] = {0,};
		char  strFullPath[_MAX_PATH] = {0,};
		_tcscpy_s( text_fullPath, _MAX_PATH, pOut );
		_tcscat_s( text_fullPath, _MAX_PATH, TEXT(c_ResIdxTextFile) );
		//MagiFileStream file_list_text( text_fullPath, TEXT("wt+") ); 

		TCHAR* pCurDir = szDir;

		_stprintf_s( text_fullPath, _MAX_PATH, TEXT("%s0byte.bat"), pOut );
		MagiFileStream Byte0_file( text_fullPath, TEXT("wt") );

		for( ; it != filelist.end(); ++it  )
		{
			_tcscpy_s( szDir , _MAX_PATH, (*it).strDirName.c_str()  );
			_tcscpy_s( szFile, _MAX_PATH, (*it).strFileName.c_str() );

			_tcscpy_s( szOri, _MAX_PATH, (*it).strDirName.c_str()  );
			_tcscat_s( szOri, _MAX_PATH, (*it).strFileName.c_str() );

			TCHAR* pSubDir = pCurDir + _tcslen( pRoot );

			_tcscpy_s( szOut, _MAX_PATH, pOut );
			_tcscat_s( szOut, _MAX_PATH, pSubDir );

			MagiFileStream it_file( szOri, TEXT("rb") );
			size_t file_size = (size_t)it_file.GetLength();

			//0Byte 파일들
			if( file_size <= 0 ) {

				memset( text_fullPath, 0, sizeof(TCHAR)*_MAX_PATH );

				_stprintf_s( text_fullPath, _MAX_PATH, TEXT("DEL \"%s%s\"\n"), (*it).strDirName.c_str(), (*it).strFileName.c_str() );

				//File List
#ifdef UNICODE
				WideCharToMultiByte( CP_ACP, 0, text_fullPath, -1, strFullPath, _MAX_PATH, NULL, NULL );
#endif
				Byte0_file.Write( strFullPath, sizeof(char)*strlen(strFullPath) );
			}
		}

		DWORD end_time = getMilliSecTime() - start_time;

		pAnalyse->OnMoveComplete();

		LOG_UTIL::_out_console( TEXT("MOVE TIME : %d\n"), end_time );

		return end_time;
	}
	catch( std::bad_alloc& ba )
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );

		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

FolderAnalyse::FolderAnalyse()
{  
	m_dwError = ERROR_SUCCESS;
	m_pIF     = NULL;
	setState( ST_NONE );
	SetNextState( ST_NONE );
	SetFilter( false );
}

FolderAnalyse::FolderAnalyse( const TCHAR* pszRoot )
{  
	m_strRoot = pszRoot;
	m_dwError = ERROR_SUCCESS;
	m_pIF     = NULL;
	setState( ST_NONE );
	SetNextState( ST_NONE );
	SetFilter( false );
}

FolderAnalyse::~FolderAnalyse()
{  

}

void FolderAnalyse::SetRoot( const TCHAR* pszRoot )
{  
	m_strRoot = pszRoot;
}

void FolderAnalyse::SetOut( const TCHAR* pszOut )
{  
	m_strOut = pszOut;
}

void FolderAnalyse::AddDir( const TCHAR* dir )
{
#ifdef UNICODE
	std::vector< std::wstring >::iterator it = std::find( m_vDirList.begin(), m_vDirList.end(), dir );
#else
	std::vector< std::string >::iterator it = std::find( m_vDirList.begin(), m_vDirList.end(), dir );
#endif
	if( it == m_vDirList.end() ) {
		m_vDirList.push_back( dir );
	}
}

void FolderAnalyse::ClearDirList()
{
	m_vDirList.clear();
}

void FolderAnalyse::AddFile( const TCHAR* dir, const TCHAR* file )
{  
	LINFO_FILE linfo_file;
	linfo_file.strDirName  = dir;
	linfo_file.strFileName = file;

	m_vFileList.push_back( linfo_file );
}

void FolderAnalyse::ClearFileList()
{  
	m_vFileList.clear();
}

void FolderAnalyse::DoAnalyse( bool bWaitThread /*= false*/ )
{  
	if( GetState() != ST_NONE ) return;

	setState( ST_ANALYZING );

	ClearFileList(); //이전 데이타 삭제

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, analyse_folder, this, 0, &tmp ) );

	if( bWaitThread )
	{
		//위 쓰레드 종료 될때까지 대기
		int rv = WaitForSingleObject(hThread, INFINITE); 
	}

	CloseHandle( hThread );
}

void FolderAnalyse::DoPacking()
{  
	if( m_vFileList.empty() ) 
	{
		setState(ST_NONE);
		return;
	}

	setState( ST_PACKING );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, packing_folder, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::DoCompress()
{  
	if( GetState() != ST_ANALYSED ) return;

	setState( ST_COMPRESSING );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, compress_folder, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::DoUnCompress()
{  
	if( GetState() != ST_ANALYSED ) return;

	setState( ST_UNCOMPRESSING );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, uncompress_folder, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::DoMove()
{  
	if( GetState() != ST_ANALYSED ) return;

	setState( ST_MOVING );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, move_folder, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::Do0Byte()
{  
	if( GetState() != ST_ANALYSED ) return;

	setState( ST_0BYTE );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, search_0byte, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::DoMakeLower()
{ 
	 
	if( GetState() != ST_ANALYSED ) return;

	setState( ST_MAKE_LOWER );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, make_lower, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::DoMakeUpper()
{ 
	 
	if( GetState() != ST_ANALYSED ) return;

	setState( ST_MAKE_UPPER );

	unsigned tmp = 0;
	HANDLE hThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, make_upper, this, 0, &tmp ) );
	CloseHandle( hThread );
}

void FolderAnalyse::OnAnalyseComplete()
{  
	setState( ST_ANALYSED );
}

void FolderAnalyse::OnPackingComplete()
{  
	setState( ST_PACKED );
}

void FolderAnalyse::OnCompressingComplete()
{  
	setState( ST_COMPRESSED	);
}

void FolderAnalyse::OnUnCompressingComplete()
{  
	setState( ST_UNCOMPRESSED	);
}

void FolderAnalyse::OnMoveComplete()
{  
	setState( ST_MOVEED	);
}

void FolderAnalyse::Process( DWORD time )
{  
	switch( GetState() )
	{
	case ST_NONE : 		{	Sleep(10);	}	break;
	case ST_ANALYSED :
		{
			SetFilter( false );

			switch( GetNextState() )
			{
			case ST_PACKING		  : DoPacking();       break;
			case ST_UNCOMPRESSING : DoUnCompress();    break;

			case ST_COMPRESSING	  : DoCompress();      break;
			case ST_MOVING        : DoMove();		   break;
			case ST_0BYTE         : Do0Byte();		   break;	
			case ST_MAKE_LOWER	  : DoMakeLower();	   break;	
			case ST_MAKE_UPPER    : DoMakeUpper();	   break;	
				
			case ST_NONE          : setState(ST_NONE); break;
			}
			
			SetNextState( ST_NONE );
		}
		break;
	case ST_PACKED       :	{	setState(ST_NONE);	}	break;
	case ST_COMPRESSED   :	{	setState(ST_NONE);	}	break;
	case ST_UNCOMPRESSED :	{	setState(ST_NONE);	}	break;
	case ST_MOVEED       :	{	setState(ST_NONE);	}	break;	
	}
}



//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//

CheckPatch::~CheckPatch()
{  
	//cli List
	{
		boost::unordered_map< std::string, PATCHINFO* >::iterator it = m_client_filelist.begin();
		for(; it != m_client_filelist.end(); ++it )
		{
			delete it->second;
		}
		m_client_filelist.clear();
	}

	//ser List
	{
		boost::unordered_map< std::string, PATCHINFO* >::iterator it = m_server_filelist.begin();
		for(; it != m_server_filelist.end(); ++it )
		{
			delete it->second;
		}
		m_server_filelist.clear();
	}
}

void SplitBuf( std::vector<std::string>& strlist, const char* pBuf, const char* pSplitChar )
{  
	unsigned len = (unsigned)strlen(pBuf)+1;
	int nSplitCharCnt = (int)strlen( pSplitChar );

	std::string strTmp;

	int i = 0;
	for( unsigned j(0); len>j; ++j )
	{
		for( i=0; nSplitCharCnt>i; ++i )
		{
			if( pBuf[j] == pSplitChar[i] )
			{
				if( strTmp.size() ) strlist.push_back( strTmp );
				strTmp.erase( strTmp.begin(), strTmp.end() );
				break;
			}
		}
		
		if( i == nSplitCharCnt )
			strTmp += pBuf[j];
	}
}

void CheckPatch::makeData( const TCHAR* pFileSrc, std::vector<std::string>& datalist )
{  
	char* pBuf = NULL;

	try
	{
		MagiFileStream srcFile( pFileSrc, TEXT("rb+") );

		if( !srcFile.IsValid() )
		{
			assert(0 && "Invaild File");
			return;
		}

		size_t file_size = (size_t)srcFile.GetLength();
		pBuf = new char[file_size];
		_AUTO_ARRAY_<char> autoBuf( pBuf );

		size_t read_size = srcFile.Read( pBuf, file_size );

		SplitBuf( datalist, pBuf, "\t\n" );
	}
	catch (std::bad_alloc ba)
	{

	}
}

void CheckPatch::parseData( boost::unordered_map< std::string, PATCHINFO* >& refhash, std::vector<std::string>& datalist )
{  
	int loopCnt = (int)datalist.size()/3;

	boost::unordered_map< unsigned long, PATCHINFO* > duplicateCheck;

	int nCnt = 0;
	char *HEX_P;
	for( int i(0); loopCnt>i; ++i )
	{
		PATCHINFO* pInfo = new PATCHINFO;

		pInfo->strName   = datalist[nCnt++].c_str();                      //화일 이름
		std::transform(pInfo->strName.begin() , pInfo->strName.end() , pInfo->strName.begin() , (int(*)(int)) std::tolower);

		pInfo->nFileSize = atoi( datalist[nCnt++].c_str() );              //File Size
		pInfo->dwCrc32   = strtoul(datalist[nCnt++].c_str(), &HEX_P, 16); //CRC32

		boost::unordered_map< unsigned long, PATCHINFO* >::const_iterator dit = duplicateCheck.find( pInfo->dwCrc32 );
		if( dit == duplicateCheck.end() )
		{
			boost::unordered_map< std::string, PATCHINFO* >::const_iterator it = refhash.find( pInfo->strName );
			if( it == refhash.end() )
			{
				refhash.insert( std::pair< std::string, PATCHINFO* >( std::string(pInfo->strName), pInfo ) );
			}
			else
			{
				//이미 존재 하는 파일
				_debug_printA( "Duplicate File : %s\n", pInfo->strName.c_str() );
				assert( 0 && "Parse Error or Critical Problem" );
			}
		}
		else
		{
			PATCHINFO* pDInfo = dit->second;

			//이미 존재 하는 파일
			LOG_UTIL::_out_consoleA( "Duplicate File - In different place location : %s\t\t%s\n", pDInfo->strName.c_str(), pInfo->strName.c_str() );
			delete pInfo;
		}
	}

	duplicateCheck.clear();
}

void CheckPatch::DoPatch( const TCHAR* pFileSrc, const TCHAR* pFileDst )
{  
	std::vector<std::string> cli_checkList;
	std::vector<std::string> ser_checkList;

	makeData( pFileSrc, cli_checkList );
	makeData( pFileDst, ser_checkList );

	parseData( m_client_filelist, cli_checkList );
	parseData( m_server_filelist, ser_checkList );

	boost::unordered_map< std::string, PATCHINFO* >::iterator it = m_server_filelist.begin();
	for(; it != m_server_filelist.end(); ++it )
	{
		PATCHINFO* pInfo = it->second;

		boost::unordered_map< std::string, PATCHINFO* >::iterator find = m_client_filelist.find( pInfo->strName );
		if( find == m_client_filelist.end() )
		{	//기존에 없는 파일
			m_down_list.push_back( pInfo );
		}
		else
		{
			PATCHINFO* pCliInfo = find->second;

			if( pCliInfo->dwCrc32 != pInfo->dwCrc32 )
			{	//변경된 파일
				m_down_list.push_back( pInfo );
			}
		}
	}

	int nDownCnt = (int)m_down_list.size();
}



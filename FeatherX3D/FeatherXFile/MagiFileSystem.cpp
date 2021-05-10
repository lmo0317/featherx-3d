
#include "MagiFileDef.h"
#include <Windows.h>
#include "MagiFileSystem.h"

#include "MagiResDef.h"
#include "SimpleCodec.h"
#include "MagiStream.h"
#include "MagiZlib.h"

#include <tchar.h>
#include <assert.h>
#include <algorithm>
#include <cctype>       // std::tolower

#include "MBfasttime.h"
#include "MBCLog.h"

using namespace LOG_UTIL;
//#include "MBDebugUtil.h"

extern void _debug_printA( const char *str, ... );
extern void _debug_print( const TCHAR *str, ... );

unsigned int MagiFileSystem::m_nPackageCount = c_nPackageCount;
MagiFileSystem* MagiFileSystem::s_pFileSystem[_res_max];

bool MagiFileSystem::s_bUsePackedFile = false;

MagiFileSystem::MagiFileSystem( const TCHAR * pRoot ) : m_bReadOnly(TRUE)
{ 
	m_strRoot = pRoot;
}

void MagiFileSystem::OnNotify( short nMsg )
{ 
	//Nothing...
}

#ifdef UNICODE
void MagiFileSystem::removeRootDir( std::wstring& strFullPath )
#else
void MagiFileSystem::removeRootDir( std::string& strFullPath )
#endif
{
	if( !_tcsnicmp( m_strRoot.c_str(), strFullPath.c_str(), m_strRoot.length() ) )
	{
		strFullPath.erase( 0, strFullPath.length() );
		std::transform(strFullPath.begin() , strFullPath.end() , strFullPath.begin() , (int(*)(int)) std::tolower); //소문자 변경
	}
}

//////////////////////////////////////////////////////////////////////////
//기본 폴더 구조

void MagiBasicFileSystem::Init( const TCHAR * pRoot, bool bInitSkip, const int nIndex )
{ 
	MagiBasicFileSystem* pBasicFileSystem = new MagiBasicFileSystem(pRoot, bInitSkip);
	s_pFileSystem[nIndex] = pBasicFileSystem;
}

MagiBasicFileSystem::MagiBasicFileSystem( const TCHAR * pRoot, bool bInitSkip ) : MagiFileSystem(pRoot) 
{ 
	if( !bInitSkip )
		init();
}

MagiBasicFileSystem::~MagiBasicFileSystem()
{ 
	//cache
	{
#ifdef UNICODE
		boost::unordered_map< std::wstring, MagiStream* >::iterator it = m_file_cache_hash.begin();
#else
		boost::unordered_map< std::string, MagiStream* >::iterator it = m_file_cache_hash.begin();
#endif
		for(; it != m_file_cache_hash.end(); ++it )
		{
			delete it->second;
		}
		m_file_cache_hash.clear();
	}

	m_all_file_hash.clear();
}

MagiStream* MagiBasicFileSystem::findCache( const TCHAR* pFullpath )
{
#ifdef UNICODE
	boost::unordered_map< std::wstring, MagiStream* >::iterator it = m_file_cache_hash.find( pFullpath );
#else
	boost::unordered_map< std::string, MagiStream* >::iterator it = m_file_cache_hash.find( pFullpath );
#endif
	if( it == m_file_cache_hash.end() )
		return NULL;
	
	return it->second;
}
#ifdef UNICODE
int MagiBasicFileSystem::GetDirFileListbyExt( const TCHAR* pDir, const TCHAR* pExt, std::vector<std::wstring>& filelist )
#else
int MagiBasicFileSystem::GetDirFileListbyExt( const TCHAR* pDir, const TCHAR* pExt, std::vector<std::string>& filelist )
#endif
{
	FolderAnalyse folderanalyse;
	folderanalyse.SetRoot( pDir );
	folderanalyse.DoAnalyse( true );

	std::vector<FolderAnalyse::LINFO_FILE>& all_list = folderanalyse.GetFileList();
#ifdef UNICODE
	std::wstring strHalfName; //Root 제외
	std::wstring strFullName;
#else
	std::string strHalfName; //Root 제외
	std::string strFullName;
#endif

	const int nExtSize = _tcslen( pExt );

	for( unsigned int i(0); all_list.size()>i; ++i )
	{
#ifdef UNICODE
		if( !_wcsicmp( pExt, all_list[i].strFileName.c_str()+(all_list[i].strFileName.length()-nExtSize) ) ) {
#else
		if( !_stricmp( pExt, all_list[i].strFileName.c_str()+(all_list[i].strFileName.length()-nExtSize) ) ) {
#endif
			strHalfName =  all_list[i].strDirName;
			strHalfName += all_list[i].strFileName;

			filelist.push_back( strHalfName );
		}
	}

	return filelist.size();
}

#ifdef UNICODE
int MagiBasicFileSystem::GetDirFileList( const TCHAR* pDir, std::vector<std::wstring>& filelist )
#else
int MagiBasicFileSystem::GetDirFileList( const TCHAR* pDir, std::vector<std::string>& filelist )
#endif
{
	FolderAnalyse folderanalyse;
	folderanalyse.SetRoot( pDir );
	folderanalyse.DoAnalyse( true );

	std::vector<FolderAnalyse::LINFO_FILE>& all_list = folderanalyse.GetFileList();
#ifdef UNICODE
	std::wstring strHalfName; //Root 제외
	std::wstring strFullName;
#else
	std::string strHalfName; //Root 제외
	std::string strFullName;
#endif

	for( unsigned int i(0); all_list.size()>i; ++i )
	{
		strHalfName =  all_list[i].strDirName;
		strHalfName += all_list[i].strFileName;

		filelist.push_back( strHalfName );
	}

	return filelist.size();
}

MagiStream* MagiBasicFileSystem::GetStream( const TCHAR* pFullpath )
{ 
	MagiStream* pCache = findCache( pFullpath );
	if( pCache ) 
		return pCache;
#ifdef UNICODE
	boost::unordered_map< std::wstring, std::wstring >::iterator it = m_all_file_hash.find( pFullpath );
#else
	boost::unordered_map< std::string, std::string >::iterator it = m_all_file_hash.find( pFullpath );
#endif
	if( it == m_all_file_hash.end() )
		return NULL; //없는 파일 요청

	MagiFileStream* pFileSteam = new MagiFileStream;
	pFileSteam->Open( it->second.c_str(), (m_bReadOnly ? TEXT("rb") : TEXT("wbc+")) );

	assert( pFileSteam->IsValid() );
#ifdef UNICODE
	m_file_cache_hash.insert( std::pair< std::wstring, MagiStream* >( std::wstring(pFullpath), pFileSteam ) );
#else
	m_file_cache_hash.insert( std::pair< std::string, MagiStream* >( std::string(pFullpath), pFileSteam ) );

#endif

	return pFileSteam;
}

void MagiBasicFileSystem::init()
{ 
	FolderAnalyse folderanalyse;
	folderanalyse.SetRoot( m_strRoot.c_str() );
	folderanalyse.DoAnalyse( true );

	std::vector<FolderAnalyse::LINFO_FILE>& all_list = folderanalyse.GetFileList();
#ifdef UNICODE
	std::wstring strHalfName; //Root 제외
	std::wstring strFullName;
#else
	std::string strHalfName; //Root 제외
	std::string strFullName;
#endif


	for( unsigned int i(0); all_list.size()>i; ++i )
	{
		strHalfName =  all_list[i].strDirName;
		strHalfName += all_list[i].strFileName;

		strHalfName.erase( 0, m_strRoot.size() ); //Root 삭제
		std::transform(strHalfName.begin() , strHalfName.end() , strHalfName.begin() , (int(*)(int)) std::tolower);

		strFullName = all_list[i].strDirName;
		strFullName += all_list[i].strFileName;
#ifdef UNICODE
		m_all_file_hash.insert( std::pair< std::wstring, std::wstring >( strHalfName, strFullName ) );
#else
		m_all_file_hash.insert( std::pair< std::string, std::string >( strHalfName, strFullName ) );
#endif
	}

}

//////////////////////////////////////////////////////////////////////////
//패키징 화일

void MagiPackageFileSystem::Init( const TCHAR * pRoot )
{ 
	MagiPackageFileSystem* pPackFileSystem = new MagiPackageFileSystem(pRoot, true);
	s_pFileSystem[_first] = pPackFileSystem;
	s_pFileSystem[_second] = NULL;
}

MagiPackageFileSystem::MagiPackageFileSystem( const TCHAR * pRoot, bool bReadOnly ) 
: m_bCompressMode(false), m_strDownDir( _T("archive\\") ), m_hWaitEvent(NULL), MagiFileSystem(pRoot), m_pGUI(NULL)
{ 
	m_bReadOnly = bReadOnly;
}

MagiPackageFileSystem::~MagiPackageFileSystem()
{ 
	clear();
}

void MagiPackageFileSystem::clear()
{ 
	{
		while( !m_qLoadList.empty() )
		{
			MagiStream* pData = m_qLoadList.front();
			delete pData;
			m_qLoadList.pop();
		}
	}

	{
		for each( auto pSteam in m_vLoadList )
		{
			delete pSteam;
		}
		m_vLoadList.clear();
	}


	//cache
	//{
	//	boost::unordered_map< std::wstring, MagiStream* >::iterator it = m_file_cache_hash.begin();
	//	for(; it != m_file_cache_hash.end(); ++it )
	//	{
	//		delete it->second;
	//	}
	//	m_file_cache_hash.clear();
	//}

	//Index List
	{
		for each( auto& p in m_idx_hash )
		{
			delete p.second;
		}
		m_idx_hash.clear();
	}

	//Package Handle
	{
		std::map< int, PackageFileHandle >::iterator it;
		for ( it = m_mapPackageFileHandle.begin(); it != m_mapPackageFileHandle.end(); ++it )
		{
			for ( int i = 0; i < MAX_HANDLE_COUNT; ++i )
			{
				if( (*it).second.handle[i] ) {
					if( !m_bReadOnly )
						fflush( (*it).second.handle[i] );
					fclose( (*it).second.handle[i] );
				}
			}
		}

		m_mapPackageFileHandle.clear();
	}

#ifdef _USE_MEMORYMAPPED_IO
	{
		std::map < std::wstring, HANDLE >::iterator it;
		for ( it = m_hMMList.begin(); it != m_hMMList.end(); ++it )
			CloseHandle((*it).second);

		m_hMMList.clear();
	}
#endif
}

MagiPackageFileSystem::PACK_FILEINFO* MagiPackageFileSystem::getPackFileInfo( const TCHAR* pFullpath )
{ 
#ifdef UNICODE
	auto it = m_idx_hash.find( std::wstring(pFullpath) );
#else
	auto it = m_idx_hash.find( std::string(pFullpath) );
#endif

	if( it == m_idx_hash.end() )
		return NULL;
	return it->second;
}

#ifdef _USE_MEMORYMAPPED_IO
HANDLE MagiPackageFileSystem::getMemoryMapedHandle( const TCHAR* pFullpath )
{
	if( m_bReadOnly == true )
	{
		size_t hash_key = GetPackageIndex( pFullpath );

		TCHAR full_pack[_MAX_PATH] = {0,};
		TCHAR name_pack[_MAX_PATH] = {0,};
		MagiFileSystem::GetPackageFileName( hash_key, name_pack, _MAX_PATH );

		_tcscpy_s( full_pack, _MAX_PATH, m_strRoot.c_str() ); //Root
		_tcscat_s( full_pack, _MAX_PATH, name_pack );         //Package Name

		std::map < std::wstring, HANDLE >::iterator it = m_hMMList.find( full_pack );
		if (it != m_hMMList.end())
			return (*it).second;
		HANDLE hFile = CreateFile( full_pack, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		HANDLE hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
		return m_hMMList[ full_pack ] = hMapFile;
	}
	return NULL;
}
#endif

FILE* MagiPackageFileSystem::getPackageFileHandle( const TCHAR* pFullpath )
{ 
	FILE *fpackage = NULL;

	int thread_id = GetCurrentThreadId();
//	MBCLog::GetSingleton()->DebugOutput(L"PackageFS : %d thread_id", thread_id );
	std::map< int, PackageFileHandle >::iterator it;

	while ( true )
	{
		it = m_mapPackageFileHandle.find( thread_id );
		if ( it != m_mapPackageFileHandle.end() ) break;
		m_mapPackageFileHandle[ thread_id ] = PackageFileHandle();
	}

	//Package Index
	size_t hash_key = GetPackageIndex( pFullpath );
	if ( (*it).second.handle[ hash_key ] != NULL ) return (*it).second.handle[ hash_key ];

	TCHAR full_pack[_MAX_PATH] = {0,};
	TCHAR name_pack[_MAX_PATH] = {0,};
	MagiFileSystem::GetPackageFileName( hash_key, name_pack, _MAX_PATH );

	_tcscpy_s( full_pack, _MAX_PATH, m_strRoot.c_str() ); //Root
	_tcscat_s( full_pack, _MAX_PATH, name_pack );         //Package Name
	
	errno_t err = _tfopen_s( &fpackage, full_pack, (m_bReadOnly ? _T("rb") : _T("wbc+")) );
	if( err != 0 )
	{
		MagiStream::ViewError( err );
		MB_ERROR( __FILE__, __LINE__ );
	}

	assert( fpackage );
	if ( !fpackage ) return NULL;

	(*it).second.handle[ hash_key ] = fpackage;

	return fpackage;
}

MagiStream* MagiPackageFileSystem::findCache( const TCHAR* pFullpath )
{ 
#ifdef UNICODE
	auto it = m_file_cache_hash.find( std::wstring(pFullpath) );
#else
	auto it = m_file_cache_hash.find( std::string(pFullpath) );
#endif

	if( it == m_file_cache_hash.end() )
		return NULL;

	it->second->Seek( 0, MagiStream::seek_set ); //초기 위치로 셋팅

	return it->second;
}

namespace
{
	const int c_nMaxLoadCount = 300;
}

MagiStream* MagiPackageFileSystem::GetWaveStream( const TCHAR* pFullpath )
{ 
	//int thread_id = GetCurrentThreadId();
	//LOGOUT( L"+++GetWaveStream : %d thread_id : %s", thread_id, pFullpath );

#ifdef _USE_MEMORYMAPPED_IO
	HANDLE hFile = getMemoryMapedHandle( pFullpath );

	if (hFile != NULL)
	{
		PACK_FILEINFO* pInfo = getPackFileInfo( pFullpath );
		if (pInfo != NULL)
		{
			MagiStream * pStream = MagiMemoryMappedStream::Create( hFile, pInfo->uOffSet, pInfo->file_size );
			if (pStream != NULL)
			{
				m_vLoadList.push_back( pStream );
				return pStream;
			}
		}
	}
#endif
	FILE* fpackage = getPackageFileHandle( pFullpath );
	if( fpackage )
	{
		PACK_FILEINFO* pInfo = getPackFileInfo( pFullpath );
		if( pInfo )
		{
			MagiPackedFileStream * pStream = new MagiPackedFileStream( fpackage, pFullpath, pInfo->uOffSet, pInfo->file_size );

//			m_file_cache_hash.insert( std::pair< std::wstring, MagiStream* >( std::wstring(pFullpath), pStream ) );

			m_vLoadList.push_back( pStream );

			return pStream;
		}
	}

	_debug_print( _T("***Resource Error : Invalid Wav File - [%s]\n"), pFullpath );
	//_MBASSERT(0 && Invalid File ); //없는 파일

	return NULL;
}

bool MagiPackageFileSystem::IsExist( const TCHAR* pFullpath, int& nError )
{ 
#ifdef _USE_MEMORYMAPPED_IO
	HANDLE hFile = getMemoryMapedHandle( pFullpath );

	if (hFile != NULL)
	{
		PACK_FILEINFO* pInfo = getPackFileInfo( pFullpath );
		if( pInfo )	{
			nError = _PACK_NO_ERROR;
			return true;
		}
		else {
			nError = _PACK_NOT_FOUND_INFO;
		}
		return false;
	}
#endif
	FILE* fpackage = getPackageFileHandle( pFullpath );
	if( fpackage ) {
		PACK_FILEINFO* pInfo = getPackFileInfo( pFullpath );
		if( pInfo )	{
			nError = _PACK_NO_ERROR;
			return true;
		}
		else {
			nError = _PACK_NOT_FOUND_INFO;
		}
	}
	else {
		nError = _PACK_NOT_FOUND_PACKAGE;
	}

	return false;
}

MagiStream* MagiPackageFileSystem::GetStream( const TCHAR* pFullpath )
{ 
#ifdef _USE_MEMORYMAPPED_IO
	HANDLE hFile = getMemoryMapedHandle( pFullpath );

	if (hFile != NULL)
	{
		PACK_FILEINFO* pInfo = getPackFileInfo( pFullpath );
		if (pInfo != NULL)
		{
			MagiStream * pStream = MagiMemoryMappedStream::Create( hFile, pInfo->uOffSet, pInfo->file_size );
			if (pStream != NULL)
			{
				pStream->IncRef();

				if( m_qLoadList.size() > c_nMaxLoadCount ) {
					for( int i(0); 5>i; ++i ) {
						MagiStream* pData = m_qLoadList.front();
						if( pData->Ref() > 0 ) {
							m_qLoadList.pop();
							m_qLoadList.push( pData ); //맨뒤로 이동
						}
						else {
							//_debug_print( L"delete MagiPackedFileStream : %x\n", pData );
							delete pData;
							m_qLoadList.pop();
						}
					}
				}
			
				m_qLoadList.push( pStream );
				return pStream;
			}
		}
	}
#endif
	FILE* fpackage = getPackageFileHandle( pFullpath );
	if( fpackage ) {
		PACK_FILEINFO* pInfo = getPackFileInfo( pFullpath );
		if( pInfo ) {
			MagiPackedFileStream * pStream = new MagiPackedFileStream( fpackage, pFullpath, pInfo->uOffSet, pInfo->file_size );
			pStream->IncRef();
			//_debug_print( L"new MagiPackedFileStream : %x\n", pStream );

			if( m_qLoadList.size() > c_nMaxLoadCount ) {
				for( int i(0); 5>i; ++i ) {
					MagiStream* pData = m_qLoadList.front();
					if( pData->Ref() > 0 ) {
						m_qLoadList.pop();
						m_qLoadList.push( pData ); //맨뒤로 이동
					}
					else {
						//_debug_print( L"delete MagiPackedFileStream : %x\n", pData );
						delete pData;
						m_qLoadList.pop();
					}
				}
			}
			
			m_qLoadList.push( pStream );
			return pStream;
		}
	}

	if( _tcslen(pFullpath) <= 0 ) {
		_MBASSERT(0 && "empty File Name" );
	}

	_debug_print( _T("***Resource Error : Invalid File - [%s]\n"), pFullpath );
	//_MBASSERT(0 && Invalid File ); //없는 파일

	return NULL;
}

bool MagiPackageFileSystem::ReadIndexFile( const TCHAR* pFileName, bool bDebug /*= false*/ )
{ 
#ifndef _WINDY_AUTH_CHECK_
	CheckPerformance timeBomb( 10, __FILE__, __FUNCTION__, __LINE__ );
#endif

	TCHAR fullPath[_MAX_PATH] = {0,};
	_tcscat_s( fullPath, _MAX_PATH, m_strRoot.c_str() );
	_tcscat_s( fullPath, _MAX_PATH, pFileName );

	MagiFileStream idx_file( fullPath, TEXT("rb") );
	if( !idx_file.IsValid() )
		return false;

	size_t idx_file_size = (size_t)idx_file.GetLength();

	unsigned char* pSrcBuf = new unsigned char[idx_file_size];
	idx_file.Read( pSrcBuf, sizeof(unsigned char)*idx_file_size );
	//XOR
	for( unsigned int i(0); idx_file_size>i; i++ ) pSrcBuf[i] ^= SimpleXORCodec::GetEncodeKeyChar( i );

	//UnCompress
	unsigned long dwDestLen = (unsigned long)idx_file_size*15;
	unsigned char* pOriBuf  = NULL;
	int nRetUnCompress      = 0;

	while( true )
	{
		pOriBuf = new unsigned char[dwDestLen];
		//압축 해제
		nRetUnCompress = Magi_uncompress( pOriBuf, &dwDestLen, pSrcBuf, sizeof(unsigned char)*(unsigned long)idx_file_size );

		if( nRetUnCompress == 0/*Z_OK*/ )
		{
			break;
		}
		else if( nRetUnCompress != -5/*Z_BUF_ERROR*/ )
		{
			assert( 0 && "압축 해제 실패" );
			break;
		}

		dwDestLen += (unsigned long)idx_file_size;  
		delete [] pOriBuf;
	}

	//File Info
	int nPos = 0;
	short nLength = 0;
	TCHAR strFileName[ _MAX_PATH ] = {0,0};
	unsigned char		uPackageIdx = 0;
	size_t				file_size = 0;
	unsigned __int64	uOffSet = 0;
	unsigned long		uCRC32  = 0;

	while( nPos != dwDestLen )
	{
		PACK_FILEINFO * pack_fileinfo = new PACK_FILEINFO;

		memcpy( &nLength    , &pOriBuf[nPos], sizeof(short)            );  nPos += sizeof(short);			 //Dir Length
		memcpy( strFileName , &pOriBuf[nPos], sizeof(TCHAR)*nLength    );  nPos += sizeof(TCHAR)*nLength;	 //Dir
		memcpy( &uPackageIdx, &pOriBuf[nPos], sizeof(unsigned char)    );  nPos += sizeof(unsigned char);	 //Package Index
		memcpy( &file_size  , &pOriBuf[nPos], sizeof(size_t)           );  nPos += sizeof(size_t);			 //File Size
		memcpy( &uCRC32     , &pOriBuf[nPos], sizeof(unsigned long)    );  nPos += sizeof(unsigned long);    //CRC Data
		memcpy( &uOffSet    , &pOriBuf[nPos], sizeof(unsigned __int64) );  nPos += sizeof(unsigned __int64); //OffSet

		strFileName[nLength] = TEXT('\0');

#ifdef _DEBUG
		_debug_print( _T("%s\n"), strFileName );
#endif

		pack_fileinfo->strName		= strFileName;
		pack_fileinfo->uPackageIdx	= uPackageIdx;
		pack_fileinfo->file_size	= file_size;
		pack_fileinfo->uCRC32       = uCRC32;
		pack_fileinfo->uOffSet		= uOffSet;
#ifdef UNICODE
		m_idx_hash.insert( std::pair< std::wstring, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#else
		m_idx_hash.insert( std::pair< std::string, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#endif
	}

	if( bDebug )
		textOutIndexFile();

	delete [] pSrcBuf;
	delete [] pOriBuf;

	return true;
}
#ifdef UNICODE
int MagiPackageFileSystem::GetFileList( std::vector<std::wstring>& filelist )
#else
int MagiPackageFileSystem::GetFileList( std::vector<std::string>& filelist )
#endif
{ 
#ifdef UNICODE
	boost::unordered_map< std::wstring, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#else
	boost::unordered_map< std::string, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#endif
	for(; it != m_idx_hash.end(); ++it )
	{
		PACK_FILEINFO* pInfo = it->second;
		filelist.push_back( pInfo->strName );
	}

	return (int)filelist.size();
}

int MagiPackageFileSystem::ImportStrem( const TCHAR* pFullpath )
{ 
	TCHAR full_path[_MAX_PATH] = {0,};
	_tcscpy_s( full_path, _MAX_PATH, pFullpath ); 

	TCHAR * pCheckRoot = removeDownloadDir( full_path );

	PACK_FILEINFO* pPackInfo = getPackFileInfo( pCheckRoot );

	unsigned char uPackageIdx = MagiFileSystem::GetPackageIndex(pCheckRoot);

	TCHAR full_pack[_MAX_PATH] = {0,};
	TCHAR name_pack[_MAX_PATH] = {0,};
	MagiFileSystem::GetPackageFileName( uPackageIdx, name_pack, _MAX_PATH );

	_tcscpy_s( full_pack, _MAX_PATH, m_strRoot.c_str() ); //Root
	_tcscat_s( full_pack, _MAX_PATH, name_pack );         //Package Name

	MagiFileStream package_file( full_pack, TEXT("abc+") );

	if( !package_file.IsValid() ) return -1;

	unsigned char* pSrcBuf = NULL;

	MagiFileStream it_file( pFullpath, TEXT("rb") );
	size_t file_size = (size_t)it_file.GetLength();
	pSrcBuf = new unsigned char[file_size];
	it_file.Read( pSrcBuf, sizeof(unsigned char)*file_size );

	unsigned long dwCrc32 = SimpleXORCodec::GetCRC( pSrcBuf, (int)file_size );

	if( pPackInfo ) 
	{	//기존것 변경

		//Garbage 인포 추가
		pPackInfo->file_size;
		pPackInfo->uOffSet;
		pPackInfo->uPackageIdx;

		pPackInfo->file_size = file_size;
		pPackInfo->uCRC32    = dwCrc32;
		pPackInfo->uOffSet   = package_file.GetLength();
	}
	else 
	{	//새로 추가
		PACK_FILEINFO *pack_fileinfo = new PACK_FILEINFO;
		pack_fileinfo->strName     = pCheckRoot;
		pack_fileinfo->file_size   = file_size;
		pack_fileinfo->uCRC32      = dwCrc32;
		pack_fileinfo->uOffSet     = package_file.GetLength();
		pack_fileinfo->uPackageIdx = uPackageIdx;
#ifdef UNICODE
		m_idx_hash.insert( std::pair< std::wstring, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#else
		m_idx_hash.insert( std::pair< std::string, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#endif
	}

	package_file.Seek( 0, MagiStream::seek_end );
	package_file.Write( pSrcBuf, sizeof(unsigned char)*file_size );

	delete [] pSrcBuf;
	pSrcBuf = NULL;

	_tcscpy_s( full_pack, _MAX_PATH, m_strRoot.c_str() );		//Root
	_tcscat_s( full_pack, _MAX_PATH, TEXT(c_ResIdxNewFile) );   //New Index File Name
	SaveIndexFile( full_pack );

	return (int)file_size;
}

#ifdef UNICODE
BOOL GetNewFileName( const TCHAR* pName, std::wstring& strNewName )
#else
BOOL GetNewFileName( const TCHAR* pName, std::string& strNewName )
#endif
{ 
	//.new 제거
	strNewName = pName;
	size_t pos = 0;
	pos = strNewName.find( _T(".new"), pos );
	if( pos == std::wstring::npos )
		return FALSE; //압축 파일 아님.

	strNewName = strNewName.substr( 0, pos );

	return TRUE;
}

#ifdef UNICODE
BOOL GetUnCompressName( const TCHAR* pName, std::wstring& strUnCompressName )
#else
BOOL GetUnCompressName( const TCHAR* pName, std::string& strUnCompressName )
#endif
{ 
	//.mip 제거
	strUnCompressName = pName;
	size_t pos = 0;
	pos = strUnCompressName.find( _T(".mip"), pos );
	if( pos == std::wstring::npos )
		return FALSE; //압축 파일 아님.

	strUnCompressName = strUnCompressName.substr( 0, pos );

	//패쳐 예외 처리
	if( !_wcsicmp( strUnCompressName.c_str(), _T("mpatcher.exe" ) ) ) {
		strUnCompressName += _T(".new" );
	}
	//패쳐 예외 처리
	if( !_wcsicmp( strUnCompressName.c_str(), _T("bugtrapu.dll" ) ) ) {
		strUnCompressName += _T(".new" );
	}
	//패쳐 예외 처리
	if( !_wcsicmp( strUnCompressName.c_str(), _T("steam_appid.txt" ) ) ) {
		strUnCompressName += _T(".new" );
	}

	return TRUE;
}

int MagiPackageFileSystem::ImportCompressStrem( const TCHAR* pFullpath )
{ 
	TCHAR full_path[_MAX_PATH] = {0,};
	_tcscpy_s( full_path, _MAX_PATH, pFullpath ); 

	TCHAR * pCheckRoot = removeDownloadDir( full_path );

	//.mip 제거
#ifdef UNICODE
	std::wstring strUnCompressName;
#else
	std::string strUnCompressName;
#endif
	if( !GetUnCompressName( pCheckRoot, strUnCompressName ) )
		return -1;
		
	PACK_FILEINFO* pPackInfo = getPackFileInfo( strUnCompressName.c_str() );

	unsigned char uPackageIdx = MagiFileSystem::GetPackageIndex( strUnCompressName.c_str() );

	TCHAR full_pack[_MAX_PATH] = {0,};
	TCHAR name_pack[_MAX_PATH] = {0,};
	MagiFileSystem::GetPackageFileName( uPackageIdx, name_pack, _MAX_PATH );

	_tcscpy_s( full_pack, _MAX_PATH, m_strRoot.c_str() ); //Root
	_tcscat_s( full_pack, _MAX_PATH, name_pack );         //Package Name

	MagiFileStream package_file( full_pack, TEXT("abc+") );
	if( !package_file.IsValid() ) return -1;

	unsigned char* pSrcBuf = NULL;

	MagiCompressFileStream it_file( pFullpath, TEXT("rb") );

 	it_file.UnCompress();
	size_t file_size = it_file.GetUnCompressBufLen();
	pSrcBuf          = it_file.GetUnCompressBuf();
	
	if( pSrcBuf == NULL || file_size <= 0 )
		return -2;

	unsigned long dwCrc32 = SimpleXORCodec::GetCRC( pSrcBuf, (int)file_size );

	if( pPackInfo ) 
	{	//기존것 변경

		//Garbage 인포 추가
		pPackInfo->file_size;
		pPackInfo->uOffSet;
		pPackInfo->uPackageIdx;

		pPackInfo->file_size = file_size;
		pPackInfo->uCRC32    = dwCrc32;
		pPackInfo->uOffSet   = package_file.GetLength();
	}
	else 
	{	//새로 추가
		PACK_FILEINFO *pack_fileinfo = new PACK_FILEINFO;
		pack_fileinfo->strName     = strUnCompressName.c_str();
		pack_fileinfo->file_size   = file_size;
		pack_fileinfo->uCRC32      = dwCrc32;
		pack_fileinfo->uOffSet     = package_file.GetLength();
		pack_fileinfo->uPackageIdx = uPackageIdx;
#ifdef UNICODE
		m_idx_hash.insert( std::pair< std::wstring, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#else
		m_idx_hash.insert( std::pair< std::string, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#endif
	}

	package_file.Seek( 0, MagiStream::seek_end );
	package_file.Write( pSrcBuf, sizeof(unsigned char)*file_size );

	_tcscpy_s( full_pack, _MAX_PATH, m_strRoot.c_str() );		//Root
	_tcscat_s( full_pack, _MAX_PATH, TEXT(c_ResIdxNewFile) );   //New Index File Name
	SaveIndexFile( full_pack );

	return (int)file_size;
}
/*
int MagiPackageFileSystem::ImportDir( const TCHAR* pFullpath )
{ 
	m_FolderAnalyse.SetInterface( this );
	m_FolderAnalyse.SetRoot( pFullpath );
	m_FolderAnalyse.DoAnalyse( true );

	unsigned long threadId = GetCurrentThreadId();
#ifdef _DEBUG
	_debug_print( TEXT( "2 THREAD_ID %d\n"), threadId );
#endif

	return 0;
}

int MagiPackageFileSystem::ImportCompressDir( const TCHAR* pFullpath )
{ 
	m_bCompressMode = true;
	m_FolderAnalyse.SetInterface( this );
	m_FolderAnalyse.SetRoot( pFullpath );
	m_FolderAnalyse.DoAnalyse( false );

	unsigned long threadId = GetCurrentThreadId();
#ifdef _DEBUG
	_debug_print( TEXT( "2 THREAD_ID %d\n"), threadId );
#endif

	return 0;
}
*/

bool MagiPackageFileSystem::SaveIndexFile( const TCHAR* pFileName )
{ 
	unsigned char* pSrcBuf = NULL;
	unsigned char* pDstBuf = NULL;

	try
	{
		MagiFileStream file_list( _T("temp.dat"), _T("wbc+") );
		MagiFileStream xor_file_list( pFileName, _T("wbc") );
#ifdef UNICODE
		boost::unordered_map< std::wstring, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#else
		boost::unordered_map< std::string, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#endif
		for(; it != m_idx_hash.end(); ++it )
		{
			PACK_FILEINFO* pack_fileinfo = it->second;

			short nLength             = (short)pack_fileinfo->strName.length();
			unsigned char uPackageIdx = pack_fileinfo->uPackageIdx;
			size_t file_size          = pack_fileinfo->file_size;
			unsigned long dwCrc32     = pack_fileinfo->uCRC32;
			unsigned __int64 uOffSet  = pack_fileinfo->uOffSet;

			//File Idx Info
			file_list.Write( &nLength    , sizeof(short) );          //Dir
			file_list.Write( pack_fileinfo->strName.c_str(), sizeof(TCHAR)*nLength );  //Dir Length
			file_list.Write( &uPackageIdx, sizeof(unsigned char)    );  //Package Index
			file_list.Write( &file_size  , sizeof(size_t)           );  //File Size
			file_list.Write( &dwCrc32    , sizeof(unsigned long)    );  //CRC Data
			file_list.Write( &uOffSet    , sizeof(unsigned __int64) );  //OffSet
		}

		size_t flen = (size_t)file_list.GetLength();
		unsigned long dwDestLen = (unsigned long)(flen + Magi_compressBufSize( (unsigned long)flen ));

		pSrcBuf = new unsigned char[flen];
		pDstBuf = new unsigned char[dwDestLen];
		memset( pSrcBuf, 0, sizeof(unsigned char)*flen );
		memset( pDstBuf, 0, sizeof(unsigned char)*dwDestLen );

		file_list.Read( pSrcBuf, flen );

		//Make Packing Info File 
		//화일 리스트 압축 
		Magi_compress( pDstBuf, &dwDestLen, pSrcBuf, sizeof(unsigned char)*(unsigned long)flen );
		//화일 리스트 XOR
		for( unsigned int i(0); dwDestLen>i; i++ ) pDstBuf[i] ^= SimpleXORCodec::GetEncodeKeyChar( i );
		xor_file_list.Write( pDstBuf, sizeof(unsigned char)*dwDestLen );

		delete [] pSrcBuf;
		delete [] pDstBuf;

		return true;
	}
	catch( std::bad_alloc& ba )
	{
#ifdef _DEBUG
		_debug_printA( "%s\n", ba.what() );
#endif

		if( pSrcBuf ) delete [] pSrcBuf;
		if( pDstBuf ) delete [] pDstBuf;
	}

	return false;
}

void MagiPackageFileSystem::OnNotify( short nMsg )
{ 
	MagiFileSystem::OnNotify( nMsg );

	switch( nMsg )
	{
	case FolderAnalyse::ST_ANALYSED :
		{
			if( m_bCompressMode ) {
				if( m_pGUI )
					m_pGUI->OnWorkInfo( m_FolderAnalyse.GetFileCount() );

				doImportCompressDir();

				if( m_pGUI )
					m_pGUI->OnWorkComplete();
			}
			else
				doImportDir();
		}
		break;
	}
}

TCHAR* MagiPackageFileSystem::removeDownloadDir( TCHAR* pFullPath )
{ 
	TCHAR * pCheckRoot = pFullPath;
	pCheckRoot += m_strRoot.size();    //ROOT, CJK 문제 없을까?
	pCheckRoot += m_strDownDir.size(); //DownLoad Dir

	return pCheckRoot;
}

void MagiPackageFileSystem::textOutIndexFile()
{ 
	TCHAR fullPath[_MAX_PATH] = {0,};
	_tcscpy_s( fullPath, _MAX_PATH, m_strRoot.c_str() );	   //Root
	_tcscat_s( fullPath, _MAX_PATH, TEXT(c_debugTextFile) );   //Debug File List

	MagiFileStream package_file( fullPath, TEXT("wt+") );

	const short c_tempSize = 512;

	TCHAR szTemp[c_tempSize] = {0,};
#ifdef UNICODE
	boost::unordered_map< std::wstring, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#else
	boost::unordered_map< std::string, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#endif

	package_file.FilePrint( TEXT("[I]          CRC            OFFSET            SIZE	NAME\n") );
	package_file.FilePrint( TEXT("----------------------------------------------------------\n") );

	for(; it != m_idx_hash.end(); ++it )
	{
		PACK_FILEINFO* pInfo = it->second;
		
		package_file.FilePrint( TEXT("[%d]\t%08x\t% 10I64u\t% 10d\t%s \n"), pInfo->uPackageIdx, pInfo->uCRC32, pInfo->uOffSet, pInfo->file_size, pInfo->strName.c_str() );

		ZeroMemory( szTemp, c_tempSize );
	}
}

void MagiPackageFileSystem::doImportDir()
{ 
	unsigned long threadId = GetCurrentThreadId();
#ifdef _DEBUG
	_debug_print( TEXT( "1 THREAD_ID %d\n"), threadId );
#endif

	unsigned char* pSrcBuf = NULL;

	TCHAR fullPath[_MAX_PATH] = {0,};

	try
	{
		std::vector< FolderAnalyse::LINFO_FILE >& filelist = m_FolderAnalyse.GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();

		if( filelist.size() <= 0 ) return;

		MagiFileStream package_file[c_nPackageCount];
		unsigned __int64 uOffSet[c_nPackageCount];

		TCHAR package_name[_MAX_PATH] = {0,};
		TCHAR package_path[_MAX_PATH] = {0,};

		for( int i(0); c_nPackageCount>i; i++ )
		{
			_stprintf_s( package_name, TEXT("%03d.pck"), i );
			_tcscpy_s( package_path, _MAX_PATH, m_strRoot.c_str() );
			_tcscat_s( package_path, _MAX_PATH, package_name );

			package_file[i].Open( package_path, TEXT("abc+") );

			uOffSet[i] = package_file[i].GetLength();
			package_file[i].Seek( 0, MagiStream::seek_end );
		}

		for( ; it != filelist.end(); ++it )
		{
			//소문자 변경
			std::transform((*it).strDirName.begin() , (*it).strDirName.end() , (*it).strDirName.begin() , (int(*)(int)) std::tolower);
			std::transform((*it).strFileName.begin(), (*it).strFileName.end(), (*it).strFileName.begin(), (int(*)(int)) std::tolower);

			_tcscpy_s( fullPath, _MAX_PATH, (*it).strDirName.c_str() );
			_tcscat_s( fullPath, _MAX_PATH, (*it).strFileName.c_str() );

			TCHAR * pCheckRoot = removeDownloadDir( fullPath );

			MagiFileStream it_file( fullPath, TEXT("rb") );
			size_t file_size = (size_t)it_file.GetLength();
			pSrcBuf = new unsigned char[file_size];
			it_file.Read( pSrcBuf, sizeof(unsigned char)*file_size );
			unsigned long dwCrc32 = SimpleXORCodec::GetCRC( pSrcBuf, (int)file_size );
			unsigned char uPackageIdx = MagiFileSystem::GetPackageIndex(pCheckRoot);

			PACK_FILEINFO* pPackInfo = getPackFileInfo( pCheckRoot );
			if( pPackInfo )
			{
				//Modify
				pPackInfo->file_size = file_size;
				pPackInfo->uCRC32    = dwCrc32;
				pPackInfo->uOffSet   = uOffSet[uPackageIdx];
			}
			else 
			{
				PACK_FILEINFO* pack_fileinfo = new PACK_FILEINFO;
				pack_fileinfo->strName     = pCheckRoot;
				pack_fileinfo->file_size   = file_size;
				pack_fileinfo->uCRC32      = dwCrc32;
				pack_fileinfo->uOffSet     = uOffSet[uPackageIdx];
				pack_fileinfo->uPackageIdx = uPackageIdx;
#ifdef UNICODE
				m_idx_hash.insert( std::pair< std::wstring, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#else
				m_idx_hash.insert( std::pair< std::string, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#endif
			}

			uOffSet[uPackageIdx] += file_size; //OffSet
			package_file[uPackageIdx].Write( pSrcBuf, sizeof(unsigned char)*file_size );

			delete [] pSrcBuf;
			pSrcBuf = NULL;
		}
	}
	catch( std::bad_alloc ba )
	{
		if( pSrcBuf ) delete [] pSrcBuf;
		pSrcBuf = NULL;
	}

	_tcscpy_s( fullPath, _MAX_PATH, m_strRoot.c_str() );	   //Root
	_tcscat_s( fullPath, _MAX_PATH, TEXT(c_ResIdxNewFile) );   //New Index File Name
	SaveIndexFile( fullPath );
}

struct String_Great
{
#ifdef UNICODE
	bool operator()( const std::wstring& a, const std::wstring& b )
#else
	bool operator()( const std::string& a, const std::string& b )
#endif
	{
		if( a.length() > b.length() )
			return true;
		return false;
	}
};

void exceptPatchFile( MagiFileStream& it_file, TCHAR* tempPath )
{
	it_file.Close();
#ifdef UNICODE
	std::wstring strNewPatcherFile; 
#else
	std::string strNewPatcherFile; 
#endif
	GetNewFileName( tempPath, strNewPatcherFile );

#ifdef UNICODE
	std::wstring strOldPatcherFile = strNewPatcherFile + L".old";  
#else
	std::string strOldPatcherFile = strNewPatcherFile + ".old";  
#endif
	//이전 패쳐 삭제
	DeleteFile( strOldPatcherFile.c_str() );

	//기존 패쳐 이름 변경
	MoveFile( strNewPatcherFile.c_str(), strOldPatcherFile.c_str() );

	CopyFile( tempPath, strNewPatcherFile.c_str(), FALSE );
	DeleteFile( tempPath );
}

void MagiPackageFileSystem::doImportCompressDir()
{ 
	//압축 해제 후 묶음
	unsigned long threadId = GetCurrentThreadId();
#ifdef _DEBUG
	_debug_print( TEXT( "1 THREAD_ID %d\n"), threadId );
#endif

	unsigned char* pSrcBuf = NULL;

	TCHAR fullPath[_MAX_PATH] = {0,};

	try
	{
		std::vector< FolderAnalyse::LINFO_FILE >& filelist = m_FolderAnalyse.GetFileList();
		std::vector< FolderAnalyse::LINFO_FILE >::iterator it = filelist.begin();

		if( filelist.size() <= 0 ) return;

		MagiFileStream package_file[c_nPackageCount];
		unsigned __int64 uOffSet[c_nPackageCount];

		TCHAR package_name[_MAX_PATH] = {0,};
		TCHAR package_path[_MAX_PATH] = {0,};
		TCHAR tempPath[_MAX_PATH] = {0,};

		for( int i(0); c_nPackageCount>i; i++ )
		{
			_stprintf_s( package_name, TEXT("%03d.pck"), i );
			_tcscpy_s( package_path, _MAX_PATH, m_strRoot.c_str() );
			_tcscat_s( package_path, _MAX_PATH, package_name );

			package_file[i].Open( package_path, TEXT("abc+") );

			uOffSet[i] = package_file[i].GetLength();
			package_file[i].Seek( 0, MagiStream::seek_end );
		}

		int nProgress = 1;
		for( ; it != filelist.end(); ++it )
		{
			if( m_pGUI )
				m_pGUI->OnProgressInfo( nProgress++, (*it).strFileName.c_str() );

			if( FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".exe" )) ||
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".dll" )) ||
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".flt" )) ||  
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".asi" )) ||
				FolderAnalyse::IsExceptionList( (*it).strFileName, _T(".ico" )) ||
				!_tcsicmp( (*it).strFileName.c_str(), _T("Service.dat.mip") ) ||
				!_tcsicmp( (*it).strFileName.c_str(), _T("steam_appid.txt.mip") ) )
			{
				//예외 리스트 처리 : 압축 해제 후 바로 복사	
				_tcscpy_s( fullPath, _MAX_PATH, (*it).strDirName.c_str() );
				_tcscat_s( fullPath, _MAX_PATH, (*it).strFileName.c_str() );

				//Root + DownLoad(archive\\)
#ifdef UNICODE
				std::wstring strSubDir = (*it).strDirName.substr( _tcslen(m_strRoot.c_str())+_tcslen(m_strDownDir.c_str()), (*it).strDirName.length() );
				std::wstring strUnCompressName;
#else
				std::string strSubDir = (*it).strDirName.substr( _tcslen(m_strRoot.c_str())+_tcslen(m_strDownDir.c_str()), (*it).strDirName.length() );
				std::string strUnCompressName;
#endif

				//.mip 제거
				if( !GetUnCompressName( (*it).strFileName.c_str(), strUnCompressName ) )
					continue; //압축 파일 아님

				_tcscpy_s( tempPath, _MAX_PATH, m_strRoot.c_str() );
				if( strSubDir.length() )
					_tcscat_s( tempPath, _MAX_PATH, strSubDir.c_str() );
				_tcscat_s( tempPath, _MAX_PATH, strUnCompressName.c_str() );

				//하위 폴더 생성
				FolderAnalyse::CreateSubDirectory( tempPath );

				//기존 파일 삭제
				DeleteFile( tempPath );

				//압축 해제후 복사
				MagiCompressFileStream co_file( fullPath, TEXT("rb") );
				if( !co_file.IsValid() )
					continue;

				co_file.UnCompress();
				MagiFileStream it_file( tempPath, TEXT("wbc") );
				if( !it_file.IsValid() ) //BugTrapU.dll 를 패치 할경우, 문제 소지 있음. BugTrapU.dll은 Patcher 도 사용 중..
					continue;

				it_file.Write( co_file.GetUnCompressBuf(), co_file.GetUnCompressBufLen() );

				//패쳐 업데이트
				if( !_wcsicmp( strUnCompressName.c_str(), _T("mpatcher.exe.new" ) ) ) {
					exceptPatchFile( it_file, tempPath );

					m_pGUI->OnPatcherUpdate();
				}

				//bugtrapu.dll 예외 처리
				if( !_wcsicmp( strUnCompressName.c_str(), _T("bugtrapu.dll.new" ) ) ) {
					exceptPatchFile( it_file, tempPath );
				}

				//bugtrapu.dll 예외 처리
				if( !_wcsicmp( strUnCompressName.c_str(), _T("steam_appid.txt.new" ) ) ) {
					exceptPatchFile( it_file, tempPath );
				}

				Sleep(10);

				continue;
			}

			//소문자 변경
			std::transform((*it).strDirName.begin() , (*it).strDirName.end() , (*it).strDirName.begin() , (int(*)(int)) std::tolower);
			std::transform((*it).strFileName.begin(), (*it).strFileName.end(), (*it).strFileName.begin(), (int(*)(int)) std::tolower);

			_tcscpy_s( fullPath, _MAX_PATH, (*it).strDirName.c_str() );
			_tcscat_s( fullPath, _MAX_PATH, (*it).strFileName.c_str() );

			TCHAR * pCheckRoot = removeDownloadDir( fullPath );
#ifdef UNICODE
			std::wstring strUnCompressName;
#else
			std::string strUnCompressName;
#endif
			//.mip 제거
			if( !GetUnCompressName( pCheckRoot, strUnCompressName ) )
				continue; //압축 파일 아님

			MagiCompressFileStream it_file( fullPath, TEXT("rb") );
			if( !it_file.IsValid() )
				continue;

			it_file.UnCompress();

			pSrcBuf          = it_file.GetUnCompressBuf();
			size_t file_size = it_file.GetUnCompressBufLen();

			unsigned long dwCrc32     = SimpleXORCodec::GetCRC( pSrcBuf, (int)file_size );
			unsigned char uPackageIdx = MagiFileSystem::GetPackageIndex( strUnCompressName.c_str() );

			PACK_FILEINFO* pPackInfo = getPackFileInfo( strUnCompressName.c_str() );
			if( pPackInfo )
			{
				//Modify
				pPackInfo->file_size = file_size;
				pPackInfo->uCRC32    = dwCrc32;
				pPackInfo->uOffSet   = uOffSet[uPackageIdx];
			}
			else 
			{
				PACK_FILEINFO* pack_fileinfo = new PACK_FILEINFO;
				pack_fileinfo->strName     = strUnCompressName.c_str();
				pack_fileinfo->file_size   = file_size;
				pack_fileinfo->uCRC32      = dwCrc32;
				pack_fileinfo->uOffSet     = uOffSet[uPackageIdx];
				pack_fileinfo->uPackageIdx = uPackageIdx;
#ifdef UNICODE
				m_idx_hash.insert( std::pair< std::wstring, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#else
				m_idx_hash.insert( std::pair< std::string, PACK_FILEINFO* >( pack_fileinfo->strName, pack_fileinfo ) );
#endif
			}

			uOffSet[uPackageIdx] += file_size; //OffSet
			package_file[uPackageIdx].Write( pSrcBuf, sizeof(unsigned char)*file_size );

			pSrcBuf = NULL;

			Sleep(10);
		}

		if( m_pGUI )
			m_pGUI->OnEtcInfo( _T("Delete Patch File.") );
		
		//패치 리스트 삭제
		it = filelist.begin();
		for( ; it != filelist.end(); ++it )
		{
			//예외 리스트 처리 : 압축 해제 후 바로 복사	
			_tcscpy_s( fullPath, _MAX_PATH, (*it).strDirName.c_str() );
			_tcscat_s( fullPath, _MAX_PATH, (*it).strFileName.c_str() );

			DeleteFile( fullPath );
		}

		if( m_pGUI )
			m_pGUI->OnEtcInfo( TEXT("Delete Patch Dir..") );

		//패치 하위 Dir 삭제
#ifdef UNICODE
		std::vector< std::wstring >& vDirList = FolderWorkList::GetSingleton()->GetDirList();
#else
		std::vector< std::string >& vDirList = FolderWorkList::GetSingleton()->GetDirList();
#endif
		std::sort( vDirList.begin(), vDirList.end(), String_Great() );

		for( UINT i(0); vDirList.size()>i; ++i ) {
			RemoveDirectory( vDirList[i].c_str() );
		}
	}
	catch( std::bad_alloc ba )
	{
		if( pSrcBuf ) delete [] pSrcBuf;
		pSrcBuf = NULL;
	}

	_tcscpy_s( fullPath, _MAX_PATH, m_strRoot.c_str() );	   //Root
	_tcscat_s( fullPath, _MAX_PATH, TEXT(c_ResIdxNewFile) );   //New Index File Name
	SaveIndexFile( fullPath );
}


#ifdef UNICODE 
bool MagiPackageFileSystem::GetTempStream( const TCHAR* pFileName, std::wstring& TempFile )
#else
bool MagiPackageFileSystem::GetTempStream( const TCHAR* pFileName, std::string& TempFile )
#endif
{ 
	MagiStream* pStream = GetStream( pFileName );
	if( pStream ) {
		unsigned char* pBuf = new unsigned char[(size_t)pStream->GetLength()];
		pStream->Read( pBuf, sizeof(unsigned char)*(size_t)pStream->GetLength() );

		TCHAR export_name[_MAX_PATH] = {0,};

		_tcscpy_s( export_name, _MAX_PATH, _T("temp\\") );
		_tcscat_s( export_name, _MAX_PATH, pFileName );

		FolderAnalyse::CreateSubDirectory( export_name );

		MagiFileStream export_file( export_name, TEXT( "wb" ) );
		export_file.Write( pBuf, sizeof(unsigned char)*(size_t)pStream->GetLength() );

		delete [] pBuf;

		TempFile = export_name;
		return true;
	}

	return false;
}

#ifdef UNICODE
bool IsExistDir( const TCHAR* pDir, std::vector<std::wstring>& dirlist )
#else
bool IsExistDir( const TCHAR* pDir, std::vector<std::string>& dirlist )
#endif
{ 
	 
#ifdef UNICODE
	std::vector<std::wstring>::iterator it = dirlist.begin();
#else
	std::vector<std::string>::iterator it = dirlist.begin();
#endif
	for( ; it != dirlist.end(); ++it ) {
		if( _tcsicmp( pDir, (*it).c_str() ) == 0 )
			return true;
	}

	return false;
}

#ifdef UNICODE
int MagiPackageFileSystem::GetSubDirList( const TCHAR* pRootDir, std::vector<std::wstring>& dirlist )
#else
int MagiPackageFileSystem::GetSubDirList( const TCHAR* pRootDir, std::vector<std::string>& dirlist )
#endif
{ 
#ifdef UNICODE
	boost::unordered_map< std::wstring, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#else
	boost::unordered_map< std::string, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#endif

#ifdef UNICODE
	static const std::wstring::size_type npos = -1;
	std::wstring::size_type pos;
	std::wstring strTemp;
#else
	static const std::string::size_type npos = -1;
	std::string::size_type pos;
	std::string strTemp;
#endif

	for(; it != m_idx_hash.end(); ++it )
	{
		PACK_FILEINFO* pInfo = it->second;

		if( _tcsnicmp( pRootDir, pInfo->strName.c_str(), _tcslen(pRootDir) ) == 0 ) {

			strTemp = pInfo->strName.c_str();
			//파일 이름 삭제
			pos = strTemp.rfind( _T("\\") );
			if( pos != npos ) {
				strTemp.erase( pos, strTemp.length() );
			}

			if( !IsExistDir( strTemp.c_str(), dirlist ) )
				dirlist.push_back( strTemp );
		}
	}

	return static_cast<int>(dirlist.size());
}

#ifdef UNICODE
int MagiPackageFileSystem::GetDirFileList( const TCHAR* pDir, std::vector<std::wstring>& filelist )
#else
int MagiPackageFileSystem::GetDirFileList( const TCHAR* pDir, std::vector<std::string>& filelist )
#endif
{
#ifdef UNICODE
	boost::unordered_map< std::wstring, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#else
	boost::unordered_map< std::string, PACK_FILEINFO* >::iterator it = m_idx_hash.begin();
#endif

	for(; it != m_idx_hash.end(); ++it )
	{
		PACK_FILEINFO* pInfo = it->second;

		if( _tcsnicmp( pDir, pInfo->strName.c_str(), _tcslen(pDir) ) == 0 ) {
			filelist.push_back( pInfo->strName );
		}
	}

	return static_cast<int>(filelist.size());
}
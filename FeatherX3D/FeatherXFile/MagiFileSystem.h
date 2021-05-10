

#pragma once

#include <string>
#include <tchar.h>
#include <hash_map>
#include <boost/unordered/unordered_map.hpp>
#include <queue>
#include <map>
#include "FolderAnalyse.h"

class MagiStream;
class MagiFileSystem 
{
public:
	static bool s_bUsePackedFile;
	static bool IsPackedFile() { return s_bUsePackedFile; }

public:
	MagiFileSystem( const TCHAR * pRoot );
	virtual ~MagiFileSystem() {}

	//Basic
	virtual MagiStream* GetStream( const TCHAR* pFullpath ) = 0;
	virtual bool ReadIndexFile( const TCHAR* pFileName, bool bDebug = false ) { return false; };
	virtual bool SaveIndexFile( const TCHAR* pFileName ) { return false; };

	virtual MagiStream* GetWaveStream( const TCHAR* pFullpath ) { return NULL; };

#ifdef UNICODE
	virtual bool GetTempStream( const TCHAR* pFileName, std::wstring& tempFile ) { tempFile = pFileName; return true; }
//Utility
	virtual int GetSubDirList( const TCHAR* pRootDir, std::vector<std::wstring>& dirlist ) = 0;
	virtual int GetDirFileList( const TCHAR* pDir, std::vector<std::wstring>& filelist ) = 0;
	virtual int GetFileList( std::vector<std::wstring>& filelist ) { return 0; }
	virtual int GetDirFileListbyExt( const TCHAR* pDir, const TCHAR* pExt, std::vector<std::wstring>& filelist ) { return 0; }
#else
	virtual bool GetTempStream( const TCHAR* pFileName, std::string&  tempFile ) { tempFile = pFileName; return true; }
//Utility
	virtual int GetSubDirList( const TCHAR* pRootDir, std::vector<std::string>& dirlist ) = 0;
	virtual int GetDirFileList( const TCHAR* pDir, std::vector<std::string>& filelist ) = 0;
	virtual int GetFileList( std::vector<std::string>& filelist ) { return 0; }
	virtual int GetDirFileListbyExt( const TCHAR* pDir, const TCHAR* pExt, std::vector<std::string>& filelist ) { return 0; }
#endif

	//InterFace
	virtual void OnNotify( short nMsg );
	virtual bool IsExist( const TCHAR* pFullpath, int& nError ) { nError = 0; return false; }

	enum
	{
		_first,
		_second,

		_res_max,
	};

	static void DeInit( const int nIndex ) {
		if( s_pFileSystem[nIndex] ) {
			delete s_pFileSystem[nIndex];
			s_pFileSystem[nIndex] = NULL;
		}
	}	

	static MagiFileSystem* Instance(const int nIndex = _first) {
		if( nIndex >= _first && nIndex < _res_max )
			return s_pFileSystem[nIndex];
		return NULL;
	}

	static unsigned char GetPackageIndex( const TCHAR* pFileName )
	{
		int key = 5381;
		int ch = 0;
		for ( int i = 0; pFileName[i]; ++i )
		{
			ch = pFileName[i];
			key = ((key<< 5) + key) + ch; // key * 33 + ch
		}
		
		return key%m_nPackageCount;
	}

	static void	GetPackageFileName( size_t hash_key, TCHAR* pBuf, int nBufSize )
	{
		_stprintf_s( pBuf, nBufSize, TEXT("%03d.pck"), hash_key );
	}

	TCHAR* removeRootDir( TCHAR* pFullPath )
	{
		TCHAR * pCheckRoot = pFullPath;
		if( !_tcsnicmp( m_strRoot.c_str(), pFullPath, m_strRoot.length() ) )
		{
			pCheckRoot += m_strRoot.size();    //ROOT, CJK 문제 없을까?
		}
		return pCheckRoot;
	}
#ifdef UNICODE
	void removeRootDir( std::wstring& strFullPath );
#else
	void removeRootDir( std::string& strFullPath );
#endif

protected:

#ifdef UNICODE
	std::wstring		m_strRoot;
#else
	std::string			m_strRoot;
#endif

	bool				m_bReadOnly;

	static unsigned int	m_nPackageCount;



	static MagiFileSystem*	s_pFileSystem[_res_max];

private:
};

//////////////////////////////////////////////////////////////////////////
//기본 폴더 구조
class MagiBasicFileSystem : public MagiFileSystem
{
public:
	static void Init( const TCHAR * pRoot, bool bInitSkip, const int nIndex );
	
	MagiBasicFileSystem( const TCHAR * pRoot, bool bInitSkip );
	virtual ~MagiBasicFileSystem();

	virtual MagiStream* GetStream( const TCHAR* pFullpath );
#ifdef UNICODE
	virtual int GetSubDirList( const TCHAR* pRootDir, std::vector<std::wstring>& dirlist ) { return 0; };
	virtual int GetDirFileList( const TCHAR* pDir, std::vector<std::wstring>& filelist );
	virtual int GetDirFileListbyExt( const TCHAR* pDir, const TCHAR* pExt, std::vector<std::wstring>& filelist );
#else
	virtual int GetSubDirList( const TCHAR* pRootDir, std::vector<std::string>& dirlist ) { return 0; };
	virtual int GetDirFileList( const TCHAR* pDir, std::vector<std::string>& filelist );
	virtual int GetDirFileListbyExt( const TCHAR* pDir, const TCHAR* pExt, std::vector<std::string>& filelist );
#endif

protected:
	void init();

	MagiStream* findCache( const TCHAR* pFullpath );

#ifdef UNICODE
	boost::unordered_map< std::wstring, std::wstring >	m_all_file_hash;
	boost::unordered_map< std::wstring, MagiStream* >	m_file_cache_hash;
#else
	boost::unordered_map< std::string, std::string >	m_all_file_hash;
	boost::unordered_map< std::string, MagiStream* >	m_file_cache_hash;
#endif

private:

};

class FileSystemGUI
{
public:
	FileSystemGUI() {}
	virtual ~FileSystemGUI() {}

	virtual void OnWorkInfo( int nWorkCount ) { }
	virtual void OnProgressInfo( int nWorkedCount, const TCHAR* pFileName ) { }
	virtual void OnEtcInfo( const TCHAR* pInfo ) { }
	virtual void OnPatcherUpdate() { }
	virtual void OnWorkComplete() { }
};

//////////////////////////////////////////////////////////////////////////
//패키징 화일
class MagiPackageFileSystem : public MagiFileSystem
{
public:
	enum {	MAX_HANDLE_COUNT = 10 };

	static void Init( const TCHAR * pRoot );

	MagiPackageFileSystem( const TCHAR * pRoot, bool bReadOnly );
	virtual ~MagiPackageFileSystem();
	
	//Basic
	virtual MagiStream* GetStream( const TCHAR* pFullpath );
	virtual bool ReadIndexFile( const TCHAR* pFileName, bool bDebug = false );
	virtual bool SaveIndexFile( const TCHAR* pFileName );

#ifdef UNICODE
	virtual bool GetTempStream( const TCHAR* pFileName, std::wstring& tempFile );
	virtual int GetSubDirList( const TCHAR* pRootDir, std::vector<std::wstring>& dirlist );
	virtual int GetDirFileList( const TCHAR* pDir, std::vector<std::wstring>& filelist );
	virtual int GetFileList( std::vector<std::wstring>& filelist );
#else
	virtual bool GetTempStream( const TCHAR* pFileName, std::string&  tempFile );
	virtual int GetSubDirList( const TCHAR* pRootDir, std::vector<std::string>& dirlist );
	virtual int GetDirFileList( const TCHAR* pDir, std::vector<std::string>& filelist );
	virtual int GetFileList( std::vector<std::string>& filelist );
#endif

	virtual MagiStream* GetWaveStream( const TCHAR* pFullpath );

	//Utility
	

	//InterFace
	virtual void OnNotify( short nMsg );

	//Launcher Interface
	//int ImportDir( const TCHAR* pFullpath );
	int ImportStrem( const TCHAR* pFullpath );

	//int ImportCompressDir( const TCHAR* pFullpath );
	int ImportCompressStrem( const TCHAR* pFullpath );

	void SetGUI( class FileSystemGUI* pGUI ) { m_pGUI = pGUI; }

	bool IsExist( const TCHAR* pFullpath, int& nError );

	enum _PACK_ERROR_INFO_
	{
		_PACK_NO_ERROR          = 0,
		_PACK_NOT_FOUND_PACKAGE = -1,
		_PACK_NOT_FOUND_INFO    = -2,
	};

protected:

	MagiStream* findCache( const TCHAR* pFullpath );

	void doImportDir();
	TCHAR* removeDownloadDir( TCHAR* pFullPath );

	void doImportCompressDir();

	void textOutIndexFile();

	typedef struct _PACK_FILEINFO_
	{
		//File Info
#ifdef UNICODE
		std::wstring		strName;
#else
		std::string			strName;
#endif
		unsigned char		uPackageIdx;
		size_t				file_size;
		unsigned long		uCRC32;
		unsigned __int64	uOffSet;
	} PACK_FILEINFO;

	

#ifdef UNICODE
	boost::unordered_map< std::wstring, PACK_FILEINFO* >	m_idx_hash;
	boost::unordered_map< std::wstring, MagiStream* >		m_file_cache_hash;
#else
	boost::unordered_map< std::string, PACK_FILEINFO* >		m_idx_hash;
	boost::unordered_map< std::string, MagiStream* >		m_file_cache_hash;
#endif

	
	std::vector< MagiStream* > m_vLoadList;
	std::queue< MagiStream* >  m_qLoadList;

	struct PackageFileHandle
	{
		PackageFileHandle()
		{
			for( int i(0); MAX_HANDLE_COUNT>i; ++i )
				handle[i] = NULL;
		}

		FILE*	handle[ MAX_HANDLE_COUNT ];
	};

	void clear();
	FILE* getPackageFileHandle( const TCHAR* pFullpath );
	MagiPackageFileSystem::PACK_FILEINFO* getPackFileInfo( const TCHAR* pFullpath );
#ifdef _USE_MEMORYMAPPED_IO
	HANDLE getMemoryMapedHandle( const TCHAR* pFullpath );
#endif

	bool m_bCompressMode;
	std::map< int, PackageFileHandle >	m_mapPackageFileHandle;

#ifdef UNICODE
	std::wstring		m_strDownDir;
#else
	std::string			m_strDownDir;
#endif

	FolderAnalyse		m_FolderAnalyse;

	HANDLE              m_hWaitEvent;

	FileSystemGUI*		m_pGUI;

#ifdef _USE_MEMORYMAPPED_IO
	std::map < std::wstring, HANDLE > m_hMMList;
#endif

private:

};
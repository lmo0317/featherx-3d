

#ifndef _FOLDER_ANALYSE_H_
#define _FOLDER_ANALYSE_H_

#include <tchar.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <hash_map>
#include <boost/unordered/unordered_map.hpp>

template<class T>
struct _AUTO_ARRAY_ 
{
	_AUTO_ARRAY_( T* pBuf ) : pAutoBuf(pBuf)
	{

	}

	~_AUTO_ARRAY_() {
		if( pAutoBuf ) {
			delete [] pAutoBuf;
			pAutoBuf = NULL;
		}
	}

	T* pAutoBuf;
};

class FolderWorkList : public FBSingleton<FolderWorkList>
{
public:
	FolderWorkList();
	~FolderWorkList();

	void AddFolder( TCHAR* pTStr );
	void AddFile( const TCHAR* pDir, int nDirLen, const TCHAR* pFile, int nFileLen );

	void MakeFile();

#ifdef UNICODE
	std::vector< std::wstring >& GetDirList() { return m_vFolder; }
	void GetReNameDir( const TCHAR* pFromDir, const TCHAR* pToDir, std::vector<std::wstring>& updirList );
#else
	std::vector< std::string >& GetDirList() { return m_vFolder; }
	void GetReNameDir( const TCHAR* pFromDir, const TCHAR* pToDir, std::vector<std::string>& updirList );
#endif

protected:

	struct FILE_DATA
	{
		std::string strDir;
		std::string strFile;
	};

#ifdef UNICODE
	std::vector< std::wstring >	m_vFolder;
#else
	std::vector< std::string >	m_vFolder;
#endif
	std::vector< FILE_DATA >	m_vFile;

private:
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct FolderAnalyseIF
{
	virtual void OnNotify( short nMsg ) = 0;
};

class FolderAnalyse
{
public:

	enum
	{
		ST_NONE = 0,
		ST_ANALYZING,
		ST_ANALYSED,
 		ST_PACKING ,
		ST_PACKED  ,
		ST_COMPRESSING,
		ST_COMPRESSED,
		ST_UNCOMPRESSING,
		ST_UNCOMPRESSED,
		ST_MOVING,
		ST_MOVEED,
		ST_COMPRESS_DIR,
		ST_0BYTE,
		ST_MAKE_LOWER,
		ST_MAKE_UPPER,
	};

	typedef struct _LINFO_FILE_
	{
		_LINFO_FILE_()
		{
			uOriginSize   = 0;
			uCompressSize = 0;
			uCrc32        = 0;
		};

#ifdef UNICODE
		std::wstring strDirName;
		std::wstring strFileName;
#else
		std::string strDirName;
		std::string strFileName;
#endif

		size_t		uOriginSize;
		size_t		uCompressSize;

		unsigned long uCrc32;

	} LINFO_FILE;

	FolderAnalyse();
	FolderAnalyse( const TCHAR* pszRoot );
	~FolderAnalyse();
	
	void SetInterface( FolderAnalyseIF* pIF ) { m_pIF = pIF; }

	//Main Job
	void DoAnalyse( bool bWaitThread = false );
	void DoCompress();
	void DoUnCompress();
	void DoEncode();
	void DoMove();
	void Do0Byte();
	void DoMakeLower();
	void DoMakeUpper();

	void AddFile( const TCHAR* dir, const TCHAR* file );
	void ClearFileList();

	void AddDir( const TCHAR* dir );
	void ClearDirList();

	//Notify
	void OnAnalyseComplete();
	void OnPackingComplete();
	void OnCompressingComplete();
	void OnUnCompressingComplete();
	void OnMoveComplete();

	void Process( DWORD time );

	void SetRoot( const TCHAR* pszRoot );
	const TCHAR* GetRoot()	{	return m_strRoot.c_str();	}

	void SetOut( const TCHAR* pszOutPut );
	const TCHAR* GetOut()	{	return m_strOut.c_str();	}

	//Util
	void SetError( DWORD dwErr ) { m_dwError = dwErr; }

	std::vector< LINFO_FILE >& GetFileList() { return m_vFileList; }
	int  GetFileCount() { return (int)m_vFileList.size(); }

#ifdef UNICODE
	std::vector< std::wstring >& GetDirList() { return  m_vDirList; }
#else
	std::vector< std::string >&  GetDirList() { return  m_vDirList; }
#endif

	short GetState()					{ return m_curState;	}

	short GetNextState()				{ return m_nextState;	}
	void  SetNextState( short nState )	{ m_nextState = nState; }

	void SetFilter( bool bUseFilter )   { m_bUseFilter = bUseFilter; }
	const bool IsUseFilter()			{ return m_bUseFilter;  }

	//Utility
	static void CreateSubDirectory( TCHAR* pFullPath );
#ifdef UNICODE
	static bool IsExceptionList( std::wstring& str, TCHAR* pFind );
#else
	static bool IsExceptionList( std::string& str, TCHAR* pFind );
#endif

protected:

	void DoPacking();

	void  setState( short state ) { m_curState = state; if( m_pIF ) m_pIF->OnNotify( m_curState ); }

	short	m_nextState;
	short	m_curState;
	bool	m_bUseFilter;

	DWORD m_dwError;

#ifdef UNICODE
	std::wstring	m_strRoot;
	std::wstring	m_strOut;
#else
	std::string		m_strRoot;
	std::string		m_strOut;
#endif

	std::vector< LINFO_FILE > m_vFileList;

#ifdef UNICODE
	std::vector< std::wstring > m_vDirList;
#else
	std::vector< std::string > m_vDirList;
#endif

	FolderAnalyseIF* m_pIF;
};

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//File List Compare
class CheckPatch
{
public:
	CheckPatch()  {}
	~CheckPatch();

	void DoPatch( const TCHAR* pFileSrc, const TCHAR* pFileDst );

protected:

	typedef struct _PatchInfo_
	{
		unsigned long	dwCrc32;		
		size_t			nFileSize;
		std::string		strName;
	} PATCHINFO ;

	void makeData( const TCHAR* pFileSrc, std::vector<std::string>& datalist );
	void parseData( boost::unordered_map< std::string, PATCHINFO* >& refhash, std::vector<std::string>& datalist );
	
	boost::unordered_map< std::string, PATCHINFO* >		m_client_filelist;
	boost::unordered_map< std::string, PATCHINFO* >		m_server_filelist;
	
	std::vector< PATCHINFO* > m_down_list;

private:

};


#endif
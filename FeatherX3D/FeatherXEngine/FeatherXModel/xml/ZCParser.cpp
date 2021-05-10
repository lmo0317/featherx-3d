#include "FeatherXEnginePrecompile.h"

ZCParser::ZCParser()
{
	m_pXML = new ZXML;
}

ZCParser::~ZCParser()
{
	m_pXML->Close();
	S_DEL( m_pXML );
}

int	ZCParser::Parse_XML( ZCParsedData* pData, WCHAR* lpszFilename )
{
	if( !m_pXML->Open( lpszFilename ) )
	{
		//CFeatherXOutputWindow::GetSingleton()->Log( "can't open [%s] file.", lpszFilename );
		return 0;
	}
	try
	{
		
		if( !_ParseInfo( pData ) )
		{
			//CFeatherXOutputWindow::GetSingleton()->Log( "can't parse info block in [%] file.", lpszFilename );
			throw;
		}
		
		if( !_ParseMaterial( pData ) )
		{
			//CFeatherXOutputWindow::GetSingleton()->Log( "can't parse material block in [%] file.", lpszFilename );
			throw;
		}

		if( !_ParseObject( pData ) )
		{
			//CFeatherXOutputWindow::GetSingleton()->Log( "can't parse object block in [%] file.", lpszFilename );
			throw;
		}
		
		return 1;
	}
	catch(...)
	{
		return 0;
	}	
}

/// TriIndex에 따라서 삼각형은 sub material정보를 새로운 ZCMesh오브젝트로 분리한다.
int	ZCParser::_SplitMeshByFaceMaterial( ZCParsedData* pData )
{
	int							i, j, cnt;
	list<ZCMesh*>::iterator		it, jt;
	ZCMesh*						pMesh;
	ZCMesh*						pOrig;

	for( it = pData->m_meshList.begin() ; it != pData->m_meshList.end() ; it++ )
	{
		/// Sub-Material이 있는가?
		if( (*it)->m_nMaterialCount > 1 )
		{
			// sub-meterial개수만큼 sub-mesh가 생성되므로
			// strObjectNameTable을 sub-mesh개수만큼 늘려준다.
			// 이때, id와 배열의 위치가 매칭되도록 하는 것이 중요하다
			// sub-mesh의 이름들은 전부다 공백문자 " " 이다.
			pData->m_info.strObjectNameTable.insert( pData->m_info.strObjectNameTable.begin() + (*it)->m_nObjectID + 1, (*it)->m_nMaterialCount-1, L" " );
			
			// sub-meterial개수만큼 sub-mesh가 생성되므로
			// BoneTables이 가리키는 뼈대의 번호중 생성되는 sub-mesh에 해당하는 번호는 
			// 그만큼 뒤로 밀어야 한다.
			for( i = 0 ; i < pData->m_info.BoneTable.size() ; i++ )
			{
				if( pData->m_info.BoneTable[i] > (*it)->m_nObjectID )
					pData->m_info.BoneTable[i] += ((*it)->m_nMaterialCount-1);
			}

			// sub-meterial개수만큼 sub-mesh가 생성되므로
			// object id와 parent id값들을 중간에 끼어넣는 sub-mesh개수만큼 증가시킨다
			for( jt = pData->m_meshList.begin() ; jt != pData->m_meshList.end() ; jt++ )
			{
				if( (*jt)->m_nObjectID > (*it)->m_nObjectID )
					(*jt)->m_nObjectID += ((*it)->m_nMaterialCount-1);
				if( (*jt)->m_nParentID > (*it)->m_nObjectID )
					(*jt)->m_nParentID += ((*it)->m_nMaterialCount-1);
			}

			j = 0;
			pOrig = *it;
			it = pData->m_meshList.erase( it );	// 원본 mesh를 리스트에서만 삭제한다
												// 아직 pOrig는 유효하다
			for( i = 0 ; i < pOrig->m_nMaterialCount ; i++ )
			{
				pMesh = new ZCMesh();
				pMesh->m_strNodeType = pOrig->m_strNodeType;
				pMesh->m_nObjectID = pOrig->m_nObjectID+i;	// object id를 증가시키며 submesh등록
				pMesh->m_nObjectHashID = pOrig->m_nObjectHashID;
				pMesh->m_nParentID = pOrig->m_nParentID;
				pMesh->m_bboxMax = pOrig->m_bboxMax;
				pMesh->m_bboxMin = pOrig->m_bboxMin;
				pMesh->m_tmLocal = pOrig->m_tmLocal;
				pMesh->m_tmWorld = pOrig->m_tmWorld;
				pMesh->m_vertex = pOrig->m_vertex;
				pMesh->m_normal = pOrig->m_normal;
				pMesh->m_tvertex = pOrig->m_tvertex;
				pMesh->m_nMaterialCount = 1;
//				pMesh->m_texindex = pOrig->m_texindex;
				pMesh->m_vertweight = pOrig->m_vertweight;
				pMesh->m_track = pOrig->m_track;
				pMesh->m_isSkinned = pOrig->m_isSkinned;
				cnt = pOrig->m_triindex.size() - 1;
				for( ; j < cnt ; )
				{
					if( pOrig->m_triindex[j].ID == pOrig->m_triindex[j+1].ID )
					{
						pMesh->m_triindex.push_back( pOrig->m_triindex[j] );
						if( pOrig->m_texindex.size() )
							pMesh->m_texindex.push_back( pOrig->m_texindex[j] );
					}
					else
					{
						j++;
						if( j == cnt )	/// 제일 마지막 index정보를 복사한다.
						{
							pMesh->m_triindex.push_back( pOrig->m_triindex[j] );
							if( pOrig->m_texindex.size() )
								pMesh->m_texindex.push_back( pOrig->m_texindex[j] );
						}

						break;
					}
					j++;
				}
				// 생성한 sub-mesh를 child로 등록
				// (STL의 insert는 it앞에 insert한다.)
				pData->m_meshList.insert( it, pMesh );
			}
			S_DEL( pOrig );	// sub-mesh를 만들었으니 원본mesh를 메모리에서 삭제
		}
	}
	pData->m_info.nObjectCount = pData->m_meshList.size();
	return 1;
}

void ZCParser::_MergeBlendWeight( VertMerged& vm, VertWeight& vw )
{
	int			i;
	VertWeight	t = vw;
	int			cnt = t.ID.size();

	/// blend weight가 4개이하라면 강제로 4개로 만든다. 
	if( cnt < 4 )
	{
		t.ID.resize( 4 );
		t.fWeight.resize( 4 );
		for( i = cnt ; i < 4 ; i++ )
		{
			t.ID[i] = 0;
			t.fWeight[i] = 0;
		}
	}
	
	/// script에서 출력한 가중치값은 큰값순서로(올림차순) 정렬되어있다.
	/// 때문에 앞에서 4개만 값을 취하면 된다.
	for( i = 0 ; i < 3 ; i++ )
	{
		vm.b[i] = t.fWeight[i];
	}
	vm.i = BLENDINDEX( t.ID[3], t.ID[2], t.ID[1], t.ID[0] );
}

int	ZCParser::_MergeVertex( ZCParsedData* pData )
{
	int							i, j, cnt;
	list<ZCMesh*>::iterator		it, jt;
	ZCMesh*						pMesh;
	VertMerged*					pVert;
	Index3i*					pIndex;
	DWORD						vindex, tindex;

	for( it = pData->m_meshList.begin() ; it != pData->m_meshList.end() ; it++ )
	{
		pMesh = *it;
		pVert = new VertMerged[ pMesh->m_vertex.size() + pMesh->m_triindex.size() * 3 ];
		pIndex = new Index3i[ pMesh->m_triindex.size() ];
		for( i = 0 ; i < pMesh->m_vertex.size() ; i++ )
		{
			pVert[i].p = pMesh->m_vertex[i];
			pVert[i].n = pMesh->m_normal[i];

			// BlendWeight가 있는 오브젝트 처리
			if( (*it)->m_isSkinned ) 
				_MergeBlendWeight( pVert[i], pMesh->m_vertweight[i] );

			pVert[i].t.x = BIGFLOAT;
			pVert[i].t.y = BIGFLOAT;
		}

		for( i = 0 ; i < pMesh->m_triindex.size() ; i++ )
			pIndex[i] = pMesh->m_triindex[i].index;

		cnt = pMesh->m_vertex.size();

		/// 텍스쳐가 있는 오브젝트인가?
		if( pMesh->m_texindex.size() )
		{
			for( i = 0 ; i < pMesh->m_triindex.size() ; i++ )
			{
				for( j = 0 ; j < 3 ; j++ )
				{
					vindex = pIndex[i].i[j];			// 정점의 인덱스
					tindex = pMesh->m_texindex[i].i[j];	// 텍스처의 인덱스
					// 아직 값이 대입되지 않았으면 텍스처 좌표 입력
					if( pVert[vindex].t.x == BIGFLOAT || pVert[vindex].t.y == BIGFLOAT )
					{
						pVert[vindex].t = pMesh->m_tvertex[tindex];
					}
					else	// 만약 텍스처 값이 이미 있으면 새로운 정점 생성
					{
						pIndex[i].i[j] = cnt;
						pVert[cnt] = pVert[vindex];
						pVert[cnt].t = pMesh->m_tvertex[tindex];
						cnt++;
					}
				}
			}
		}
		pMesh->m_nMaterialID = pMesh->m_triindex.size() ? max( pMesh->m_triindex[0].ID, 0 ) : 0;
		pMesh->m_vtxFinal.resize( cnt );
		pMesh->m_idxFinal.resize( pMesh->m_triindex.size() );
		for( i = 0 ; i < cnt ; i++ ) pMesh->m_vtxFinal[i] = pVert[i];
		for( i = 0 ; i < pMesh->m_triindex.size() ; i++ ) pMesh->m_idxFinal[i] = pIndex[i];
		S_DELS( pVert );
		S_DELS( pIndex );
	}

	return 1;
}

int	ZCParser::_ParseInfo( ZCParsedData* pData )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pList = NULL;
	IXMLDOMNode*		pNode = NULL;

	//CFeatherXOutputWindow::GetSingleton()->Log( "Parsing Info" );

	try
	{
		pList = m_pXML->FindElement( L"Body/Info/ObjectCount" );	pList->get_item( 0, &pNode );	m_pXML->GetElementText( pNode, str );
		pData->m_info.nObjectCount = _wtoi( str );

		pList = m_pXML->FindElement( L"Body/Info/MeshCount" );	pList->get_item( 0, &pNode );	m_pXML->GetElementText( pNode, str );
		pData->m_info.nMeshCount = _wtoi( str );
		
		pList = m_pXML->FindElement( L"Body/Info/AnimationStart" );	pList->get_item( 0, &pNode );	m_pXML->GetElementText( pNode, str );
		pData->m_info.fAnimationStart = _wtoi( str );
		
		pList = m_pXML->FindElement( L"Body/Info/AnimationEnd" );	pList->get_item( 0, &pNode );	m_pXML->GetElementText( pNode, str );
		pData->m_info.fAnimationEnd = _wtoi( str );
		
		pList = m_pXML->FindElement( L"Body/Info/KeyType" );	pList->get_item( 0, &pNode );	m_pXML->GetElementText( pNode, str );
		pData->m_info.nKeyType = 1;
		
		pList = m_pXML->FindElement( L"Body/Info/Object/Value" );
		pList->get_length( &nCount );
		pData->m_info.strObjectNameTable.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pList->get_item( i, &pNode );
			m_pXML->GetElementText( pNode, str );
			pData->m_info.strObjectNameTable[i] = str;
			//CFeatherXOutputWindow::GetSingleton()->Log( "Objects[%d] = %s", i, pData->m_info.strObjectNameTable[i].c_str() );
		}

		pList = m_pXML->FindElement( L"Body/Info/Bone/Value" );
		pList->get_length( &nCount );
		pData->m_info.BoneTable.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pList->get_item( i, &pNode );
			m_pXML->GetElementText( pNode, str );
			m_pXML->GetAttributeText( pNode, L"ID", str );
			pData->m_info.BoneTable[i] = _wtoi( str );
			//CFeatherXOutputWindow::GetSingleton()->Log( "Bone[%d] = %s", i, pData->m_info.strObjectNameTable[atoi(str)].c_str() );
		}


		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseMaterial( ZCParsedData* pData )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pList = NULL;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNode = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//CFeatherXOutputWindow::GetSingleton()->Log( "Parsing Material" );

	try
	{
		pList = m_pXML->FindElement( L"Body/Material/Slot" );
		pList->get_length( &nCount );
		pData->m_materialTable.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pList->get_item( i, &pNode );
			pListChild = m_pXML->FindElement( pNode, L"Diffuse" );
			pListChild->get_item( 0, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pData->m_materialTable[i].diffuse = _StrToColor3f( str );

			pListChild = m_pXML->FindElement( pNode, L"Ambient" );
			pListChild->get_item( 0, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pData->m_materialTable[i].ambient = _StrToColor3f( str );

			pListChild = m_pXML->FindElement( pNode, L"Specular" );
			pListChild->get_item( 0, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pData->m_materialTable[i].specular = _StrToColor3f( str );

			pListChild = m_pXML->FindElement( pNode, L"Emissive" );
			pListChild->get_item( 0, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pData->m_materialTable[i].emissive = _StrToColor3f( str );

			pListChild = m_pXML->FindElement( pNode, L"Opacity" );
			pListChild->get_item( 0, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pData->m_materialTable[i].opacity = _wtof( str );

			pListChild = m_pXML->FindElement( pNode, L"Power" );
			pListChild->get_item( 0, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pData->m_materialTable[i].power = _wtof( str );

			pListChild = m_pXML->FindElement( pNode, L"DiffuseMap" );
			if( S_OK == pListChild->get_item( 0, &pNodeChild ) )
			{
				m_pXML->GetElementText( pNodeChild, str );
				pData->m_materialTable[i].strMapDiffuse = str;
			}

			pListChild = m_pXML->FindElement( pNode, L"SpecularMap" );
			if( S_OK == pListChild->get_item( 0, &pNodeChild ) )
			{
				m_pXML->GetElementText( pNodeChild, str );
				pData->m_materialTable[i].strMapSpecular = str;
			}

			pListChild = m_pXML->FindElement( pNode, L"BumpMap" );
			if( S_OK == pListChild->get_item( 0, &pNodeChild ) )
			{
				m_pXML->GetElementText( pNodeChild, str );
				pData->m_materialTable[i].strMapBump = str;
			}

			pListChild = m_pXML->FindElement( pNode, L"DisplacementMap" );
			if( S_OK == pListChild->get_item( 0, &pNodeChild ) )
			{
				m_pXML->GetElementText( pNodeChild, str );
				pData->m_materialTable[i].strMapDisplace = str;
			}
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObject( ZCParsedData* pData )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pList = NULL;
	IXMLDOMNode*		pNode = NULL;
	ZCMesh*				pMesh;

	//CFeatherXOutputWindow::GetSingleton()->Log( "Parsing Objects..." );

	try
	{
		pList = m_pXML->FindElement( L"Body/Object" );
		pList->get_length( &nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pMesh = new ZCMesh();
			pData->m_meshList.push_back( pMesh );

			pList->get_item( i, &pNode );
			m_pXML->GetAttributeText( pNode, L"ID", str );
			pMesh->m_nObjectID = _wtoi( str );
			m_pXML->GetAttributeText( pNode, L"Class", str );
			pMesh->m_strNodeType = str;

			//CFeatherXOutputWindow::GetSingleton()->Log( "Parsing [%s]...", pData->m_info.strObjectNameTable[i].c_str() );

			if( !_ParseObjectParent( pMesh, pNode ) ) throw;
			if( !_ParseObjectBBox( pMesh, pNode ) ) throw;
			if( !_ParseObjectTM( pMesh, pNode ) ) throw;
			if( !_ParseObjectVertex( pMesh, pNode ) ) throw;
			if( !_ParseObjectVertexNormal( pMesh, pNode ) ) throw;
			if( !_ParseObjectTVertex( pMesh, pNode ) ) throw;
			if( !_ParseObjectTriIndex( pMesh, pNode ) ) throw;
			if( !_ParseObjectTexIndex( pMesh, pNode ) ) throw;
			if( !_ParseObjectVertWeight( pMesh, pNode ) ) throw;
			if( !_ParseObjectKey( pMesh, pNode ) ) throw;

			pMesh->m_track.start = pData->m_info.fAnimationStart;
			pMesh->m_track.end = pData->m_info.fAnimationEnd;
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectParent( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	WCHAR				str[128];
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing Parent" ) );
	
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"Parent" );
		pListChild->get_item( 0, &pNodeChild );
		m_pXML->GetAttributeText( pNodeChild, L"ID", str );
		pMesh->m_nParentID = _wtoi( str );
		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectBBox( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	WCHAR				str[128];
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing BoundingBox" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"BoundingBox/Max" );
		pListChild->get_item( 0, &pNodeChild );
		m_pXML->GetElementText( pNodeChild, str );
		pMesh->m_bboxMax = _StrToVector3f( str );

		pListChild = m_pXML->FindElement( pNode, L"BoundingBox/Min" );
		pListChild->get_item( 0, &pNodeChild );
		m_pXML->GetElementText( pNodeChild, str );
		pMesh->m_bboxMin = _StrToVector3f( str );
		
		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectTM( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing TM" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"LocalTM/Value" );
		pListChild->get_length( &nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_tmLocal.row[i] = _StrToVector4f( str );
		}

		pListChild = m_pXML->FindElement( pNode, L"WorldTM/Value" );
		pListChild->get_length( &nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_tmWorld.row[i] = _StrToVector4f( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectVertex( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing Vertex" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"Vertex/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_vertex.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_vertex[i] = _StrToVector3f( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectVertexNormal( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing VertexNormal" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"VertexNormal/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_normal.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_normal[i] = _StrToVector3f( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectTVertex( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing TexVertex" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"TexVertex/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_tvertex.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_tvertex[i] = _StrToVector2f( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectTriIndex( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing TriIndex" ) );
	try
	{
		/// Get material count
		pListChild = m_pXML->FindElement( pNode, L"TriIndex" );
		pListChild->get_item( 0, &pNodeChild );
		if( pNodeChild == NULL )
		{
			pMesh->m_nMaterialCount = 0;
			return 1;
		}
		m_pXML->GetAttributeText( pNodeChild, L"MaterialCount", str );
		pMesh->m_nMaterialCount = _wtoi( str );

		pListChild = m_pXML->FindElement( pNode, L"TriIndex/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_triindex.resize( nCount );

		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetAttributeText( pNodeChild, L"MaterialID", str );
			pMesh->m_triindex[i].ID = _wtoi( str );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_triindex[i].index = _StrToIndex3i( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectTexIndex( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing TexIndex" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"TexIndex/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_texindex.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_texindex[i] = _StrToIndex3i( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectVertWeight( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i, j;
	WCHAR				str[128];
	long				nCount = 0;
	long				nCount2 = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNodeList*	pListChild2 = NULL;
	IXMLDOMNode*		pNodeChild = NULL;
	IXMLDOMNode*		pNodeChild2 = NULL;

	//assert( CFeatherXOutputWindow::GetSingleton()->Log( "Parsing VertexWeight" ) );
	try
	{
		pListChild = m_pXML->FindElement( pNode, L"VertexWeight/Vertex" );
		pListChild->get_length( &nCount );
		if( 0 == nCount ) { pMesh->m_isSkinned = false; return 1; }
		pMesh->m_isSkinned = true;

		pMesh->m_vertweight.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			pListChild2 = m_pXML->FindElement( pNodeChild, L"Value" );
			pListChild2->get_length( &nCount2 );
			pMesh->m_vertweight[i].ID.resize( nCount2 );
			pMesh->m_vertweight[i].fWeight.resize( nCount2 );
			for( j = 0 ; j < nCount2 ; j++ )
			{
				pListChild2->get_item( j, &pNodeChild2 );
				m_pXML->GetAttributeText( pNodeChild2, L"BoneID", str );
				pMesh->m_vertweight[i].ID[j] = _wtoi( str );
				//assert( CFeatherXOutputWindow::GetSingleton()->Log( "vertex[%d] Bone ID=%d", i, pMesh->m_vertweight[i].ID[j] ) );

				pListChild2->get_item( j, &pNodeChild2 );
				m_pXML->GetElementText( pNodeChild2, str );
				pMesh->m_vertweight[i].fWeight[j] = (float)_wtof( str );
				//assert( CFeatherXOutputWindow::GetSingleton()->Log( "vertex[%d] Bone Weight=%f", i, pMesh->m_vertweight[i].fWeight[j] ) );
			}
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}

int	ZCParser::_ParseObjectKey( ZCMesh* pMesh, IXMLDOMNode* pNode )
{
	int					i;
	WCHAR				str[128];
	long				nCount = 0;
	IXMLDOMNodeList*	pListChild = NULL;
	IXMLDOMNode*		pNodeChild = NULL;

	try
	{
		pListChild = m_pXML->FindElement( pNode, L"Key/Position/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_track.pos.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetAttributeText( pNodeChild, L"Frame", str );
			pMesh->m_track.pos[i].frame = (float)_wtof( str );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_track.pos[i].value = _StrToVector3f( str );
		}

		pListChild = m_pXML->FindElement( pNode, L"Key/Quaternion/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_track.rot.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetAttributeText( pNodeChild, L"Frame", str );
			pMesh->m_track.rot[i].frame = (float)_wtof( str );
			m_pXML->GetElementText( pNodeChild, str );
			pMesh->m_track.rot[i].value = _StrToVector4f( str );
		}

		pListChild = m_pXML->FindElement( pNode, L"Key/Scale/Value" );
		pListChild->get_length( &nCount );
		pMesh->m_track.scl.resize( nCount );
		for( i = 0 ; i < nCount ; i++ )
		{
			pListChild->get_item( i, &pNodeChild );
			m_pXML->GetAttributeText( pNodeChild, L"Frame", str );
			pMesh->m_track.scl[i].frame = (float)_wtof( str );
			m_pXML->GetElementText( pNodeChild, str );
			//assert( CFeatherXOutputWindow::GetSingleton()->Log( "[%d]th [%f]frame key=%s", i, pMesh->m_track.scl[i].frame, str ) );
			pMesh->m_track.scl[i].value = _StrToVector3f( str );
		}

		return 1;
	}
	catch(...)
	{
		return 0;
	}
}
#ifdef UNICODE
Index3i ZCParser::_StrToIndex3i( WCHAR str[] )
{
	Index3i		val;
	WCHAR*		sep = L" [],\n";

	val.i[0] = _wtoi( wcstok( str, sep ) );
	val.i[1] = _wtoi( wcstok( NULL, sep ) );
	val.i[2] = _wtoi( wcstok( NULL, sep ) );

	return val;
}

Color3f ZCParser::_StrToColor3f( WCHAR str[] )
{
	Color3f		val;
	WCHAR*		sep = L" [],\n";

	wcstok( str, sep );		// skip 'Color' string
	val.r = (float)_wtof( wcstok( NULL, sep ) ) / 255.0f;
	val.g = (float)_wtof( wcstok( NULL, sep ) ) / 255.0f;
	val.b = (float)_wtof( wcstok( NULL, sep ) ) / 255.0f;

	return val;
}

Vector2f ZCParser::_StrToVector2f( WCHAR str[] )
{
	Vector2f	val;
	WCHAR*		sep = L" [],\n";

	val.x = (float)_wtof( wcstok( str, sep ) );
	val.y = (float)_wtof( wcstok( NULL, sep ) );

	return val;
}

Vector3f ZCParser::_StrToVector3f( WCHAR str[] )
{
	Vector3f	val;
	WCHAR*		sep = L" [],\n";

	val.x = (float)_wtof( wcstok( str, sep ) );
	val.y = (float)_wtof( wcstok( NULL, sep ) );
	val.z = (float)_wtof( wcstok( NULL, sep ) );

	return val;
}

Vector4f ZCParser::_StrToVector4f( WCHAR str[] )
{
	Vector4f	val;
	WCHAR*		sep = L" [],\n";

	val.x = (float)_wtof( wcstok( str, sep ) );
	val.y = (float)_wtof( wcstok( NULL, sep ) );
	val.z = (float)_wtof( wcstok( NULL, sep ) );
	val.w = (float)_wtof( wcstok( NULL, sep ) );

	return val;
}
#else
Index3i ZCParser::_StrToIndex3i( char str[] )
{
	Index3i		val;
	char*		sep = " [],\n";

	val.i[0] = atoi( strtok( str, sep ) );
	val.i[1] = atoi( strtok( NULL, sep ) );
	val.i[2] = atoi( strtok( NULL, sep ) );

	return val;
}

Color3f ZCParser::_StrToColor3f( char str[] )
{
	Color3f		val;
	char*		sep = " [],\n";

	strtok( str, sep );		// skip 'Color' string
	val.r = (float)atof( strtok( NULL, sep ) ) / 255.0f;
	val.g = (float)atof( strtok( NULL, sep ) ) / 255.0f;
	val.b = (float)atof( strtok( NULL, sep ) ) / 255.0f;

	return val;
}

Vector2f ZCParser::_StrToVector2f( char str[] )
{
	Vector2f	val;
	char*		sep = " [],\n";

	val.x = (float)atof( strtok( str, sep ) );
	val.y = (float)atof( strtok( NULL, sep ) );

	return val;
}

Vector3f ZCParser::_StrToVector3f( char str[] )
{
	Vector3f	val;
	char*		sep = " [],\n";

	val.x = (float)atof( strtok( str, sep ) );
	val.y = (float)atof( strtok( NULL, sep ) );
	val.z = (float)atof( strtok( NULL, sep ) );

	return val;
}

Vector4f ZCParser::_StrToVector4f( char str[] )
{
	Vector4f	val;
	char*		sep = " [],\n";

	val.x = (float)atof( strtok( str, sep ) );
	val.y = (float)atof( strtok( NULL, sep ) );
	val.z = (float)atof( strtok( NULL, sep ) );
	val.w = (float)atof( strtok( NULL, sep ) );

	return val;
}
#endif

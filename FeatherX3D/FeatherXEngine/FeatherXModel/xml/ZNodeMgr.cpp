#include "FeatherXEnginePrecompile.h"

ZNodeMgr::ZNodeMgr( LPDIRECT3DDEVICE9 pDev, ZCParsedData* pData, SKINMETHOD sm )
{
	int		i;

	m_pDev = pDev;
	D3DXMatrixIdentity( &m_matTM );

	// copy info
	m_info.nObjectCount		= pData->m_info.nObjectCount;
	m_info.nSkinType		= pData->m_info.nSkinType;
	m_info.nMeshCount		= pData->m_info.nMeshCount;
	m_info.fAnimationStart	= pData->m_info.fAnimationStart;
	m_info.fAnimationEnd	= pData->m_info.fAnimationEnd;
	m_info.nKeyType			= pData->m_info.nKeyType;
	m_info.BoneTable		= pData->m_info.BoneTable;
	m_info.strObjectNameTable = pData->m_info.strObjectNameTable;

	// ��Ʈ���� �ȷ�Ʈ�� Bone�� ������
	m_matPalette.resize( m_info.BoneTable.size() );

	for( i = 0 ; i < pData->m_meshList.size() ; i++ )
	{
		if( !_IsBone( i ) )
			m_info.MeshTable.push_back( i );
	}

	// copy materials
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	wstring str;

	int size = pData->m_materialTable.size();
	m_materials.resize( size );

	for( i = 0 ; i < size ; i++ )
	{
		// ������ ��
		m_materials[i].opacity = pData->m_materialTable[i].opacity;	// �� ���� diffuse�� alpha������...

		// ������ Ȯ�걤 �Ӽ� ����
		D3DCOLORVALUE diffuse = { pData->m_materialTable[i].diffuse.r, pData->m_materialTable[i].diffuse.g, pData->m_materialTable[i].diffuse.b, m_materials[i].opacity };
		m_materials[i].material.Diffuse = diffuse;

		// ������ ȯ�汤 �Ӽ� ����
		D3DCOLORVALUE ambient = { pData->m_materialTable[i].ambient.r, pData->m_materialTable[i].ambient.g, pData->m_materialTable[i].ambient.b, m_materials[i].opacity };
		m_materials[i].material.Ambient = ambient;
		
		// ������ �ݻ籤 �Ӽ� ����
		D3DCOLORVALUE specular = { pData->m_materialTable[i].specular.r, pData->m_materialTable[i].specular.g, pData->m_materialTable[i].specular.b, m_materials[i].opacity };
		m_materials[i].material.Specular = specular;
		
		// ������ �߻걤 �Ӽ� ����
		D3DCOLORVALUE emissive = { pData->m_materialTable[i].emissive.r, pData->m_materialTable[i].emissive.g, pData->m_materialTable[i].emissive.b, m_materials[i].opacity };
		m_materials[i].material.Emissive = emissive;

		// �ݻ籤�� �������
		m_materials[i].material.Power = pData->m_materialTable[i].power;

		// Ȯ��� �ؽ�ó
		m_materials[i].pTex[0] = NULL;
		char *s = (char *)pData->m_materialTable[i].strMapDiffuse.c_str();
		_wsplitpath_s( pData->m_materialTable[i].strMapDiffuse.c_str(), drive, dir, fname, ext ); str = fname; str += ext;
		D3DXCreateTextureFromFile( pDev, str.c_str(), &m_materials[i].pTex[0] );

		// ������ �ؽ�ó
		m_materials[i].pTex[1] = NULL;
		_wsplitpath_s( pData->m_materialTable[i].strMapBump.c_str(), drive, dir, fname, ext ); str = fname; str += ext;
		D3DXCreateTextureFromFile( pDev, str.c_str(), &m_materials[i].pTex[1] );
		
		// �ݻ�� �ؽ�ó
		m_materials[i].pTex[2] = NULL;
		_wsplitpath_s( pData->m_materialTable[i].strMapSpecular.c_str(), drive, dir, fname, ext ); str = fname; str += ext;
		D3DXCreateTextureFromFile( pDev, str.c_str(), &m_materials[i].pTex[2] );
		
		// �׿��� �߰����� �ؽ�ó
		m_materials[i].pTex[3] = NULL;
		_wsplitpath_s( pData->m_materialTable[i].strMapDisplace.c_str(), drive, dir, fname, ext ); str = fname; str += ext;
		D3DXCreateTextureFromFile( pDev, str.c_str(), &m_materials[i].pTex[3] );
	}

	// copy meshes
	list<ZCMesh*>::iterator	it;
	ZNode*					pNode;
	size = pData->m_meshList.size();
	D3DXVECTOR3	vMinBox, vMaxBox;
	D3DXVECTOR3	vMin, vMax;
	vMinBox = D3DXVECTOR3( 0, 0, 0 );
	vMaxBox = D3DXVECTOR3( 0, 0, 0 );
	for( i = 0, it = pData->m_meshList.begin() ; it != pData->m_meshList.end() ; i++, it++ )
	{
		// �θ� ���� ���� 0����带 �θ�� �Ѵ�.
		if( (*it)->m_nParentID == -1 ) 
			(*it)->m_nParentID = 0;

		// Bone�̳� Biped��� ZBone���� ����
		if( !( (*it)->m_strNodeType.compare( L"BoneGeometry" ) ) || 
			!( (*it)->m_strNodeType.compare( L"Biped_Object" ) ) || 
			!( (*it)->m_strNodeType.compare( L"Bone" ) ) ||
			( (*it)->m_nObjectID == 0 ) )
		{
			pNode = (ZNode*)(new ZBone( pDev, *it ));
		}
		else	// Editable_Mesh��� ZMesh�� ����
		if( !( (*it)->m_strNodeType.compare( L"Editable_mesh" ) ) )
		{
			if( (*it)->m_isSkinned )	// Skin�� ������ ZSkinnedMesh�� ����
			{
				switch( sm )	// ��Ű�� ����� ���� ��ü����
				{
					case SKINMETHOD_SW :
						pNode = (ZNode*)(new ZSWSkinnedMesh( pDev, *it ));
						break;
					case SKINMETHOD_FF :
						pNode = (ZNode*)(new ZFFSkinnedMesh( pDev, *it ));
						break;
					case SKINMETHOD_VS :
						pNode = (ZNode*)(new ZFFSkinnedMesh( pDev, *it ));
						break;
				}
				((ZSkinnedMesh*)pNode)->SetMatrixPalette( &m_matPalette );
			}
			else	// Skin�� ������ ZRigidMesh�� ����
				pNode = (ZNode*)(new ZRigidMesh( pDev, *it ));
		}
		else	// �̰͵� ���͵� �ƴϸ� �׳� ZNode�� ����
			pNode = new ZNode( pDev, *it );

		pNode->GetBBox( &vMin, &vMax );
		if( D3DXVec3Length( &vMinBox ) < D3DXVec3Length( &vMin ) ) vMinBox = vMin;
		if( D3DXVec3Length( &vMaxBox ) < D3DXVec3Length( &vMax ) ) vMaxBox = vMax;

		m_nodes.push_back( pNode );
	}
	m_bboxMinOrig = vMinBox;
	m_bboxMaxOrig = vMaxBox;
	m_fRadius = ( D3DXVec3Length( &vMinBox ) > D3DXVec3Length( &vMaxBox ) ) ? D3DXVec3Length( &vMinBox ) : D3DXVec3Length( &vMaxBox );
}

ZNodeMgr::~ZNodeMgr()
{
	stl_wipe_vector( m_matPalette );
	stl_wipe_vector( m_materials );
	for( int i = 0 ; i < m_nodes.size() ; i++ ) S_DEL( m_nodes[i] );
	stl_wipe_vector( m_nodes );
}

int ZNodeMgr::Animate( float fFrame )
{
	int				i;
	int				id;
	int				size;
	D3DXMATRIXA16	m;
	D3DXMATRIXA16*	pTM;

	if( fFrame > m_info.fAnimationEnd ) fFrame = m_info.fAnimationEnd;
	if( fFrame < m_info.fAnimationStart ) fFrame = m_info.fAnimationStart;

	// ���� ����(bone)���� �ִϸ��̼� ����� ����� �д�
	size = m_info.BoneTable.size();
	for( i = 0 ; i < size ; i++ )
	{
		id =  m_nodes[m_info.BoneTable[i]]->GetParentID();
		pTM = m_nodes[id]->GetMatrixTM();
		m_nodes[m_info.BoneTable[i]]->Animate( fFrame, pTM );
		pTM = ((ZBone*)m_nodes[m_info.BoneTable[i]])->GetMatrixSkin();
		// ��Ű���� ���� ��Ų����� ��Ʈ���� �ȷ�Ʈ�� ���
		m_matPalette[i] = *((D3DXMATRIX*)pTM); 
	}

	// ���밡 �ƴ� �͵��� �ִϸ��̼� ����� �����
	size = m_info.MeshTable.size();
	for( i = 0 ; i < size ; i++ )
	{
		id =  m_nodes[m_info.MeshTable[i]]->GetParentID();
		pTM = m_nodes[id]->GetMatrixTM();
		m_nodes[m_info.MeshTable[i]]->Animate( fFrame, pTM );
	}

	return 1;
}

int ZNodeMgr::Draw( int nNodeType )
{
	int		i;
	int		size;


	if( nNodeType == ZNode::NODE_MESH )
	{
		size = m_info.MeshTable.size();
		for( i = 0 ; i < size ; i++ )
		{
			m_pDev->SetMaterial( &m_materials[m_nodes[m_info.MeshTable[i]]->GetMaterialID()].material );


			int nID = m_nodes[m_info.MeshTable[i]]->GetMaterialID();
			LPDIRECT3DTEXTURE9 pTex = m_materials[nID].pTex[0];
			m_pDev->SetTexture( 0, pTex ); 
			m_pDev->SetTransform( D3DTS_WORLD, &m_matTM );

			std::wstring strName = m_info.strObjectNameTable[m_info.MeshTable[i]];
			if(strName.find(L"dummy")) {
				m_nodes[m_info.MeshTable[i]]->Draw( &m_matTM );			
			}
		}
	}
	else
	if( nNodeType == ZNode::NODE_BONE )
	{
		size = m_info.BoneTable.size();
		for( i = 0 ; i < size ; i++ )
		{
			m_pDev->SetMaterial( &m_materials[m_nodes[m_info.BoneTable[i]]->GetMaterialID()].material );
			m_pDev->SetTexture( 0, m_materials[m_nodes[m_info.BoneTable[i]]->GetMaterialID()].pTex[0] );
			m_pDev->SetTransform( D3DTS_WORLD, &m_matTM );
			m_nodes[m_info.BoneTable[i]]->Draw( &m_matTM );
		}
	}
	else	// NODE_NODE
	{
		size = m_nodes.size();
		for( i = 0 ; i < size ; i++ )
		{
			m_pDev->SetMaterial( &m_materials[m_nodes[i]->GetMaterialID()].material );
			m_pDev->SetTexture( 0, m_materials[m_nodes[i]->GetMaterialID()].pTex[0] ); 
			m_pDev->SetTransform( D3DTS_WORLD, &m_matTM );
			m_nodes[i]->Draw( &m_matTM );
		}
	}

	return 1;
}

int ZNodeMgr::DrawBBox( int nNodeType )
{
	int		i;
	int		size;

	m_pDev->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	size = m_nodes.size();
	for( i = 0 ; i < size ; i++ )
	{
		m_pDev->SetTransform( D3DTS_WORLD, &m_matTM );
		if( nNodeType == ZNode::NODE_NODE ) m_nodes[i]->DrawBBox( &m_matTM );
		else if( nNodeType == ZNode::NODE_BONE ) { if( m_nodes[i]->GetNodeType() == ZNode::NODE_BONE ) m_nodes[i]->DrawBBox( &m_matTM ); }
		else if( nNodeType == ZNode::NODE_MESH ) { if( m_nodes[i]->GetNodeType() == ZNode::NODE_MESH ) m_nodes[i]->DrawBBox( &m_matTM ); }
	}
	m_pDev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	return 1;
}

D3DXMATRIXA16* ZNodeMgr::GetNodeMat(std::wstring& strName)
{
	for(unsigned int i=0;i<m_info.strObjectNameTable.size();++i)
	{
		if(m_info.strObjectNameTable[i] == strName)
		{
			return m_nodes[i]->GetMatrixTM();
		}
	}

	return NULL;
}

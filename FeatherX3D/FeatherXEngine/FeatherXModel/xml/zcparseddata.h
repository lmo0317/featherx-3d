#pragma once

/// XML������ �Ľ��� ��� Ŭ����
class ZCParsedData
{
public:
	Info					m_info;				/// �� ���Ͽ� ���� �������� ����
	vector<Material>		m_materialTable;	/// �����迭
	list<ZCMesh*>			m_meshList;			/// �޽�(���) ����Ʈ
public:
	ZCParsedData() {}
	~ZCParsedData() 
	{ 
		stl_wipe_vector( m_materialTable );
		stl_wipe( m_meshList );
	}
};



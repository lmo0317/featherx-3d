========================================================================
    ���� ���̺귯�� : Tinyxml ������Ʈ ����
========================================================================

ex)
	TiXmlDocument doc( wstrName.c_str() );
	doc.SetCondenseWhiteSpace( false );
	if ( !doc.LoadFile() )
		return;

	const TiXmlElement* pElem = doc.FirstChildElement( "Name" );

/////////////////////////////////////////////////////////////////////////////
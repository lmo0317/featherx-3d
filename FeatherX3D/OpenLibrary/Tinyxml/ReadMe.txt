========================================================================
    정적 라이브러리 : Tinyxml 프로젝트 개요
========================================================================

ex)
	TiXmlDocument doc( wstrName.c_str() );
	doc.SetCondenseWhiteSpace( false );
	if ( !doc.LoadFile() )
		return;

	const TiXmlElement* pElem = doc.FirstChildElement( "Name" );

/////////////////////////////////////////////////////////////////////////////
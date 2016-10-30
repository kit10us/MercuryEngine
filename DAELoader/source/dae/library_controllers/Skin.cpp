#include <dae/library_controllers/Skin.h>

using namespace dae;

Skin::Skin( IDocument & document, const qxml::Element * node )
	: DocumentNode( document, node )
	, m_source( node->GetAttributeElse( "source", std::string() ) )
{
	/*
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->HasElements( "convex_mesh" ) )
		{
			throw Exception_NotSupported( "geometry.convex_mesh" );
		}
		else if ( childNode->IsTagName( "mesh" ) )
		{
			m_meshSource = MeshSource::Mesh;
			m_mesh.reset( new Mesh( GetDocument(), childNode ) );
		}
		else if ( childNode->HasElements( "spline" ) )
		{
			throw Exception_NotSupported( "geometry.spline" );
		}
		if ( childNode->HasElements( "brep" ) )
		{
			throw Exception_NotSupported( "geometry.brep" );
		}
	}
	*/
}

std::string Skin::GetSource() const
{
	return m_source;
}

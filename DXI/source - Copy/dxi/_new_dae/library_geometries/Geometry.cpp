#include <dxi/dae/library_geometries/DAE_Geometry.h>

using namespace dxi;
using namespace dae;

Geometry::Geometry( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetStringAttributeElse( "id", std::string() ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
{
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
}

const std::string & Geometry::GetID() const
{
	return m_id;
}

const std::string & Geometry::GetName() const
{
	return m_name;
}

Geometry::MeshSource::TYPE Geometry::GetMeshSource() const
{
	return m_meshSource;
}

const Mesh & Geometry::GetMesh() const
{
	return *m_mesh;
}

void Geometry::Build( PrimitiveList & pl, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const
{
	GetMesh().Build( pl, matrix, technique );
}

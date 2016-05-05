#include <dae/library_visual_scenes/VisualScene.h>
#include <dae/library_geometries/LibraryGeometries.h>
#include <dae/library_nodes/LibraryNodes.h>
#include <dae/library_visual_scenes/InstanceGeometry.h>
#include <dae/library_materials/Material.h>

using namespace dxi;
using namespace dae;

VisualScene::VisualScene( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetStringAttributeElse( "id", std::string() ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "node" ) )
		{
			if ( ! m_firstNode )
			{
				m_firstNode = std::shared_ptr< Node >( new Node( document, 0, childNode ) );
			}
			else
			{
				m_firstNode->Append( std::shared_ptr< Node >( new Node( document, 0, childNode ) ) );
			}
		}
	}
}

const std::string & VisualScene::GetID() const
{
	return m_id;
}

const std::string & VisualScene::GetName() const
{
	return m_name;
}

const Node * VisualScene::GetFirstNode() const
{
	return m_firstNode.get();
}

void VisualScene::Build( PrimitiveList & pl ) const
{
	Build( pl, unify::Matrix::MatrixIdentity(), GetFirstNode() );
	int x(0);x;
}

void VisualScene::Build( PrimitiveList & pl, const unify::Matrix & matrix, const dae::Node * node ) const
{
	// Iterate this nodes instances...
	for ( std::vector< std::shared_ptr< InstanceSet > >::const_iterator itr = node->GetInstances().begin(), end = node->GetInstances().end(); itr != end; ++itr )
	{
		const InstanceSet * instance = itr->get();
		switch( instance->GetType() )
		{
		case InstanceSet::InstanceTypeGeometry:
			{
				const InstanceGeometry * instanceGeometry = dynamic_cast< const InstanceGeometry * >( instance );
				const dae::Geometry & geo = *GetDocument().GetLibraryGeometries().Find( instance->GetURL() ) ;
				geo.Build( pl, matrix * node->GetFinalMatrix(), instanceGeometry->GetBindMaterial()->GetTechniqueCommon() );
			}
			break;
		case InstanceSet::InstanceTypeNode:
			{
				const dae::Node * referencedNode = GetDocument().GetLibraryNodes().Find( instance->GetURL() );
				Build( pl, matrix * node->GetFinalMatrix(), referencedNode );
			}
			break;
		}
	}

	// Iterate children nodes...
	if ( node->GetFirstChild() )
	{
		Build( pl, matrix, node->GetFirstChild() );
	}

	// Iterate next sibling...
	if ( node->GetNext() )
	{
		Build( pl, matrix, node->GetNext() );
	}
}


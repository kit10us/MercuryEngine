// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/library_visual_scenes/VisualScene.h>
#include <dae/library_geometries/LibraryGeometries.h>
#include <dae/library_controllers/LibraryControllers.h>
#include <dae/library_nodes/LibraryNodes.h>
#include <dae/library_visual_scenes/InstanceGeometry.h>
#include <dae/library_visual_scenes/InstanceController.h>
#include <dae/library_materials/Material.h>

using namespace dae;

VisualScene::VisualScene( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetAttributeElse( "id", std::string() ) )
, m_name( node->GetAttributeElse( "name", std::string() ) )
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

void VisualScene::Build( dxi::PrimitiveList & pl ) const
{
	Build( pl, unify::MatrixIdentity(), GetFirstNode() );
}

void VisualScene::Build( dxi::PrimitiveList & pl, const unify::Matrix & matrix, const dae::Node * node ) const
{
	OutputDebugStringA( node->GetID().c_str() );
	OutputDebugStringA( "\n" );

	// Iterate this nodes instances...
	for ( const auto instance : node->GetInstances() )
	{
		switch( instance->GetType() )
		{
		case InstanceSet::InstanceTypeGeometry:
			{
				const InstanceGeometry * instanceGeometry = dynamic_cast< const InstanceGeometry * >( instance.get() );
				const dae::Geometry & geo = *GetDocument().GetLibraryGeometries().Find( instance->GetURL() ) ;
				geo.Build( pl, matrix * node->GetFinalMatrix(), instanceGeometry->GetBindMaterial()->GetTechniqueCommon() );
			}
			break;
		case InstanceSet::InstanceTypeController:
			{
				const InstanceController * instanceController = dynamic_cast< const InstanceController * >(instance.get());
				const dae::Controller & controller = *GetDocument().GetLibraryControllers().Find( instanceController->GetURL() );
				const dae::Geometry & geo = *GetDocument().GetLibraryGeometries().Find( controller.GetSkin()->GetSource() );
				geo.Build( pl, matrix * node->GetFinalMatrix(), instanceController->GetBindMaterial()->GetTechniqueCommon() );
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


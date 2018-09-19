// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/object/component/BBoxRendererComponent.h>
#include <me/render/GeometryCache.h>
#include <me/render/Mesh.h>
#include <me/render/VertexUtil.h>
#include <me/object/Object.h>

using namespace me;
using namespace object;
using namespace component;
using namespace render;

BBoxRendererComponent::BBoxRendererComponent( BBoxRendererComponent & component )
	: ObjectComponent( component )
	, m_os{ component.m_os }
	, m_effect( component.m_effect )
	, m_padding( component.m_padding )
	, m_color( component.m_color )
	, m_componentsChecked{ 0 }
{
}

BBoxRendererComponent::BBoxRendererComponent( os::IOS * os, Effect::ptr effect, unify::Color color )
	: ObjectComponent( "BBoxRenderer", true, true )
	, m_os{ os }
	, m_effect{ effect }
	, m_padding{ 0.1f }
	, m_color{ color }
	, m_componentsChecked{ 0 }
{
}

BBoxRendererComponent::~BBoxRendererComponent()
{
}

float BBoxRendererComponent::GetPadding() const
{
	return m_padding;
}

void BBoxRendererComponent::SetPadding( float padding )
{
	m_padding = padding;
}
void BBoxRendererComponent::OnAttach( Object * object )
{
	ObjectComponent::OnAttach( object );
}

void BBoxRendererComponent::OnDetach( Object * object )
{
	ObjectComponent::OnDetach( object );

	m_geomertries.clear();
}

void BBoxRendererComponent::OnUpdate( const UpdateParams & params )
{
	// If there is a change in the object's components, check for new geometry...
	if( m_componentsChecked < GetObject()->GetComponentCount() )
	{
		// Determine bbox size...
		int components = GetObject()->GetComponentCount();
		for( int i = m_componentsChecked; i < components; ++i )
		{
			GeometryComponent * geometryComponent = GetObject()->GetComponentT< GeometryComponent >( i );
			if( geometryComponent != nullptr )
			{
				// Add geometry to list...
				m_geomertries.push_back( geometryComponent );

				unify::Matrix modelMatrix = geometryComponent->GetMatrix();

				for( auto & geo : m_geomertries )
				{
					unify::BBox< float > bbox;
					geo->GetBBox( bbox, unify::MatrixIdentity() );

					unify::V3< float > corners[8];
					bbox.GenerateCorners( corners );
					modelMatrix.TransformCoords( corners, 8 );
					m_bbox.AddPoints( corners, 8 );
				}
			}
		}
		m_componentsChecked = components;

		// Build BBox geometry...

		VertexDeclaration::ptr vd = m_effect->GetVertexShader()->GetVertexDeclaration();
		size_t lineSegmentCount = 12;
		size_t vertexCount = lineSegmentCount * 2;
		std::shared_ptr< char * > vertices( new char *[vd->GetSizeInBytes( 0 ) * vertexCount] );

		unsigned short stream = 0;
		VertexElement positionE = CommonVertexElement::Position( stream );
		VertexElement diffuseE = CommonVertexElement::Diffuse( stream );

		unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

		// All from inf...

		WriteVertex( *vd, lock, 0, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetInf().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 1, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetInf().y, m_bbox.GetInf().z ) );

		WriteVertex( *vd, lock, 2, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetInf().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 3, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetSup().y, m_bbox.GetInf().z ) );

		WriteVertex( *vd, lock, 4, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetInf().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 5, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetInf().y, m_bbox.GetSup().z ) );

		// All from sup...

		WriteVertex( *vd, lock, 6, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetSup().y, m_bbox.GetSup().z ) );
		WriteVertex( *vd, lock, 7, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetSup().y, m_bbox.GetSup().z ) );

		WriteVertex( *vd, lock, 8, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetSup().y, m_bbox.GetSup().z ) );
		WriteVertex( *vd, lock, 9, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetInf().y, m_bbox.GetSup().z ) );

		WriteVertex( *vd, lock, 10, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetSup().y, m_bbox.GetSup().z ) );
		WriteVertex( *vd, lock, 11, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetSup().y, m_bbox.GetInf().z ) );

		// Others...

		WriteVertex( *vd, lock, 12, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetInf().y, m_bbox.GetSup().z ) );
		WriteVertex( *vd, lock, 13, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetInf().y, m_bbox.GetSup().z ) );

		WriteVertex( *vd, lock, 14, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetInf().y, m_bbox.GetSup().z ) );
		WriteVertex( *vd, lock, 15, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetSup().y, m_bbox.GetSup().z ) );

		WriteVertex( *vd, lock, 16, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetSup().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 17, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetSup().y, m_bbox.GetSup().z ) );

		WriteVertex( *vd, lock, 18, positionE, unify::V3< float >( m_bbox.GetInf().x, m_bbox.GetSup().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 19, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetSup().y, m_bbox.GetInf().z ) );

		WriteVertex( *vd, lock, 20, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetInf().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 21, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetInf().y, m_bbox.GetSup().z ) );

		WriteVertex( *vd, lock, 22, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetInf().y, m_bbox.GetInf().z ) );
		WriteVertex( *vd, lock, 23, positionE, unify::V3< float >( m_bbox.GetSup().x, m_bbox.GetSup().y, m_bbox.GetInf().z ) );

		// Set all colors the same...
		for( size_t i = 0; i < vertexCount; ++i )
		{
			WriteVertex( *vd, lock, i, diffuseE, m_color );
		}

		Mesh * mesh = new Mesh( m_os->GetRenderer( 0 ) );

		BufferSet & set = mesh->GetPrimitiveList().AddBufferSet();
		set.ClearMethods();

		VertexBufferParameters vbParameters{ vd, { { vertexCount, vertices.get() } } };
		set.AddVertexBuffer( vbParameters );
		set.AddMethod( RenderMethod::CreateLineList( 0, lineSegmentCount, m_effect ) );

		m_geometry.reset( mesh );
		GetObject()->MakeDirty();
	}
}

void BBoxRendererComponent::CollectGeometry( GeometryCache & solids, GeometryCache & trans, const unify::FrameLite * frame )
{
	if ( !m_effect || !m_geometry ) return;

	solids.Add( m_geometry.get(), { frame, nullptr } );
}
					   
IObjectComponent::ptr BBoxRendererComponent::Duplicate()
{
	auto duplicate = new BBoxRendererComponent( *this );
	return IObjectComponent::ptr( duplicate );
}

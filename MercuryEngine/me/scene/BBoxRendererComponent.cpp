// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/BBoxRendererComponent.h>
#include <me/Mesh.h>
#include <me/VertexUtil.h>
#include <me/scene/Object.h>

using namespace me;
using namespace scene;

BBoxRendererComponent::BBoxRendererComponent( BBoxRendererComponent & component )
	: ObjectComponent( component )
	, m_effect( component.m_effect )
	, m_padding( component.m_padding )
	, m_color( component.m_color )
{
}

BBoxRendererComponent::BBoxRendererComponent( IOS * os, Effect::ptr effect, unify::Color color )
	: ObjectComponent( os )
	, m_effect( effect )
	, m_padding( 0.1f )
	, m_color( color )
{
}

BBoxRendererComponent::~BBoxRendererComponent()
{
}

std::string BBoxRendererComponent::GetName() const
{
	return "BBoxRenderer";
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
	for ( int i = 0; i < object->ComponentCount(); ++i )
	{
		GeometryComponent * geometryComponent = dynamic_cast< GeometryComponent * >(object->GetComponent( i ).get());
		if ( geometryComponent != nullptr )
		{
			m_geomertries.push_back( geometryComponent );
		}
	}
}

void BBoxRendererComponent::OnDetach()
{
	m_geomertries.clear();
}

void BBoxRendererComponent::OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo )
{
}

void BBoxRendererComponent::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
	if ( !m_effect || !m_geomertries.size() ) return;

	// TODO: Move to OnUpdate.

	unify::BBox< float > bbox; // TODO: Should probably create a new bbox that includes the transformed bbox's of the individual geos, for more acuracy (when needed - so far one geo seems normal).
	for( auto & geo : m_geomertries )
	{
		unify::V3< float > inf( geo->GetGeometry()->GetBBox().inf );
		unify::V3< float > sup( geo->GetGeometry()->GetBBox().sup );

		bbox += ( inf - unify::V3< float >( m_padding, m_padding, m_padding ) );
		bbox += ( sup + unify::V3< float >( m_padding, m_padding, m_padding ) );
	}

	VertexDeclaration::ptr vd = m_effect->GetVertexShader()->GetVertexDeclaration();
	size_t lineSegmentCount = 12;
	size_t vertexCount = lineSegmentCount * 2;
	std::shared_ptr< char * > vertices( new char *[ vd->GetSizeInBytes( 0 ) * vertexCount ] );

	unsigned short stream = 0;
	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

	// All from inf...

	WriteVertex( *vd, lock, 0, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 1, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.inf.z ) );

	WriteVertex( *vd, lock, 2, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 3, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.inf.z ) );

	WriteVertex( *vd, lock, 4, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 5, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.sup.z ) );

	// All from sup...

	WriteVertex( *vd, lock, 6, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.sup.z ) );
	WriteVertex( *vd, lock, 7, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.sup.z ) );

	WriteVertex( *vd, lock, 8, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.sup.z ) );
	WriteVertex( *vd, lock, 9, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.sup.z ) );

	WriteVertex( *vd, lock, 10, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.sup.z ) );
	WriteVertex( *vd, lock, 11, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.inf.z ) );

	// Others...

	WriteVertex( *vd, lock, 12, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.sup.z ) );
	WriteVertex( *vd, lock, 13, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.sup.z ) );

	WriteVertex( *vd, lock, 14, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.sup.z ) );
	WriteVertex( *vd, lock, 15, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.sup.z ) );

	WriteVertex( *vd, lock, 16, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 17, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.sup.z ) );

	WriteVertex( *vd, lock, 18, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 19, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.inf.z ) );

	WriteVertex( *vd, lock, 20, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 21, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.sup.z ) );

	WriteVertex( *vd, lock, 22, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.inf.z ) );
	WriteVertex( *vd, lock, 23, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.inf.z ) );

	// Set all colors the same...
	for( size_t i = 0; i < vertexCount; ++i )
	{
		WriteVertex( *vd, lock, i, diffuseE, m_color );
	}

	Mesh * mesh = new Mesh( GetOS()->GetRenderer(0) );

	BufferSet & set = mesh->GetPrimitiveList().AddBufferSet();
	set.ClearMethods();

	VertexBufferParameters vbParameters{ vd, { { vertexCount, vertices.get() } } };
	set.AddVertexBuffer( vbParameters );
	set.AddMethod( RenderMethod::CreateLineList( 0, lineSegmentCount, m_effect ) );	 

	m_geometry.reset( mesh );
	cache.Add( m_geometry.get(), frame );
}
					   
IObjectComponent * BBoxRendererComponent::Duplicate()
{
	auto duplicate = new BBoxRendererComponent( *this );
	return duplicate;
}

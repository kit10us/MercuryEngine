// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/BBoxRendererComponent.h>
#include <dxi/scene/Object.h>

using namespace dxi;
using namespace scene;

BBoxRendererComponent::BBoxRendererComponent( BBoxRendererComponent & component )
	: ObjectComponent( component )
	, m_pl( component.GetOS()->GetRenderer( 0 ) )
	, m_effect( component.m_effect )
	, m_padding( component.m_padding )
	, m_color( component.m_color )
{
	m_pl.AddBufferSet();
}

BBoxRendererComponent::BBoxRendererComponent( core::IOS * os, Effect::ptr effect, unify::Color color )
	: ObjectComponent( os )
	, m_pl( os->GetRenderer(0) )
	, m_effect( effect )
	, m_padding( 0.1f )
	, m_color( color )
{
	m_pl.AddBufferSet();
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

void BBoxRendererComponent::OnDetach( Object * object )
{
	m_geomertries.clear();
}

void BBoxRendererComponent::OnUpdate( Object * object, const RenderInfo & renderInfo )
{
}

void BBoxRendererComponent::OnRender( Object * object, const RenderInfo & renderInfo )
{
	if ( !m_effect || !m_geomertries.size() ) return;

	RenderInfo myRenderInfo( renderInfo );
		
	unify::BBox< float > bbox; // TODO: Should probably create a new bbox that includes the transformed bbox's of the individual geos, for more acuracy (when needed - so far one geo seems normal).
	for( auto & geo : m_geomertries )
	{
		myRenderInfo.SetWorldMatrix( geo->GetModelMatrix() * renderInfo.GetWorldMatrix() );
		bbox += ( geo->GetGeometry()->GetBBox().inf - unify::V3< float >( m_padding, m_padding, m_padding ) );
		bbox += ( geo->GetGeometry()->GetBBox().sup + unify::V3< float >( m_padding, m_padding, m_padding ) );
	}

	BufferSet & bs = m_pl.GetBufferSet( 0 );
	bs.GetRenderMethodBuffer().Clear();

	VertexDeclaration::ptr vd = m_effect->GetVertexShader()->GetVertexDeclaration();
	size_t lineSegmentCount = 12;
	size_t vertexCount = lineSegmentCount * 2;
	std::shared_ptr< char * > vertices( new char *[ vd->GetSize() * vertexCount ] );

	unsigned short stream = 0;
	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	
	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

	// All from inf...

	vd->WriteVertex( lock, 0, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 1, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.inf.z ) );

	vd->WriteVertex( lock, 2, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 3, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.inf.z ) );

	vd->WriteVertex( lock, 4, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 5, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.sup.z ) );

	// All from sup...

	vd->WriteVertex( lock, 6, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.sup.z ) );
	vd->WriteVertex( lock, 7, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.sup.z ) );

	vd->WriteVertex( lock, 8, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.sup.z ) );
	vd->WriteVertex( lock, 9, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.sup.z ) );

	vd->WriteVertex( lock, 10, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.sup.z ) );
	vd->WriteVertex( lock, 11, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.inf.z ) );

	// Others...

	vd->WriteVertex( lock, 12, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.sup.z ) );
	vd->WriteVertex( lock, 13, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.sup.z ) );

	vd->WriteVertex( lock, 14, positionE, unify::V3< float >( bbox.inf.x, bbox.inf.y, bbox.sup.z ) );
	vd->WriteVertex( lock, 15, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.sup.z ) );

	vd->WriteVertex( lock, 16, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 17, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.sup.z ) );

	vd->WriteVertex( lock, 18, positionE, unify::V3< float >( bbox.inf.x, bbox.sup.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 19, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.inf.z ) );

	vd->WriteVertex( lock, 20, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 21, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.sup.z ) );

	vd->WriteVertex( lock, 22, positionE, unify::V3< float >( bbox.sup.x, bbox.inf.y, bbox.inf.z ) );
	vd->WriteVertex( lock, 23, positionE, unify::V3< float >( bbox.sup.x, bbox.sup.y, bbox.inf.z ) );

	// Set all colors the same...
	for( size_t i = 0; i < vertexCount; ++i )
	{
		vd->WriteVertex( lock, i, diffuseE, m_color );
	}

	bs.GetVertexBuffer().Create( vertexCount, vd, vertices.get() );
	bs.GetRenderMethodBuffer().AddMethod( RenderMethod::CreateLineList( 0, lineSegmentCount, m_effect ) );
	m_pl.Render( myRenderInfo );
}


IObjectComponent * BBoxRendererComponent::Duplicate()
{
	auto duplicate = new BBoxRendererComponent( *this );
	return duplicate;
}

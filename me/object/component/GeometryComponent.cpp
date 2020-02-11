// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/object/component/GeometryComponent.h>
#include <me/object/Object.h>
#include <me/render/GeometryCache.h>
#include <me/interop/MyThing.h>

using namespace me;
using namespace object;
using namespace component;
using namespace render;

GeometryComponent::GeometryComponent( GeometryComponent & component )
	: ObjectComponent( component )
	, m_geometry( component.m_geometry )
	, m_geometryInstanceData( component.m_geometry->CreateInstanceData() )
	, m_visible{ true }
{
	{
		me::interop::Getter< const me::object::component::IObjectComponent * > geometryNameGetter =
			[&]( const me::object::component::IObjectComponent * component ) -> std::string {
			return dynamic_cast< const GeometryComponent * >(component)->GetGeometry()->GetName();
		};

		GetLookup()->Add( "geometryName", interop::IValue::ptr{
			new interop::MyThing< me::object::component::IObjectComponent * >( this, geometryNameGetter ) 
		} ); 
	}

	{
		me::interop::Getter< const me::object::component::IObjectComponent * > geometrySourceGetter =
			[&]( const me::object::component::IObjectComponent * component ) -> std::string {
			return dynamic_cast< const GeometryComponent * >(component)->GetGeometry()->GetSource();
		};

		GetLookup()->Add( "geometrySource", interop::IValue::ptr{
			new interop::MyThing< me::object::component::IObjectComponent * >( this, geometrySourceGetter ) 
		} );
	}
}

GeometryComponent::GeometryComponent()
	: ObjectComponent( "Geometry", false, true )
	, m_visible{ true }
{
	{
		me::interop::Getter< const me::object::component::IObjectComponent * > geometryNameGetter =
			[&]( const me::object::component::IObjectComponent * component ) -> std::string {
			return dynamic_cast< const GeometryComponent * >(component)->GetGeometry()->GetName();
		};

		GetLookup()->Add( "geometryName", interop::IValue::ptr{
			new interop::MyThing< GeometryComponent * >( this, geometryNameGetter )
		} );
	}

	{
		me::interop::Getter< const me::object::component::IObjectComponent * > geometrySourceGetter =
			[&]( const me::object::component::IObjectComponent * component ) -> std::string {
			return dynamic_cast< const GeometryComponent * >(component)->GetGeometry()->GetSource();
		};

		GetLookup()->Add( "geometrySource", interop::IValue::ptr{
			new interop::MyThing< me::object::component::IObjectComponent * >( this, geometrySourceGetter )
		} );
	}
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry )
	: GeometryComponent()
{
	m_geometry = geometry;
	m_geometryInstanceData = me::render::GeometryInstanceData::ptr( geometry->CreateInstanceData() );
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry, unify::Matrix modelMatrix )
	: GeometryComponent( geometry )
{
	m_geometryInstanceData = render::GeometryInstanceData::ptr( geometry->CreateInstanceData() );
	m_geometryInstanceData->SetMatrix( modelMatrix );
}

GeometryComponent::~GeometryComponent()
{
}

void GeometryComponent::SetGeometry( Geometry::ptr geometry )
{
	m_geometry = geometry;
	m_geometryInstanceData = geometry->CreateInstanceData();
}

Geometry::ptr GeometryComponent::GetGeometry()
{
	return m_geometry;
}

const Geometry::ptr GeometryComponent::GetGeometry() const
{
	return m_geometry;
}

void GeometryComponent::SetMatrix( unify::Matrix matrix )
{
	m_geometryInstanceData->SetMatrix( matrix );
}

unify::Matrix GeometryComponent::GetMatrix() const
{
	return *m_geometryInstanceData->GetMatrix();
}

void GeometryComponent::OnStart()
{
}

void GeometryComponent::OnUpdate( const UpdateParams & params )
{
	m_geometry->Update( params, m_geometryInstanceData.get() );
}

void GeometryComponent::CollectGeometry( GeometryCache & solids, GeometryCache & trans, const unify::FrameLite * frame )
{
	if( m_geometry->IsTrans() )
	{
		trans.Add( m_geometry.get(), me::render::FrameAndMatrix{ frame, m_geometryInstanceData->GetMatrix() } );
	}
	else
	{
		solids.Add( m_geometry.get(), me::render::FrameAndMatrix{ frame, m_geometryInstanceData->GetMatrix() } );
	}
}

void GeometryComponent::GetBBox( unify::BBox< float > & bbox, const unify::Matrix & matrix ) const
{
	if( !m_geometry )
	{
		return;
	}

	bbox += m_geometry->GetBBox();
	matrix.TransformBBox( bbox );
}

IObjectComponent::ptr GeometryComponent::Duplicate()
{
	auto duplicate = new GeometryComponent( *this );
	return IObjectComponent::ptr( duplicate );
}

namespace me
{
	GeometryComponent * AddGeometryComponent( Object * object, Geometry::ptr geometry, unify::Matrix matrix )
	{
		GeometryComponent * component = new GeometryComponent( geometry, matrix );
		object->AddComponent( IObjectComponent::ptr( component ) );
		return component;
	}
}

// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/object/component/GeometryComponent.h>
#include <me/object/Object.h>
#include <me/render/GeometryCache.h>

using namespace me;
using namespace object;
using namespace component;
using namespace render;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "visible", 1 }
	};

	std::vector< std::string > g_ValuesList
	{
		{ "visible" }
	};
}

GeometryComponent::GeometryComponent( GeometryComponent & component )
	: ObjectComponent( component )
	, m_geometry( component.m_geometry )
	, m_geometryInstanceData( component.m_geometry->CreateInstanceData() )
	, m_visible{ true }
{
}

GeometryComponent::GeometryComponent()
	: ObjectComponent( "Geometry", false, true )
	, m_visible{ true }
{
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry )
	: ObjectComponent( "Geometry", false, true )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
	, m_visible{ true }
{
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry, unify::Matrix modelMatrix )
	: ObjectComponent( "Geometry", false, true )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
	, m_visible{ true }
{
	m_geometryInstanceData->SetMatrix( modelMatrix );
}

GeometryComponent::~GeometryComponent()
{
}

void GeometryComponent::SetGeometry( Geometry::ptr geometry )
{
	m_geometry = geometry;
	m_geometryInstanceData.reset( geometry->CreateInstanceData() );
}

void GeometryComponent::SetVisible( bool visible )
{
	m_visible = visible;
}

bool GeometryComponent::GetVisible() const
{
	return m_visible;
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


int GeometryComponent::GetValueCount() const
{
	return ObjectComponent::GetValueCount() + g_ValuesList.size();
}

bool GeometryComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return true;
	}

	return ObjectComponent::ValueExists( name );
}

std::string GeometryComponent::GetValueName( int index ) const
{
	if( index < ObjectComponent::GetValueCount() )
	{
		return ObjectComponent::GetValueName( index );
	}

	int myIndex = index - ObjectComponent::GetValueCount();
	if( myIndex < (int)g_ValuesList.size() )
	{
		return g_ValuesList[myIndex];
	}

	return std::string();
}

int GeometryComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}
	return ObjectComponent::FindValueIndex( name );
}

bool GeometryComponent::SetValue( int index, std::string value )
{
	if( index <= ObjectComponent::GetValueCount() )
	{
		return ObjectComponent::SetValue( index, value );
	}

	int myIndex = index - ObjectComponent::GetValueCount();
	switch( myIndex )
	{
	case 0:
		m_visible = unify::Cast< bool >( value );
		return true;

	default:
		return false;
	}
}

std::string GeometryComponent::GetValue( int index ) const
{
	if( index <= ObjectComponent::GetValueCount() )
	{
		return ObjectComponent::GetValue( index );
	}

	int myIndex = index - ObjectComponent::GetValueCount();
	switch( myIndex )
	{
	case 0:
		return unify::Cast< std::string >( m_visible );

	default:
		return std::string();
	}
}

bool GeometryComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string GeometryComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
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

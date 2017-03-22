// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/GeometryComponent.h>
#include <me/object/Object.h>
#include <me/GeometryCache.h>

using namespace me;
using namespace object;

namespace
{
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		/*
		{ "origin", 0 },
		{ "orbit", 1 },
		{ "angleASecond", 2 }
		*/
	};
	std::vector< std::string > g_ValuesList
	{
		/*
		{ "origin" },
		{ "orbit" },
		{ "angleASecond" }
		*/
	};
}


GeometryComponent::GeometryComponent( GeometryComponent & component )
	: ObjectComponent( component )
	, m_geometry( component.m_geometry )
	, m_geometryInstanceData( component.m_geometry->CreateInstanceData() )
{
}

GeometryComponent::GeometryComponent()
	: ObjectComponent( "Geometry", false, true )
{
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry )
	: ObjectComponent( "Geometry", false, true )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
{
}

GeometryComponent::~GeometryComponent()
{
}

void GeometryComponent::SetGeometry( Geometry::ptr geometry )
{
	m_geometry = geometry;
	m_geometryInstanceData.reset( geometry->CreateInstanceData() );
}

Geometry::ptr GeometryComponent::GetGeometry()
{
	return m_geometry;
}

const Geometry::ptr GeometryComponent::GetGeometry() const
{
	return m_geometry;
}

void GeometryComponent::OnStart()
{
}

void GeometryComponent::OnUpdate( UpdateParams params )
{
	m_geometry->Update( params, m_geometryInstanceData.get() );
}

void GeometryComponent::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
	cache.Add( m_geometry.get(), frame );
}

void GeometryComponent::OnSuspend()
{
}

void GeometryComponent::OnResume()
{
}

IObjectComponent::ptr GeometryComponent::Duplicate()
{
	auto duplicate = new GeometryComponent( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}

GeometryComponent * me::AddGeometryComponent( Object * object, Geometry::ptr geometry )
{
	GeometryComponent * component = new GeometryComponent( geometry );
	object->AddComponent( IObjectComponent::ptr( component ) );
	return component;
}

int GeometryComponent::GetValueCount() const
{
	return object::ObjectComponent::GetValueCount() + g_ValuesList.size();
}

bool GeometryComponent::ValueExists( std::string name ) const
{
	if( object::ObjectComponent::ValueExists( name ) )
	{
		return true;
	}

	auto && itr = g_ValuesMap.find( name );
	if( itr == g_ValuesMap.end() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::string GeometryComponent::GetValueName( int index ) const
{
	if( index >= GetValueCount() )
	{
		return std::string();
	}

	int baseValueCount = object::ObjectComponent::GetValueCount();
	if( index < baseValueCount )
	{
		return object::ObjectComponent::GetValueName( index );
	}
	else
	{
		return g_ValuesList[index - baseValueCount];
	}
}

int GeometryComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr == g_ValuesMap.end() )
	{
		return object::ObjectComponent::FindValueIndex( name );
	}
	else
	{
		return itr->second + object::ObjectComponent::GetValueCount();
	}
}

bool GeometryComponent::SetValue( int index, std::string value )
{
	int baseValueCount = object::ObjectComponent::GetValueCount();
	if( index < baseValueCount )
	{
		return object::ObjectComponent::SetValue( index, value );
	}
	/*
	switch( index - baseValueCount )
	{
	case 0:
		m_origin = unify::V3< float >( value );
		return true;
	case 1:
		m_orbit = unify::V3< float >( value );
		return true;
	case 2:
		m_angleASecond = unify::Angle( value );
		return true;
	}
	*/

	return false;
}

bool GeometryComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string GeometryComponent::GetValue( int index ) const
{
	int baseValueCount = object::ObjectComponent::GetValueCount();
	if( index < baseValueCount )
	{
		return object::ObjectComponent::GetValue( index );
	}
	/*
	switch( index - baseValueCount )
	{
	case 0:
		return m_origin.ToString();
	case 1:
		return m_orbit.ToString();
	case 2:
		return m_angleASecond.ToString( false );
	}
	*/

	return std::string();
}

std::string GeometryComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}

std::string GeometryComponent::GetWhat() const
{
	return std::string();
}

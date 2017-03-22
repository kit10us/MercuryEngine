// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/ObjectComponent.h>

using namespace me;
using namespace object;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "typename", 0 },
		{ "enabled", 1 },
		{ "update", 2 },
		{ "render", 3 }
	};

	std::vector< std::string > g_ValuesList
	{
		{ "typename" },
		{ "enabled" },
		{ "update" },
		{ "render" }
	};
}

Lookup * ObjectComponent::GetLookup()
{
	return &m_values;
}

const Lookup * ObjectComponent::GetLookup() const
{
	return &m_values;
}

ObjectComponent::ObjectComponent( const ObjectComponent & component )
	: m_typeName( component.m_typeName )
	, m_values( component.m_values )
	, m_object{ nullptr }
	, m_enabled{ true }
	, m_update{ component.m_update }
	, m_render{ component.m_render }
{
	AddInterface( "IComponent", this );
	AddInterface( "IObjectComponent", this );
	AddInterface( "ObjectComponent", this );
	AddInterface( m_typeName, this );
}

ObjectComponent::ObjectComponent( std::string typeName, bool update, bool render )
	: m_typeName{ typeName }
	, m_object{ nullptr }
	, m_enabled{ true }
	, m_update{ update }
	, m_render{ render }
{
	AddInterface( "IComponent", this );
	AddInterface( "IObjectComponent", this );
	AddInterface( "ObjectComponent", this );
	AddInterface( typeName, this );
}

ObjectComponent::~ObjectComponent()
{
}

Object* ObjectComponent::GetObject()
{
	return m_object;
}

const Object* ObjectComponent::GetObject() const
{
	return m_object;
}

void ObjectComponent::AddInterface( std::string name, me::IThing* ptr )
{
	m_interfaceMap[ name ] = ptr;
}

bool ObjectComponent::Updateable() const 
{ 
	return m_update; 
}

bool ObjectComponent::Renderable() const 
{
	return m_render; 
}

void ObjectComponent::OnAttach( Object * object )
{
	m_object = object;
}

void ObjectComponent::OnDetach( Object * object ) 
{
	m_object = nullptr;
}

void ObjectComponent::OnStart() 
{
}

void ObjectComponent::OnUpdate( UpdateParams params ) 
{
}

void ObjectComponent::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) 
{
}

void ObjectComponent::OnSuspend() 
{
}

void ObjectComponent::OnResume() 
{
}

bool ObjectComponent::IsEnabled() const
{
	return m_enabled;
}

void ObjectComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

int ObjectComponent::GetValueCount() const
{
	return (int)g_ValuesList.size() + m_values.Count();
}

bool ObjectComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return m_values.Exists( name );
}

std::string ObjectComponent::GetValueName( int index ) const
{
	if ( index < (int)g_ValuesList.size() )
	{
		return g_ValuesList[ index ];
	}
	return m_values.GetName( index - (int)g_ValuesList.size() );
}

int ObjectComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}
	return m_values.Find( name ) + g_ValuesMap.size();
}

bool ObjectComponent::SetValue( int index, std::string value )
{
	switch ( index )
	{
	case 0:
		return false;
	case 1:
		m_enabled = unify::Cast< bool >( value );
		return true;
	case 2:
		m_update = unify::Cast< bool >( value );
		return true;
	case 3:
		m_render = unify::Cast< bool >( value );
		return true;
	default:
		return m_values.SetValue( index - g_ValuesMap.size(), value );
	}
}

bool ObjectComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ObjectComponent::GetValue( int index ) const
{
	switch ( index )
	{
	case 0:
		return m_typeName;
	case 1:
		return unify::Cast< std::string >( m_enabled );
	case 2:
		return unify::Cast< std::string >( m_update );
	case 3:
		return unify::Cast< std::string >( m_render );
	default:
		return m_values.GetValue( index - g_ValuesMap.size() );
	}
}
 
std::string ObjectComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}

me::IThing* ObjectComponent::QueryInterface( std::string name )
{
	auto itr = m_interfaceMap.find( name );
	if ( itr == m_interfaceMap.end() )
	{
		return nullptr;
	}

	return itr->second;
}

std::string ObjectComponent::GetTypeName() const
{
	return m_typeName;
}


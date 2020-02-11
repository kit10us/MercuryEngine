// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/object/component/ObjectComponent.h>
#include <me/interop/ReferenceCast.h>

using namespace me;
using namespace object;
using namespace component;

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

	GetLookup()->Add( "typename", interop::IValue::ptr{ new interop::ReferenceCast< std::string >( m_typeName, false, true ) } );
	GetLookup()->Add( "alias", interop::IValue::ptr{ new interop::ReferenceCast< std::string >( m_alias ) } );
	GetLookup()->Add( "enabled", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_enabled ) } );
	GetLookup()->Add( "update", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_update ) } );
	GetLookup()->Add( "render", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_render ) } );
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

	GetLookup()->Add( "typename", interop::IValue::ptr{ new interop::ReferenceCast< std::string >( m_typeName, false, true ) } );
	GetLookup()->Add( "alias", interop::IValue::ptr{ new interop::ReferenceCast< std::string >( m_alias ) } );
	GetLookup()->Add( "enabled", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_enabled ) } );
	GetLookup()->Add( "update", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_update ) } );
	GetLookup()->Add( "render", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_render ) } );
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::SetAlias( std::string alias )
{
	m_alias = alias;
}

std::string ObjectComponent::GetAlias() const
{
	return m_alias;
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
	m_interfaceMap[name] = ptr;
}

void ObjectComponent::GetBBox( unify::BBox< float > & bbox, const unify::Matrix & matrix ) const
{
	// DO NOTHING BY DEFAULT.
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
	// STUBBED
}

void ObjectComponent::OnUpdate( const UpdateParams & params )
{
	// STUBBED
}

void ObjectComponent::CollectGeometry( render::GeometryCache & solids, render::GeometryCache & trans, const unify::FrameLite * frame )
{
	// STUBBED
}

void ObjectComponent::OnSuspend()
{
	// STUBBED
}

void ObjectComponent::OnResume()
{
	// STUBBED
}

bool ObjectComponent::IsEnabled() const
{
	return m_enabled;
}

void ObjectComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

interop::Interop * ObjectComponent::GetLookup()
{
	return &m_values;
}

const interop::Interop * ObjectComponent::GetLookup() const
{
	return &m_values;
}

me::IThing* ObjectComponent::QueryInterface( std::string name )
{
	auto itr = m_interfaceMap.find( name );
	if (itr == m_interfaceMap.end())
	{
		return nullptr;
	}

	return itr->second;
}

std::string ObjectComponent::GetTypeName() const
{
	return m_typeName;
}

std::string ObjectComponent::GetWhat() const
{
	return std::string();
}


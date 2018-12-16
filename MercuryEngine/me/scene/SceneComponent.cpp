// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/SceneComponent.h>

using namespace me;
using namespace scene;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		// Name, Index into value.
		{ "typename", 0 },
		{ "enabled", 1 },
	};

	std::vector< std::string > g_ValuesList
	{
		{ "typename" },
		{ "enabled" },
	};
}

SceneComponent::SceneComponent( os::IOS * os, std::string typeName )
	: m_os{ os }
	, m_typeName{ typeName }
	, m_enabled{ true }
	, m_scene{ nullptr }
{
	AddInterface( "IComponent", this );
	AddInterface( "ISceneComponent", this );
	AddInterface( "SceneComponent", this );
	AddInterface( typeName, this );
}

SceneComponent::~SceneComponent()
{
}

os::IOS * SceneComponent::GetOS()
{
	return m_os;
}

const os::IOS * SceneComponent::GetOS() const
{
	return m_os;
}

 IScene * SceneComponent::GetScene()
{
	return m_scene;
}

const IScene * SceneComponent::GetScene() const
{
	return m_scene;
}

void SceneComponent::AddInterface( std::string name, IThing* ptr )
{
	m_interfaceMap[ name ] = ptr;
}

void SceneComponent::OnAttach( IScene * scene )
{
	m_scene = scene;
}

void SceneComponent::OnDetach( IScene * scene ) 
{
	m_scene = nullptr;
}

void SceneComponent::OnBeforeStart() 
{
}

void SceneComponent::OnAfterStart() 
{
}

void SceneComponent::OnEarlyUpdate( const UpdateParams & params )
{
}

void SceneComponent::OnUpdate( const UpdateParams & params ) 
{
}

void SceneComponent::OnLateUpdate( const UpdateParams & params )
{
}

void SceneComponent::CollectCameras( RenderGirl & renderGirl )
{
}

void SceneComponent::OnRender( RenderGirl & renderGirl ) 
{
}

void SceneComponent::OnSuspend() 
{
}

void SceneComponent::OnResume()
{
}

void SceneComponent::OnEnd()
{
}

bool SceneComponent::IsEnabled() const
{
	return m_enabled;
}

void SceneComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

int SceneComponent::GetValueCount() const
{
	return (int)g_ValuesList.size() + m_values.Count();
}

bool SceneComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return m_values.Exists( name );
}

std::string SceneComponent::GetValueName( int index ) const
{
	if ( index < (int)g_ValuesList.size() )
	{
		return g_ValuesList[ index ];
	}
	return m_values.GetName( index - (int)g_ValuesList.size() );
}

int SceneComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}
	return m_values.Find( name ) + g_ValuesMap.size();
}

bool SceneComponent::SetValue( int index, std::string value )
{
	switch ( index )
	{
	case 0:
		return false;
	case 1:
		m_enabled = unify::Cast< bool >( value );
		return true;
	default:
		m_values.SetValue( index - g_ValuesMap.size(), value );
		return true;
	}
}

bool SceneComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string SceneComponent::GetValue( int index ) const
{
	switch ( index )
	{
	case 0:
		return m_typeName;
	case 1:
		return unify::Cast< std::string >( m_enabled );
	default:
		return m_values.GetValue( index - g_ValuesMap.size() );
	}
}
 
std::string SceneComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}

Interop * SceneComponent::GetLookup()
{
	return &m_values;
}

const Interop * SceneComponent::GetLookup() const
{
	return &m_values;
}

me::IThing* SceneComponent::QueryInterface( std::string name )
{
	auto itr = m_interfaceMap.find( name );
	if ( itr == m_interfaceMap.end() )
	{
		return nullptr;
	}

	return itr->second;
}

std::string SceneComponent::GetTypeName() const
{
	return m_typeName;
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}

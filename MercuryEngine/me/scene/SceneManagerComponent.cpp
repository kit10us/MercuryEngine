// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/SceneManagerComponent.h>
#include <me/interop/ReferenceCast.h>

using namespace me;
using namespace scene;

SceneManagerComponent::SceneManagerComponent( std::string typeName )
	: m_typeName{ typeName }
	, m_enabled{ true }
{
	AddInterface("IComponent", this);
	AddInterface("ISceneComponent", this);
	AddInterface("SceneComponent", this);

	GetLookup()->Add( "typename", interop::IValue::ptr{ new interop::ReferenceCast< std::string >( m_typeName, false, true ) } );
	GetLookup()->Add( "enabled", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_enabled ) } );
}

void SceneManagerComponent::AddInterface(std::string name, me::IThing* ptr)
{
	m_interfaceMap[name] = ptr;
}

void SceneManagerComponent::OnAttach( SceneManager * sceneManager )
{
}

void SceneManagerComponent::OnDetach( SceneManager * sceneManager )
{
}

void SceneManagerComponent::OnSceneStart( IScene * scene )
{
}

void SceneManagerComponent::OnSceneEnd( IScene * from )
{
}

bool SceneManagerComponent::IsEnabled() const
{
	return m_enabled;
}

void SceneManagerComponent::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

interop::Interop * SceneManagerComponent::GetLookup()
{
	return &m_values;
}

const interop::Interop * SceneManagerComponent::GetLookup() const
{
	return &m_values;
}

me::IThing* SceneManagerComponent::QueryInterface(std::string name)
{
	auto itr = m_interfaceMap.find(name);
	if (itr == m_interfaceMap.end())
	{
		return nullptr;
	}

	return itr->second;
}

std::string SceneManagerComponent::GetTypeName() const
{
	return m_typeName;
}

std::string SceneManagerComponent::GetWhat() const
{
	return std::string();
}

// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/SceneManagerComponent.h>

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

SceneManagerComponent::SceneManagerComponent( std::string typeName )
	: m_typeName{ typeName }
	, m_enabled{ true }
{
	AddInterface("IComponent", this);
	AddInterface("ISceneComponent", this);
	AddInterface("SceneComponent", this);
	AddInterface(typeName, this);
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

int SceneManagerComponent::GetValueCount() const
{
	return (int)g_ValuesList.size() + m_values.Count();
}

bool SceneManagerComponent::ValueExists(std::string name) const
{
	auto && itr = g_ValuesMap.find(name);
	if (itr != g_ValuesMap.end())
	{
		return true;
	}
	return m_values.Exists(name);
}

std::string SceneManagerComponent::GetValueName(int index) const
{
	if (index < (int)g_ValuesList.size())
	{
		return g_ValuesList[index];
	}
	return m_values.GetName(index - (int)g_ValuesList.size());
}

int SceneManagerComponent::FindValueIndex(std::string name) const
{
	auto && itr = g_ValuesMap.find(name);
	if (itr != g_ValuesMap.end())
	{
		return itr->second;
	}
	return m_values.Find(name) + g_ValuesMap.size();
}

bool SceneManagerComponent::SetValue(int index, std::string value)
{
	switch (index)
	{
	case 0:
		return false;
	case 1:
		m_enabled = unify::Cast< bool >(value);
		return true;
	default:
		m_values.SetValue(index - g_ValuesMap.size(), value);
		return true;
	}
}

std::string SceneManagerComponent::GetValue(int index) const
{
	switch (index)
	{
	case 0:
		return m_typeName;
	case 1:
		return unify::Cast< std::string >(m_enabled);
	default:
		return m_values.GetValue(index - g_ValuesMap.size());
	}
}

bool SceneManagerComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string SceneManagerComponent::GetValue(std::string name) const
{
	int index = FindValueIndex(name);
	return GetValue(index);
}

Interop * SceneManagerComponent::GetLookup()
{
	return &m_values;
}

const Interop * SceneManagerComponent::GetLookup() const
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

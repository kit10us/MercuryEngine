// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/AutoSceneManagerComponent.h>
#include <melua/component/SceneComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>

using namespace melua;
using namespace component;
using namespace me;
using namespace scene;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "autoPath", 0 },
		{ "onceBeforeStartScript", 1 },
	};

	std::vector< std::string > g_ValuesList
	{
		{ "autoPath" },
		{ "onceBeforeStartScript" },
	};
}

AutoSceneManagerComponent::AutoSceneManagerComponent( melua::ScriptEngine * scriptEngine, unify::Path autoPath, unify::Path onceBeforeStartScript )
	: me::scene::SceneManagerComponent( "LuaAutoSceneComponent" )
	, m_scriptEngine{ scriptEngine }
	, m_autoPath{ autoPath }
	, m_onceBeforeStartScript{ onceBeforeStartScript }
	, m_once{ false }
{
}

AutoSceneManagerComponent::~AutoSceneManagerComponent()
{
}

void AutoSceneManagerComponent::OnAttach( SceneManager * sceneManager )
{
}

void AutoSceneManagerComponent::OnDetach( SceneManager * sceneManager )
{
}

void AutoSceneManagerComponent::OnSceneStart( IScene * scene )
{
	if (!m_once )
	{
		if (!m_onceBeforeStartScript.Empty())
		{
			m_scriptEngine->GetGame()->Debug()->LogLine("Loading startup script \"" + m_onceBeforeStartScript.ToString() + "\".", 1);
			m_scriptEngine->ExecuteFile(m_onceBeforeStartScript);
		}
		else
		{
			m_scriptEngine->GetGame()->Debug()->LogLine("Unable to find startup script \"" + m_onceBeforeStartScript.ToString() + "\".", 1);
		}
		m_once = true;
	}

	std::string name = scene->GetName();
	unify::Path scriptPath( m_autoPath + unify::Path(name + ".lua"));
	if ( scriptPath.Exists() )
	{
		auto sceneComponent = m_scriptEngine->LoadSceneScript( scriptPath );
		scene->AddComponent( sceneComponent );
	}
}

void AutoSceneManagerComponent::OnSceneEnd( IScene * scene )
{
}

int AutoSceneManagerComponent::GetValueCount() const
{
	return SceneManagerComponent::GetValueCount() + (int)g_ValuesList.size();
}

bool AutoSceneManagerComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return SceneManagerComponent::ValueExists( name );
}

std::string AutoSceneManagerComponent::GetValueName( int index ) const
{
	if( index < SceneManagerComponent::GetValueCount() )
	{
		return SceneManagerComponent::GetValueName( index );
	}
	
	int localIndex = index - SceneManagerComponent::GetValueCount();
	if( localIndex < (int)g_ValuesList.size() )
	{
		return g_ValuesList[ localIndex ];
	}

	return std::string();
}

int AutoSceneManagerComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}

	return SceneManagerComponent::FindValueIndex( name );
}

bool AutoSceneManagerComponent::SetValue( int index, std::string value )
{
	if( index < SceneManagerComponent::GetValueCount() )
	{
		return SceneManagerComponent::SetValue( index, value );
	}

	int localIndex = index - SceneManagerComponent::GetValueCount();
	switch( localIndex )
	{
	case 0:
		m_autoPath = unify::Path( value );
		return true;

	case 1:
		m_onceBeforeStartScript = unify::Path( value );
		return true;

	default:
		return false;
	}
}

std::string AutoSceneManagerComponent::GetValue( int index ) const
{
	if( index < SceneManagerComponent::GetValueCount() )
	{
		return SceneManagerComponent::GetValue( index );
	}

	int localIndex = index - SceneManagerComponent::GetValueCount();
	switch( localIndex )
	{
	case 0:
		return m_autoPath.ToString();

	case 1:
		return m_onceBeforeStartScript.ToString();

	default:
		return std::string();
	}
}

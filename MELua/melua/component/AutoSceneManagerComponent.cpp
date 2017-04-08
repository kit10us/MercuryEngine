// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/AutoSceneManagerComponent.h>
#include <melua/component/SceneComponent.h>
#include <melua/ExportGame.h>
#include <melua/CreateState.h>

using namespace melua;
using namespace component;
using namespace me;
using namespace scene;

AutoSceneManagerComponent::AutoSceneManagerComponent( melua::ScriptEngine * scriptEngine, unify::Path autoPath, unify::Path onceBeforeStart)
	: me::scene::SceneManagerComponent( "LuaAutoSceneComponent" )
	, m_scriptEngine{ scriptEngine }
	, m_autoPath{ autoPath }
	, m_onceBeforeStart{ onceBeforeStart }
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
		if (!m_onceBeforeStart.Empty())
		{
			m_scriptEngine->GetGame()->LogLine("Loading startup script \"" + m_onceBeforeStart.ToString() + "\".");
			m_scriptEngine->ExecuteFile(m_onceBeforeStart);
		}
		else
		{
			m_scriptEngine->GetGame()->LogLine("Unable to find startup script \"" + m_onceBeforeStart.ToString() + "\".");
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

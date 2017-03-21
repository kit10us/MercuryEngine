// Copyright (c) 2002 - 2011, Quentin S. Smith
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

AutoSceneManagerComponent::AutoSceneManagerComponent( melua::ScriptEngine * scriptEngine, unify::Path autoPath )
	: me::scene::SceneManagerComponent()
	, m_scriptEngine{ scriptEngine }
	, m_autoPath{ autoPath }
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
	std::string name = scene->GetName();
	unify::Path scriptPath = m_autoPath + (name + ".lua");
	if ( scriptPath.Exists() )
	{
		auto sceneComponent = m_scriptEngine->LoadSceneScript( scriptPath );
		scene->AddComponent( sceneComponent );
	}
}

void AutoSceneManagerComponent::OnSceneEnd( IScene * scene )
{
}

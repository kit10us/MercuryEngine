// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/AutoSceneManagerComponent.h>
#include <melua/component/SceneComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>
#include <me/interop/MyThing.h>
#include <me/debug/Block.h>

using namespace melua;
using namespace component;
using namespace me;
using namespace scene;

AutoSceneManagerComponent::AutoSceneManagerComponent( melua::ScriptEngine * scriptEngine, unify::Path autoPath, unify::Path onceBeforeStartScript )
	: me::scene::component::SceneManagerComponent( "LuaAutoSceneManagerComponent" )
	, m_scriptEngine{ scriptEngine }
	, m_autoPath{ autoPath }
	, m_onceBeforeStartScript{ onceBeforeStartScript }
	, m_onceBeforeStart{ false }
{
	GetLookup()->Add( "autoPath",
		me::interop::IValue::ptr{ new interop::MyThing< AutoSceneManagerComponent * >(
			this,
			[&]( const AutoSceneManagerComponent * component ) -> std::string { return component->GetAutoPath().ToString(); },
			[&]( AutoSceneManagerComponent* component, std::string value ){ component->SetAutoPath( unify::Path( value ) ); } 
			)
	} );

	GetLookup()->Add( "onceBeforeStartScript",
		me::interop::IValue::ptr{ new interop::MyThing< AutoSceneManagerComponent * >(
			this,
			[&]( const AutoSceneManagerComponent * component ) -> std::string { return component->GetOnceBeforeStartScript().ToString(); },
			[&]( AutoSceneManagerComponent* component, std::string value ) { component->SetOnceBeforeStartScript( unify::Path( value ) ); }
			)
	} );

	GetLookup()->Add( "onceBeforeStart",
		me::interop::IValue::ptr{ new interop::MyThing< AutoSceneManagerComponent * >(
			this,
			[&]( const AutoSceneManagerComponent * component ) -> std::string { return unify::Cast< std::string >( component->GetOnceBeforeStart() ); }
			)
	} );
}

AutoSceneManagerComponent::~AutoSceneManagerComponent()
{
}

unify::Path AutoSceneManagerComponent::GetAutoPath() const
{
	return m_autoPath;
}

void AutoSceneManagerComponent::SetAutoPath( unify::Path autoPath )
{
	m_autoPath = autoPath;
}

bool AutoSceneManagerComponent::GetOnceBeforeStart() const
{
	return m_onceBeforeStart;
}

unify::Path AutoSceneManagerComponent::GetOnceBeforeStartScript() const
{
	return m_onceBeforeStartScript;
}

void AutoSceneManagerComponent::SetOnceBeforeStartScript( unify::Path onceBeforeStartScript )
{
	m_onceBeforeStartScript = onceBeforeStartScript;
}

void AutoSceneManagerComponent::OnAttach( SceneManager * sceneManager )
{
}

void AutoSceneManagerComponent::OnDetach( SceneManager * sceneManager )
{
}

void AutoSceneManagerComponent::OnSceneStart( IScene * scene )
{
	debug::Block block{ m_scriptEngine->GetGame()->Debug(), "AutoSceneManagerComponent::OnSceneStart" };
	if (!m_onceBeforeStart)
	{
		if (!m_onceBeforeStartScript.Empty())
		{
			block.LogLine( "Loading startup script \"" + m_onceBeforeStartScript.ToString() + "\"." );
			m_scriptEngine->ExecuteFile(m_onceBeforeStartScript);
		}
		else
		{
			block.LogLine( "Unable to find startup script \"" + m_onceBeforeStartScript.ToString() + "\"." );
		}
		m_onceBeforeStart = true;
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
// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <chrono>
#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>

#include <qxml/Document.h>

using namespace me;
using namespace scene;


const char* SceneManager::Name()
{
	return "SceneManager";
}

SceneManager::SceneManager()
	: GameComponent( Name() )
	, m_currentScene{ nullptr }
	, m_updateTick{ 0 }
	, m_renderTick{ 0 }
	, m_renderCount{ 0 }
{
}

SceneManager::~SceneManager()
{
    Destroy();
}

void SceneManager::Destroy()
{
    m_scenes.Clear();
}

unify::Result<> SceneManager::OnAttach( game::IGame* gameInstance )
{
	GameComponent::OnAttach( gameInstance );
	m_block = gameInstance->Debug()->GetLogger()->CreateBlock( "SceneManager" );
	return {};
}


size_t SceneManager::GetSceneCount() const
{
	return m_scenes.Count();
}

void SceneManager::AddScene( IScene::ptr scene )
{
	if( m_scenes.Exists( scene->GetName() ) )
	{
		throw exception::FailedToCreate( "Attempted to add scene \"" + scene->GetName() + "\", but it already exists!" );
	}

	m_scenes.Add( scene->GetName(), scene );
}

size_t SceneManager::FindSceneIndex( std::string name )
{
	return m_scenes.Find( name );
}
													  
std::string SceneManager::GetSceneName(size_t index)
{
	return m_scenes.GetName( index );
}

IScene* SceneManager::GetCurrentScene()
{
	return m_currentScene.get();
}

std::string SceneManager::GetPreviousSceneName()
{
	return m_previousSceneName;
}

unify::Result<> SceneManager::ChangeScene( std::string name )
{
	auto debug = GetGame()->Debug();
	IScene::ptr newScene = m_scenes.GetValue( name );

	// Leave current scene...
	if ( m_currentScene )
	{
		{
			auto result = m_currentScene->OnEnd();
			if (!result)
			{
				return result;
			}
		}
		{
			auto result = m_currentScene->Component_OnEnd();
			if (!result)
			{
				return result;
			}
		}

		// Let all components mess with the scene before we destroy it...
		for ( auto component : m_components )
		{
			auto result = component->OnSceneEnd( m_currentScene.get() );
			if (!result)
			{
				debug->ReportError(me::debug::ErrorLevel::Critical, "Component \"" + component->GetWhat() + "\" failed to on OnSceneEnd!");
				return unify::Failure{"Component \"" + component->GetWhat() + "\" failed to on OnSceneEnd!"};
			}
		}

		m_previousSceneName = m_currentScene->GetName();

		m_currentScene.reset();
	}

	// Create new scene...
	m_currentScene = newScene;

	// Let all components mess with the scene first...
	for ( auto component : m_components )
	{
		auto result = component->OnSceneStart( m_currentScene.get() );
		if (!result)
		{
			debug->ReportError(me::debug::ErrorLevel::Critical, "Component \"" + component->GetWhat() + "\" failed on OnSceneStart!");
			return unify::Failure{"Component \"" + component->GetWhat() + "\" failed on OnSceneStart!"};
		}
	}


	{
		auto result = m_currentScene->Component_BeforeOnStart();
		if (!result)
		{
			return result;
		}
	}

	{
		debug->GetLogger()->Log( "Scene \"" + m_currentScene->GetName() + "\" OnStart begin" );
		auto result = m_currentScene->OnStart();
		if (!result)
		{
			return result;
		}
		debug->GetLogger()->Log( "Scene \"" + m_currentScene->GetName() + "\" OnStart end" );
	}

	{
		auto result = m_currentScene->Component_AfterOnStart();
		if (!result)
		{
			return result;
		}
	}

	return {};
}

unify::Result<> SceneManager::RestartScene()
{
	return ChangeScene(m_currentScene->GetName());
}

int SceneManager::GetComponentCount() const
{
	return (int)m_components.size();
}

unify::Result<> SceneManager::AddComponent( component::ISceneManagerComponent::ptr component )
{
	auto result = component->OnAttach(this);
	if (!result)
	{
		return result;
	}
	m_components.push_back(component);

	return {};
}

unify::Result<> SceneManager::RemoveComponent( component::ISceneManagerComponent::ptr component )
{
	m_components.remove( component );
	auto result = component->OnDetach( this );
	if (!result)
	{
		return result;
	}
	return {};
}

component::ISceneManagerComponent* SceneManager::GetComponent(size_t index)
{
	if (index > m_components.size()) return nullptr;

	size_t i = 0;
	for (auto component : m_components)
	{
		if (index == i) return component.get();
		++i;
	}

	return nullptr;
}

component::ISceneManagerComponent* SceneManager::GetComponent(std::string name)
{
	int index = FindComponent(name);
	if (index == -1) return nullptr;
	return GetComponent(index);
}

int SceneManager::FindComponent(std::string typeName) const
{
	int i = 0;
	for (auto component : m_components)
	{
		if (unify::String::StringIs(component->GetTypeName(), typeName)) return i;
		++i;
	}
	return -1;
}

size_t SceneManager::GetRenderCount() const
{
	return m_renderCount;
}

unify::Result<> SceneManager::EarlyOnUpdate( const UpdateParams & params )
{
	if( IsEnabled() == false || !m_currentScene )
	{
		return {}; // Not a failure.
	}

	auto result = m_currentScene->Component_BeforeOnUpdate( params );
	if (!result)
	{
		return result;
	}

	return {};
}

unify::Result<> SceneManager::OnUpdate( const UpdateParams & params ) 
{
	if ( IsEnabled() == false || ! m_currentScene )
	{
		return {}; // Not a failure.
	}

	{
		auto result = m_currentScene->Component_OnUpdate( params );
		if (!result)
		{
			return result;
		}
	}
	{
		auto result = m_currentScene->OnUpdate( params );
		if (!result)
		{
			return result;
		}
	}

	return {};
}

unify::Result<> SceneManager::LateOnUpdate( const UpdateParams & params )
{
	if( IsEnabled() == false || !m_currentScene )
	{
		return {}; // Not a failure.
	}

	auto result = m_currentScene->Component_AfterOnUpdate( params );
	if (!result)
	{
		return result;
	}

	return {};
}

unify::Result<> SceneManager::OnRender( const render::Params & params )
{
	auto debug = GetGame()->Debug();
	debug->DebugTimeStampBegin( "Render" );
	
	if ( IsEnabled() == false || !m_currentScene )
	{
		return {}; // Not a failure.
	}

	RenderGirl renderGirl;
	renderGirl.Begin( &params );

	{
		auto result = m_currentScene->Component_OnRender( renderGirl );
		if (!result)
		{
			return result;
		}
	}

	{
		auto result = m_currentScene->OnRender( renderGirl );
		if (!result)
		{
			return result;
		}
	}

	m_renderCount = renderGirl.End();

	debug->DebugTimeStampEnd( "Render" );
	return {};
}

std::string SceneManager::SendCommand( size_t id, std::string extra )
{
	if( m_currentScene )
	{
		return m_currentScene->SendCommand( id, extra );
	}
	return std::string();
}

std::string SceneManager::GetWhat() const
{
	return std::string();
}

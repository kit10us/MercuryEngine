// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <me/scene/DefaultSceneFactory.h>
#include <qxml/Document.h>

using namespace me;
using namespace scene;


char* SceneManager::Name()
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
    m_scenes.clear();
}

size_t SceneManager::GetSceneCount() const
{
	return m_scenes.size();
}

void SceneManager::AddScene( std::string name, ISceneFactory::ptr sceneFactory )
{
	auto itr = m_scenes.find( name );
	if ( itr != m_scenes.end() )
	{
		throw exception::FailedToCreate( "Attempted to add scene \"" + name + "\", but it already exists!" );
	}

	m_scenes[ name ] = m_sceneList.size();
	m_sceneList.push_back( sceneFactory );

	if ( ! m_currentScene )
	{
		ChangeScene( name );
	}
}

 void SceneManager::AddScene( std::string name )
{
	auto sceneFactory = new DefaultSceneFactory( name );
	AddScene( name, ISceneFactory::ptr( sceneFactory ) );	
}

int SceneManager::FindSceneIndex( std::string name )
{
	auto itr = m_scenes.find( name );
	return itr == m_scenes.end() ? -1 : itr->second;
}
													  
std::string SceneManager::GetSceneName( int index )
{
	if ( index >= (int)m_sceneList.size() ) return std::string();
	auto sceneFactory = m_sceneList[index];
	return sceneFactory->GetName();
}

IScene* SceneManager::GetCurrentScene()
{
	return m_currentScene.get();
}

std::string SceneManager::GetPreviousSceneName()
{
	return m_previousSceneName;
}

bool SceneManager::ChangeScene( std::string name )
{
	// Find scene factory...
	int index = FindSceneIndex(name);
	ISceneFactory::ptr sceneFactory = m_sceneList[ index ];
	if ( ! sceneFactory )
	{
		return false;
	}
	
	// Leave current scene...
	if ( m_currentScene )
	{
		m_currentScene->OnEnd();
		m_currentScene->Component_OnEnd();

		// Let all components mess with the scene before we destroy it...
		for (auto component : m_components)
		{
			component->OnSceneEnd( m_currentScene.get());
		}

		m_previousSceneName = m_currentScene->GetName();

		m_currentScene.reset();
	}
	
	// Create new scene...
	m_currentScene = sceneFactory->Produce( dynamic_cast< me::game::Game* >( m_game ) );

	// Let all components mess with the scene first...
	for (auto component : m_components)
	{
		component->OnSceneStart( m_currentScene.get() );
	}

	GetGame()->Debug()->LogLine( "Starting scene \"" + m_currentScene->GetName() + "\" Begin", 0 );
	m_currentScene->Component_OnBeforeStart();	

	GetGame()->Debug()->LogLine( "Scene OnStart Begin", 1 );
	m_currentScene->OnStart();
	GetGame()->Debug()->LogLine( "Scene OnStart End", 1 );

	m_currentScene->Component_OnAfterStart();
	GetGame()->Debug()->LogLine( "Start scene \"" + m_currentScene->GetName() + "\" Done", 1 );

	return true;
}

void SceneManager::RestartScene()
{
	ChangeScene(m_currentScene->GetName());
}

int SceneManager::GetComponentCount() const
{
	return (int)m_components.size();
}

void SceneManager::AddComponent(ISceneManagerComponent::ptr component)
{
	component->OnAttach(this);
	m_components.push_back(component);
}

void SceneManager::RemoveComponent(ISceneManagerComponent::ptr component)
{
	m_components.remove(component);
	component->OnDetach(this);
}

ISceneManagerComponent* SceneManager::GetComponent(int index)
{
	if (index > (int)m_components.size()) return nullptr;

	int i = 0;
	for (auto component : m_components)
	{
		if (index == i) return component.get();
		++i;
	}

	return nullptr;
}

ISceneManagerComponent* SceneManager::GetComponent(std::string name)
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
		if (unify::StringIs(component->GetTypeName(), typeName)) return i;
		++i;
	}
	return -1;
}


size_t SceneManager::GetRenderCount() const
{
	return m_renderCount;
}

void SceneManager::OnEarlyUpdate( const UpdateParams & params )
{
	if( m_enabled == false || !m_currentScene )
	{
		return;
	}

	m_currentScene->Component_OnEarlyUpdate( params );
}

void SceneManager::OnUpdate( const UpdateParams & params ) 
{
	if ( m_enabled == false || ! m_currentScene )
	{
		return;
	}

	m_currentScene->Component_OnUpdate( params );
	m_currentScene->OnUpdate( params );
}

void SceneManager::OnLateUpdate( const UpdateParams & params )
{
	if( m_enabled == false || !m_currentScene )
	{
		return;
	}

	m_currentScene->Component_OnLateUpdate( params );
}

void SceneManager::OnRender( const render::Params & params )
{
	if ( m_enabled == false )
	{
		return;
	}

	if ( !m_currentScene )
	{
		return;
	}

	RenderGirl renderGirl;
	renderGirl.Begin( &params );

	m_currentScene->Component_OnRender( renderGirl );
	m_currentScene->OnRender( renderGirl );

	m_renderCount = renderGirl.End();
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

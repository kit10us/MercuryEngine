// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <me/debug/Block.h>
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
    m_scenes.Clear();
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

int SceneManager::FindSceneIndex( std::string name )
{
	return m_scenes.Find( name );
}
													  
std::string SceneManager::GetSceneName( int index )
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

bool SceneManager::ChangeScene( std::string name )
{
	debug::Block block( GetGame()->Debug(), "Scene::ChangeScene" );

	IScene::ptr newScene = m_scenes.GetValue( name );
	
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
	m_currentScene = newScene;

	// Let all components mess with the scene first...
	for (auto component : m_components)
	{
		component->OnSceneStart( m_currentScene.get() );
	}

	block.LogLine( "Starting scene \"" + m_currentScene->GetName() + "\" Begin" );
	m_currentScene->Component_OnBeforeStart();	

	block.LogLine( "OnStart Begin" );
	m_currentScene->OnStart();

	block.LogLine( "OnStart End" );

	m_currentScene->Component_OnAfterStart();
	block.LogLine( "Start scene \"" + m_currentScene->GetName() + "\" Done" );

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

void SceneManager::AddComponent( component::ISceneManagerComponent::ptr component )
{
	component->OnAttach(this);
	m_components.push_back(component);
}

void SceneManager::RemoveComponent( component::ISceneManagerComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

component::ISceneManagerComponent* SceneManager::GetComponent(int index)
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
		if (unify::string::StringIs(component->GetTypeName(), typeName)) return i;
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
	auto * debug = GetGame()->GetOS()->Debug();
	debug->DebugTimeStampBegin( "Render" );
	
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

	debug->DebugTimeStampEnd( "Render" );
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

// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;
using namespace scene;

SceneManager::SceneManager()
	: GameComponent( "SceneManager" )
	, m_game{ nullptr }
	, m_currentScene{ nullptr }
	, m_previousScene{ nullptr }
	, m_updateTick{ 0 }
	, m_renderTick{ 0 }
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

void SceneManager::AddScene( std::string name, IScene::ptr scene )
{
	auto itr = m_scenes.find( name );
	if ( itr != m_scenes.end() )
	{
		throw exception::FailedToCreate( "Attempted to add scene \"" + name + "\", but it already exists!" );
	}

	m_scenes[ name ] = scene;
	m_sceneList.push_back( scene );

	for ( auto component : m_componentList )
	{
		component->OnSceneAdded( scene.get() );
	}										   

	if ( ! m_currentScene )
	{
		ChangeScene( name );
	}
}

IScene* SceneManager::AddScene( std::string name )
{
	Scene* scene = new Scene( dynamic_cast<me::Game*>(GetGame()), name );
	AddScene( name, IScene::ptr( scene ) );	
	return scene;
}

IScene* SceneManager::FindScene( std::string name )
{
	auto itr = m_scenes.find( name );
	return itr == m_scenes.end() ? nullptr : itr->second.get();
}
													  
IScene* SceneManager::GetScene( size_t index )
{
	if ( index >= m_sceneList.size() ) return nullptr;
	return m_sceneList[ index ].get();
}

IScene* SceneManager::GetCurrentCurrent()
{
	return m_currentScene;
}

IScene* SceneManager::GetPrevious()
{
	return m_previousScene;
}

bool SceneManager::ChangeScene( std::string name )
{
	IScene* scene = FindScene( name );
	if ( ! scene )
	{
		return false;
	}
	
	if ( m_currentScene )
	{
		m_currentScene->LeaveScene( scene );
	}

	m_previousScene = m_currentScene;
	
	m_currentScene = scene;

	if ( m_currentScene )
	{
		m_currentScene->EnterScene( m_previousScene );
	}				

	scene->Start();

	for ( auto component : m_componentList )
	{
		component->OnSceneChange( m_previousScene, m_currentScene );
	}

	return true;
}

void SceneManager::AddComponent( ISceneManagerComponent::ptr component )
{
	m_componentList.push_back( component );
}

void SceneManager::OnUpdate( UpdateParams params )
{
	if ( m_enabled == false )
	{
		return;
	}

	if ( ! m_currentScene )
	{
		return;
	}

	m_currentScene->Update( params );
}

void SceneManager::OnRender( RenderParams params )
{
	if ( m_enabled == false )
	{
		return;
	}

	if ( !m_currentScene )
	{
		return;
	}

	m_currentScene->Render( params );
}

std::string SceneManager::GetWhat() const
{
	return std::string();
}

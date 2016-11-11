// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneManager.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;
using namespace scene;

SceneManager::SceneManager( IGame * game )
: m_game( game )
, m_focusScene( 0 )
, m_enabled( true )
{
}

SceneManager::~SceneManager()
{
    Destroy();
}

std::string SceneManager::GetName() const
{
	return "SceneManager";
}

IGame * SceneManager::GetGame()
{
	return m_game;
}

const IGame * SceneManager::GetGame() const
{
	return m_game;
}

void SceneManager::Destroy()
{
    m_scenes.clear();
}

void SceneManager::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool SceneManager::GetEnabled() const
{
	return m_enabled;
}

Scene::ptr SceneManager::Add( std::string name )
{
    Scene::ptr scene( new Scene( GetGame() ) );

	for ( auto & component : m_components )
	{
		component->OnNewScene( scene.get() );
	}

	m_scenes[ name ] = scene;
    return scene;
}

Scene::ptr SceneManager::Find( std::string name )
{
	std::map< std::string, Scene::ptr >::iterator itr = m_scenes.find( name );
	return itr == m_scenes.end() ? Scene::ptr() : itr->second;
}

void SceneManager::OnAttach( IGame * game )
{
}

void SceneManager::OnDetach( IGame * game )
{
}

void SceneManager::OnUpdate( IGame * game, const RenderInfo & renderInfo )
{
	if ( m_enabled == false )
	{
		return;
	}

    std::list< Scene * > sceneList;
    for( std::map< std::string, Scene::ptr >::iterator itr = m_scenes.begin(), end = m_scenes.end(); itr != end; ++itr )
    {
        Scene * scene = (*itr).second.get();
        if ( scene->GetEnabled() )
        {
            sceneList.push_back( scene );
        }
    }
 
    // Update all scenes...
    for ( std::list< Scene * >::iterator itr = sceneList.begin(), end = sceneList.end(); itr != end; ++itr )
    {
        Scene * scene = (*itr);
        scene->Update( renderInfo );
    }
}

void SceneManager::OnRender( IGame * game, const RenderInfo & renderInfo )
{
	if ( m_enabled == false )
	{
		return;
	}

    // Build a list of visible scenes in order of GetOrder...

    class SortSceneList
    {
    public:
        bool operator()( Scene * a, Scene * b ) const
        {
            return a->GetOrder() < b->GetOrder();
        }
    };

    std::list< Scene * > sceneList;

    for( std::map< std::string, Scene::ptr >::iterator itr = m_scenes.begin(), end = m_scenes.end(); itr != end; ++itr )
	{
        Scene * scene = (*itr).second.get();
        if ( scene->GetEnabled() )
        {
            sceneList.push_back( scene );
        }
	}

    sceneList.sort( SortSceneList() );

    for ( std::list< Scene * >::iterator itr = sceneList.begin(), end = sceneList.end(); itr != end; ++itr )
    {
        Scene * scene = (*itr);
        scene->Render( renderInfo );
    }
}

int SceneManager::ComponentCount() const
{
	return (int)m_components.size();
}

void SceneManager::AddComponent( ISceneManagerComponent::ptr component )
{
	m_components.push_back( component );
}

void SceneManager::RemoveComponent( ISceneManagerComponent::ptr component )
{
}

ISceneManagerComponent::ptr SceneManager::GetComponent( int index )
{
	if ( index > (int)m_components.size() ) return ISceneManagerComponent::ptr();

	int i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component;
		++i;
	}

	assert( 0 );
	return ISceneManagerComponent::ptr(); // Should never hit here.
}

ISceneManagerComponent::ptr SceneManager::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if ( index == -1 ) return ISceneManagerComponent::ptr();
	return GetComponent( index );
}

int SceneManager::FindComponent( std::string name, int startIndex ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( i >= startIndex && unify::StringIs( component->GetName(), name ) ) return i;
		++i;
	}
	return -1;
}

// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneManager.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;
using namespace scene;

SceneManager::SceneManager()
	: GameComponent( "SceneManager" )
	, m_focusScene( 0 )
	, m_enabled{ true }
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

Scene::ptr SceneManager::AddScene( std::string name )
{
	auto itr = m_scenes.find( name );
	if ( itr != m_scenes.end() )
	{
		throw exception::FailedToCreate( "Attempted to add scene \"" + name + "\", but it already exists!" );
	}

    Scene::ptr scene( new Scene( GetGame(), name ) );

	m_scenes[ name ] = scene;
	m_sceneList.push_back( scene );
    return scene;
}

Scene::ptr SceneManager::FindScene( std::string name ) const
{
	auto itr = m_scenes.find( name );
	return itr == m_scenes.end() ? Scene::ptr() : itr->second;
}
													  
Scene::ptr SceneManager::GetScene( size_t index ) const
{
	if ( index >= m_sceneList.size() ) return Scene::ptr();
	return m_sceneList[ index ];
}

void SceneManager::OnUpdate( UpdateParams params )
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
        scene->Update( params );
    }
}

void SceneManager::OnRender( RenderParams params )
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
        scene->Render( params );
    }
}

std::string SceneManager::GetWhat() const
{
	return std::string();
}

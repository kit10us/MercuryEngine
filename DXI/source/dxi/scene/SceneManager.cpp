// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/SceneManager.h>
#include <dxi/core/Game.h>
#include <qxml/Document.h>
#include <dxi/exception/NotImplemented.h>
#include <dxi/exception/FailedToCreate.h>

using namespace dxi;
using namespace scene;

SceneManager::SceneManager()
: m_focusScene( 0 )
, m_enabled( true )
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

void SceneManager::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool SceneManager::GetEnabled() const
{
	return m_enabled;
}

SceneLoader & SceneManager::GetSceneLoader()
{
	return m_sceneLoader;
}

Scene::shared_ptr SceneManager::Add( std::string name, scene::Scene * scene )
{
    Scene::shared_ptr sceneSharedPtr( scene );
	m_scenes[ name ] = sceneSharedPtr;
    return sceneSharedPtr;
}

Scene::shared_ptr SceneManager::Load( std::string name, unify::Path scenePath )
{
	Scene::shared_ptr scene = Find( name ); 
	if ( scene.get() )
	{
		throw exception::FailedToCreate( "Scene \"" + name + "\" already exists!" );
	}

	scene.reset( new Scene );

	m_sceneLoader.LoadSceneFromXML( scene, scenePath );

	m_scenes[name] = scene;

	return scene;
}

Scene::shared_ptr SceneManager::Find( std::string name )
{
	std::map< std::string, Scene::shared_ptr >::iterator itr = m_scenes.find( name );
	return itr == m_scenes.end() ? Scene::shared_ptr() : itr->second;
}

void SceneManager::Update( unify::Seconds elapsed, core::IInput & input )
{
	if ( m_enabled == false )
	{
		return;
	}

    std::list< Scene * > sceneList;
    for( std::map< std::string, Scene::shared_ptr >::iterator itr = m_scenes.begin(), end = m_scenes.end(); itr != end; ++itr )
    {
        Scene * scene = (*itr).second.get();
        if ( scene->GetEnabled() )
        {
            sceneList.push_back( scene );
        }
    }

    unify::Any onUpdateEventData( EventData::OnUpdate( this, sceneList, elapsed, input ) );
    GetListenerMap().Fire( "onUpdate", onUpdateEventData );
        
    if ( ! m_focusScene || m_focusScene->CantLoseFocus() == false )
    {
        // Verify or find the focus scene...

        class SortSceneList
        {
        public:
            bool operator()( Scene * a, Scene * b ) const
            {
                return a->GetOrder() > b->GetOrder(); // NOTE: reverse order
            }
        };
        sceneList.sort( SortSceneList() );

        // Find first window we are over (in order of top to bottom, so that becomes focus window)...
        Scene * newFocusScene = 0;
        for ( std::list< Scene * >::iterator itr = sceneList.begin(), end = sceneList.end(); itr != end; ++itr )
        {
            Scene * scene = (*itr);
            unify::Rect< float > sceneRect( scene->GetPosition(), scene->GetSize() );
			if ( sceneRect.IsIn( input.MouseV2< float >() ) )
            {
                newFocusScene = scene;
                break;
            }
        }

        if ( newFocusScene != m_focusScene )
        {
            if ( m_focusScene )
            {
                m_focusScene->SetFocus( false );
                m_focusScene->GetListenerMap().Fire( "onFocusLost", unify::Any( scene::Scene::EventData::OnFocusLost( m_focusScene ) ) );
            }

            if ( newFocusScene )
            {
                newFocusScene->SetFocus( true );
                newFocusScene->GetListenerMap().Fire( "onFocus", unify::Any( scene::Scene::EventData::OnFocus( newFocusScene, m_focusScene ) ) );
            }

            m_focusScene = newFocusScene;
        }
    }

    // Update all scenes...
    for ( std::list< Scene * >::iterator itr = sceneList.begin(), end = sceneList.end(); itr != end; ++itr )
    {
        Scene * scene = (*itr);
        scene->Update( elapsed, input );
    }
}

void SceneManager::Render()
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

    for( std::map< std::string, Scene::shared_ptr >::iterator itr = m_scenes.begin(), end = m_scenes.end(); itr != end; ++itr )
	{
        Scene * scene = (*itr).second.get();
        if ( scene->GetVisible() )
        {
            sceneList.push_back( scene );
        }
	}

    sceneList.sort( SortSceneList() );

    for ( std::list< Scene * >::iterator itr = sceneList.begin(), end = sceneList.end(); itr != end; ++itr )
    {
        Scene * scene = (*itr);
        scene->Render();
    }
}


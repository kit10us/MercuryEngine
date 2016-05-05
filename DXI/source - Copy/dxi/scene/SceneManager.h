// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/loader/SceneLoader.h>
#include <memory>

namespace dxi
{
    namespace scene
    {
        class SceneManager : public events::ListenerMapOwner
	    {
	    public:
			typedef std::shared_ptr< SceneManager > shared_ptr;

            struct EventData
            {
				typedef std::tuple< SceneManager *, std::list< Scene * > &, unify::Seconds, core::IInput & > OnUpdate;
            };

		    SceneManager();
			SceneManager( GeometryManager::shared_ptr geometryManager );
		    virtual ~SceneManager();

            void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;

		    void SetGeometryManager( GeometryManager::shared_ptr geometryManager );
		    GeometryManager::shared_ptr GetGeometryManager() const;

            Scene::shared_ptr Add( const std::string & name, scene::Scene * scene );
            Scene::shared_ptr Load( const std::string & name, const unify::Path & path );
		    Scene::shared_ptr Find( const std::string & name );

            loader::SceneLoader & GetSceneLoader();

            std::map< std::string, Scene::shared_ptr > & GetScenes();
            const std::map< std::string, Scene::shared_ptr > & GetScenes() const;

			void Update( unify::Seconds elapsed, core::IInput & input );
		    void Render();

	    private:
            loader::SceneLoader m_sceneLoader;
			GeometryManager::shared_ptr m_geometryManager;
			std::map< std::string, Scene::shared_ptr > m_scenes;
            Scene * m_focusScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			bool m_enabled;
	    };
    }
}
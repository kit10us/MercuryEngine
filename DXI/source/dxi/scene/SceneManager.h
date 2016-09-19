// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/events/ListenerMap.h>
#include <dxi/scene/Scene.h>
#include <dxi/scene/SceneLoader.h>
#include <dxi/core/GameDependant.h>

namespace dxi
{
    namespace scene
    {
        class SceneManager : public events::ListenerMapOwner, public core::GameDependant
	    {
	    public:
			typedef std::shared_ptr< SceneManager > shared_ptr;

            struct EventData
            {
				typedef std::tuple< SceneManager *, std::list< Scene * > &, unify::Seconds, core::IInput & > OnUpdate;
            };

		    SceneManager( core::IGame & game );
		    virtual ~SceneManager();

            void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;
			
			SceneLoader & GetSceneLoader();

            Scene::shared_ptr Add( std::string name );
            Scene::shared_ptr Load( std::string name, unify::Path path );
		    Scene::shared_ptr Find( std::string name );

            std::map< std::string, Scene::shared_ptr > & GetScenes();
            const std::map< std::string, Scene::shared_ptr > & GetScenes() const;

			void Update( unify::Seconds elapsed, core::IInput & input );
		    void Render();

	    private:
			SceneLoader m_sceneLoader;
			std::map< std::string, Scene::shared_ptr > m_scenes;
            Scene * m_focusScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			bool m_enabled;
	    };
    }
}
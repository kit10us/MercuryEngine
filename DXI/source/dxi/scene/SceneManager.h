// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/events/ListenerMap.h>
#include <dxi/scene/Scene.h>

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
		    virtual ~SceneManager();

            void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;

            Scene::shared_ptr Add( const std::string & name, scene::Scene * scene );
            Scene::shared_ptr Load( const std::string & name, const unify::Path & path );
		    Scene::shared_ptr Find( const std::string & name );

            std::map< std::string, Scene::shared_ptr > & GetScenes();
            const std::map< std::string, Scene::shared_ptr > & GetScenes() const;

			void Update( unify::Seconds elapsed, core::IInput & input );
		    void Render();

	    private:
			std::map< std::string, Scene::shared_ptr > m_scenes;
            Scene * m_focusScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			bool m_enabled;
	    };
    }
}
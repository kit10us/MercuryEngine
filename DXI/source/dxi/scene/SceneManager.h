// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/scene/Scene.h>
#include <dxi/scene/ISceneManagerComponent.h>

namespace dxi
{
    namespace scene
    {
        class SceneManager
	    {
	    public:
			typedef std::shared_ptr< SceneManager > shared_ptr;

		    SceneManager( core::IGame * game );
		    virtual ~SceneManager();

			core::IGame * GetGame();
			const core::IGame * GetGame() const;

            void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;			

            Scene::ptr Add( std::string name );
		    Scene::ptr Find( std::string name );

			void Update( const RenderInfo & renderInfo );
		    void Render( size_t renderer, const Viewport & viewport );

			int ComponentCount() const;
			void AddComponent( ISceneManagerComponent::ptr component );
			void RemoveComponent( ISceneManagerComponent::ptr component );
			ISceneManagerComponent::ptr GetComponent( int index );
			ISceneManagerComponent::ptr GetComponent( std::string name, int startIndex = 0 );
			int FindComponent( std::string name, int startIndex = 0 ) const;

	    private:
			core::IGame * m_game;
			std::list< ISceneManagerComponent::ptr > m_components;

			std::map< std::string, Scene::ptr > m_scenes;
            Scene * m_focusScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			bool m_enabled;
	    };
    }
}
// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/scene/Scene.h>
#include <me/GameComponent.h>
#include <me/scene/ISceneManagerComponent.h>

namespace me
{
    namespace scene
    {
        class SceneManager : public GameComponent
	    {
	    public:
			typedef std::shared_ptr< SceneManager > shared_ptr;

		    SceneManager();
		    virtual ~SceneManager();

            void Destroy();

			size_t GetSceneCount() const;
            Scene::ptr AddScene( std::string name );
			Scene::ptr FindScene( std::string name ) const;
			Scene::ptr GetScene( size_t index ) const;

		public: // IGameCompnent...
			void OnUpdate( UpdateParams params ) override;
			void OnRender( RenderParams params ) override;
			
			int ComponentCount() const;
			void AddComponent( ISceneManagerComponent::ptr component );
			void RemoveComponent( ISceneManagerComponent::ptr component );
			ISceneManagerComponent::ptr GetComponent( int index );
			ISceneManagerComponent::ptr GetComponent( std::string typeName, int startIndex = 0 );
			int FindComponent( std::string typeName, int startIndex = 0 ) const;

		public: // IComponent...
			std::string GetWhat() const;

		private:
			IGame * m_game;
			std::list< ISceneManagerComponent::ptr > m_components;

			std::map< std::string, Scene::ptr > m_scenes;
			std::vector< Scene::ptr > m_sceneList;
			Scene * m_focusScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			bool m_enabled;
	    };
    }
}
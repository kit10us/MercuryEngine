// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/scene/Scene.h>
#include <me/IGameComponent.h>
#include <me/scene/ISceneManagerComponent.h>

namespace me
{
    namespace scene
    {
        class SceneManager : public IGameComponent
	    {
	    public:
			typedef std::shared_ptr< SceneManager > shared_ptr;

		    SceneManager( IGame * game );
		    virtual ~SceneManager();

			std::string GetName() const override;

			IGame * GetGame();
			const IGame * GetGame() const;

            void Destroy();

			void SetEnabled( bool enabled );
			bool GetEnabled() const;			

            Scene::ptr Add( std::string name );
			Scene::ptr Find( std::string name );

			void OnAttach( IGame * game ) override;
			void OnDetach( IGame * game ) override;
			void OnUpdate( IGame * game, const RenderInfo & renderInfo ) override;
			void OnRender( IGame * game, const RenderInfo & renderInfo ) override;

			int ComponentCount() const;
			void AddComponent( ISceneManagerComponent::ptr component );
			void RemoveComponent( ISceneManagerComponent::ptr component );
			ISceneManagerComponent::ptr GetComponent( int index );
			ISceneManagerComponent::ptr GetComponent( std::string name, int startIndex = 0 );
			int FindComponent( std::string name, int startIndex = 0 ) const;

	    private:
			IGame * m_game;
			std::list< ISceneManagerComponent::ptr > m_components;

			std::map< std::string, Scene::ptr > m_scenes;
			Scene * m_focusScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			bool m_enabled;
	    };
    }
}
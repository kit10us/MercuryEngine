// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/scene/IScene.h>
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
            void AddScene( std::string name, IScene::ptr scene );
            IScene* AddScene( std::string name );
			IScene* FindScene( std::string name );
			IScene* GetScene( size_t index );
			
			IScene* GetCurrentCurrent();
			IScene* GetPrevious();

			bool ChangeScene( std::string name );

			void AddComponent( ISceneManagerComponent::ptr component );

		public: // IGameCompnent...
			void OnUpdate( UpdateParams params ) override;
			void OnRender( RenderParams params ) override;

		public: // IComponent...
			std::string GetWhat() const;

		private:
			IGame * m_game;

			std::map< std::string, IScene::ptr > m_scenes;
			std::vector< IScene::ptr > m_sceneList;
			IScene * m_currentScene;
			IScene * m_previousScene;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;

			std::vector< ISceneManagerComponent::ptr > m_componentList;
	    };
    }
}
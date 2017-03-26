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

			static char* Name();

		    SceneManager();
		    virtual ~SceneManager();

            void Destroy();

			size_t GetSceneCount() const;
            void AddScene( std::string name, ISceneFactory::ptr sceneFactory );
            void AddScene( std::string name );
			
			int FindSceneIndex( std::string name );
			std::string GetSceneName(int index);
			
			IScene* GetCurrentScene();
			std::string GetPreviousSceneName();

			bool ChangeScene( std::string name );

			void RestartScene();

			int GetComponentCount() const;
			void AddComponent(ISceneManagerComponent::ptr component);
			void RemoveComponent(ISceneManagerComponent::ptr component);
			ISceneManagerComponent* GetComponent(int index);
			ISceneManagerComponent* GetComponent(std::string typeName);
			int FindComponent(std::string typeName) const;


		public: // IGameCompnent...
			void OnUpdate( UpdateParams params ) override;
			void OnRender( RenderParams params ) override;

		public: // IComponent...
			std::string GetWhat() const;

		private:
			std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_scenes;
			std::vector< ISceneFactory::ptr > m_sceneList;
			IScene::ptr m_currentScene;
			std::string m_previousSceneName;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;

			std::list< ISceneManagerComponent::ptr > m_components;
	    };
    }
}
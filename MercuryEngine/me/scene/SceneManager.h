// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once
#include <me/scene/IScene.h>
#include <me/game/GameComponent.h>
#include <me/scene/ISceneManagerComponent.h>

namespace me
{
    namespace scene
    {
        class SceneManager : public game::GameComponent
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

			size_t SceneManager::GetRenderCount() const;

		public: // IGameCompnent...
			void OnEarlyUpdate( const UpdateParams & params ) override;
			void OnUpdate( const UpdateParams & params ) override;
			void OnLateUpdate( const UpdateParams & params ) override;
			void OnRender( const render::Params & params ) override;

			std::string SendCommand( size_t id, std::string extra ) override;

		public: // IComponent...
			std::string GetWhat() const;

		private:
			std::map< std::string, size_t, unify::string::CaseInsensitiveLessThanTest > m_scenes;
			std::vector< ISceneFactory::ptr > m_sceneList;
			IScene::ptr m_currentScene;
			std::string m_previousSceneName;
			unsigned long long m_updateTick;
			unsigned long long m_renderTick;
			size_t m_renderCount;

			std::list< ISceneManagerComponent::ptr > m_components;
	    };
    }
}
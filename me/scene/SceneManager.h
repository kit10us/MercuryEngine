// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/IScene.h>
#include <me/game/component/GameComponent.h>
#include <me/scene/component/ISceneManagerComponent.h>

namespace me::scene
{
	/// <summary>
	/// Standard Scene Manager, enables controlling game scenes.
	/// </summary>
	class SceneManager : public game::component::GameComponent
	{
		kit::debug::IBlock::ptr m_block;
		unify::Lookup< std::string, IScene::ptr > m_scenes;
		IScene::ptr m_currentScene;
		std::string m_previousSceneName;
		unsigned long long m_updateTick;
		unsigned long long m_renderTick;
		size_t m_renderCount;
		std::list< component::ISceneManagerComponent::ptr > m_components;

	public:
		typedef std::shared_ptr< SceneManager > shared_ptr;

		static char* Name();

		SceneManager();
		virtual ~SceneManager();

		void Destroy();

		size_t GetSceneCount() const;
		void AddScene( IScene::ptr scene );

		int FindSceneIndex( std::string name );
		std::string GetSceneName( int index );

		IScene* GetCurrentScene();
		std::string GetPreviousSceneName();

		bool ChangeScene( std::string name );

		void RestartScene();

		int GetComponentCount() const;
		void AddComponent( component::ISceneManagerComponent::ptr component );
		void RemoveComponent( component::ISceneManagerComponent::ptr component );
		component::ISceneManagerComponent* GetComponent( int index );
		component::ISceneManagerComponent* GetComponent( std::string typeName );
		int FindComponent( std::string typeName ) const;

		/// <summary>
		/// Returns the number of objects rendered.
		/// </summary>
		size_t SceneManager::GetRenderCount() const;

	public: // IGameCompnent...
		void OnAttach( game::IGame* gameInstance ) override;
		void OnEarlyUpdate( const UpdateParams& params ) override;
		void OnUpdate( const UpdateParams& params ) override;
		void OnLateUpdate( const UpdateParams& params ) override;
		void OnRender( const render::Params& params ) override;

		std::string SendCommand( size_t id, std::string extra ) override;

	public: // IComponent...
		std::string GetWhat() const override;
	};
}
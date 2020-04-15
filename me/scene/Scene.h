// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/game/Game.h>
#include <me/scene/SceneManager.h>
#include <me/scene/component/ISceneComponent.h>
#include <me/scene/GrowableObjectStack.h>
#include <me/render/Viewport.h>
#include <unify/Range.h>
#include <list>
#include <memory>

namespace me
{
	namespace scene
	{
		class Scene : public IScene
		{
			game::Game* m_game;
			std::string m_name;
			unify::Owner::ptr m_ownership;
			std::list< component::ISceneComponent::ptr > m_components;
			IObjectAllocator* m_objectAllocator;
			SceneManager* m_sceneManager;			

		protected:
			kit::debug::IBlock::ptr m_block;

		public:
			Scene(game::Game * gameInstance, std::string name);
			virtual ~Scene();

			unify::Owner::ptr GetOwnership() override;

		public: // Events...
			void Component_OnBeforeStart();
			void Component_OnAfterStart();

			void Component_OnEarlyUpdate( const UpdateParams & params ) override;
			void Component_OnUpdate( const UpdateParams & params ) override;
			void Component_OnLateUpdate( const UpdateParams & params ) override;

			void Component_OnRender( RenderGirl renderGirl ) override;
			void Component_OnSuspend() override;
			void Component_OnResume() override;
			void Component_OnEnd() override;

			// User defined events...
			void OnStart() override {}
			void OnUpdate( const UpdateParams & params ) override {}
			void OnRender( RenderGirl renderGirl ) override {}
			void OnSuspend() override {}
			void OnResume() override {}
			void OnEnd() override {}
			
			std::string SendCommand( size_t id, std::string extra ) override { return std::string();  }

		public:

			game::IGame * GetGame() override;

			me::os::IOS * GetOS() override;

			std::string GetName() const override;

			size_t ObjectCount() const;

			int GetComponentCount() const;
			void AddComponent( component::ISceneComponent::ptr component );
			void RemoveComponent( component::ISceneComponent::ptr component );
			component::ISceneComponent* GetComponent( int index );
			component::ISceneComponent* GetComponent( std::string typeName );
			int FindComponent( std::string typeName ) const;

			IObjectAllocator * GetObjectAllocator();
			
			object::Object * FindObject( std::string name ) override;	
			std::list< HitInstance > FindObjectsWithinRay( unify::Ray ray, float withinDistance ) const override;
			std::list< HitInstance > FindObjectsWithinSphere( unify::BSphere< float > sphere ) const override;

			void AddResources( unify::Path path ) override;

			SceneManager* GetSceneManager();

			template< typename T > 
			rm::ResourceManager< T > * GetManager()
			{
				return m_game->GetManager< T >();
			}

			template< typename T >
			std::shared_ptr< T > GetAsset( std::string name )
			{
				auto asset = GetManager< T >()->Find( name );
				if ( ! asset )
				{
					GetOS()->Debug()->ReportError(  debug::ErrorLevel::Critical, "Could not find asset \"" + name + "\"!" );
				}
				return asset;
			}

		};
	}
}
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

			unify::Result<> Component_BeforeOnStart();
			unify::Result<> Component_AfterOnStart();

			unify::Result<> Component_BeforeOnUpdate( const UpdateParams & params ) override;
			unify::Result<> Component_OnUpdate( const UpdateParams & params ) override;
			unify::Result<> Component_AfterOnUpdate( const UpdateParams & params ) override;

			unify::Result<> Component_OnRender( RenderGirl renderGirl ) override;
			unify::Result<> Component_OnSuspend() override;
			unify::Result<> Component_OnResume() override;
			unify::Result<> Component_OnEnd() override;

			// User defined events...
			unify::Result<> OnStart() override {return {};}
			unify::Result<> OnUpdate( const UpdateParams & params ) override {return{};}
			unify::Result<> OnRender( RenderGirl renderGirl ) override {return {};}
			unify::Result<> OnSuspend() override {return {};}
			unify::Result<> OnResume() override {return {};}
			unify::Result<> OnEnd() override {return {};}
			
			std::string SendCommand( size_t id, std::string extra ) override { return std::string();  }

		public:

			game::IGame * GetGame() override;

			me::os::IOS * GetOS() override;

			std::string GetName() const override;

			size_t ObjectCount() const;

			int GetComponentCount() const;
			unify::Result<> AddComponent( component::ISceneComponent::ptr component ) override;
			void RemoveComponent( component::ISceneComponent::ptr component );
			component::ISceneComponent* GetComponent( size_t index );
			component::ISceneComponent* GetComponent( std::string typeName );
			int FindComponent( std::string typeName ) const;

			IObjectAllocator * GetObjectAllocator();
			
			object::Object * FindObject( std::string name ) override;	
			std::list< HitInstance > FindObjectsWithinRay( unify::Ray<float> ray, float withinDistance ) const override;
			std::list< HitInstance > FindObjectsWithinSphere( unify::BSphere< float > sphere ) const override;

			unify::Result<> AddResources( unify::Path path ) override;

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
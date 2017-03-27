// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/Game.h>
#include <me/scene/ISceneComponent.h>
#include <me/scene/GrowableObjectStack.h>
#include <me/Viewport.h>
#include <unify/Range.h>
#include <list>
#include <memory>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	namespace scene
	{
		class Scene : public IScene
		{
		public:
			Scene(Game * game, std::string name);
			virtual ~Scene();

			unify::Owner::ptr GetOwnership() override;

		public: 
			void Component_OnBeforeStart();
			void Component_OnAfterStart();

			void Component_OnEarlyUpdate( UpdateParams params ) override;
			void Component_OnUpdate( UpdateParams params ) override;
			void Component_OnLateUpdate( UpdateParams params ) override;

			void Component_OnRender( RenderGirl renderGirl ) override;
			void Component_OnSuspend() override;
			void Component_OnResume() override;
			void Component_OnEnd() override;

			// User defined events...
			void OnStart() override {}
			void OnUpdate( UpdateParams params ) override {}
			void OnRender( RenderGirl renderGirl ) override {}
			void OnSuspend() override {}
			void OnResume() override {}
			void OnEnd() override {}

		public:

			me::Game * GetGame() override;

			me::IOS * GetOS() override;

			std::string GetName() const override;

			size_t ObjectCount() const;

			int GetComponentCount() const;
			void AddComponent( ISceneComponent::ptr component );
			void RemoveComponent( ISceneComponent::ptr component );
			ISceneComponent* GetComponent( int index );
			ISceneComponent* GetComponent( std::string typeName );
			int FindComponent( std::string typeName ) const;

			IObjectAllocator * GetObjectAllocator();
			
			object::Object * FindObject( std::string name );	 

			std::string SendCommand( std::string command, std::string extra );

			template< typename T > 
			rm::ResourceManagerSimple< T > * GetManager()
			{
				return GetGame()->GetManager< T >();
			}

		private:
			Game * m_game;
			std::string m_name;	 
			unify::Owner::ptr m_ownership;
			std::list< ISceneComponent::ptr > m_components;
			IObjectAllocator* m_objectAllocator;		   
		};
	}
}
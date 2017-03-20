// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/Game.h>
#include <me/scene/ISceneComponent.h>
#include <me/scene/GrowableObjectStack.h>
#include <me/Viewport.h>
#include <unify/MinMax.h>
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
			Scene( Game * game, std::string name );
			virtual ~Scene();

		protected: // User defined events...
			void OnEnterScene( IScene* previous ) override {}
			void OnLeaveScene( IScene * next ) override {}
			void OnStart() override {}
			void OnUpdate( UpdateParams params ) override {}
			void OnRender( RenderParams params ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

		public:
			void EnterScene( IScene* previous ) override;
			void LeaveScene( IScene * next ) override;
			void Start() override;
			void Update( UpdateParams params ) override;
			void Render( RenderParams params ) override;
			void Suspend() override;
			void Resume() override;

			me::Game * GetGame() override;

			me::IOS * GetOS() override;

			std::string GetName() const override;

			void Restart() override;

			size_t ObjectCount() const;

			int GetComponentCount() const;
			void AddComponent( ISceneComponent::ptr component );
			void RemoveComponent( ISceneComponent::ptr component );
			ISceneComponent* GetComponent( int index );
			ISceneComponent* GetComponent( std::string typeName );
			int FindComponent( std::string typeName ) const;

			IObjectAllocator * GetObjectAllocator();
			
			object::Object * FindObject( std::string name );	 

			size_t GetRenderCount() const;

			template< typename T > 
			rm::ResourceManagerSimple< T > * GetManager()
			{
				return GetGame()->GetManager< T >();
			}

		private:
			Game * m_game;
			std::string m_name;	 
			std::list< ISceneComponent::ptr > m_components;
			IObjectAllocator* m_objectAllocator;		   
			size_t m_renderCount;
		};
	}
}
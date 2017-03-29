// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/scene/GrowableObjectStack.h>
#include <me/Viewport.h>
#include <unify/Owner.h>
#include <unify/Range.h>
#include <list>
#include <memory>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	class IGame;

	namespace scene
	{
		class IScene
		{
		public:
			typedef std::shared_ptr< IScene > ptr;

		public:
			virtual void Component_OnBeforeStart() = 0;
			virtual void Component_OnAfterStart() = 0;

			virtual void Component_OnEarlyUpdate( UpdateParams & params ) = 0;
			virtual void Component_OnUpdate( UpdateParams & params ) = 0;
			virtual void Component_OnLateUpdate( UpdateParams & params ) = 0;

			virtual void Component_OnRender( RenderGirl renderGirl ) = 0;
			virtual void Component_OnSuspend() = 0;
			virtual void Component_OnResume() = 0;
			virtual void Component_OnEnd() = 0;

			virtual void OnStart() = 0;			
			virtual void OnUpdate( UpdateParams & params ) = 0;			
			virtual void OnRender( RenderGirl renderGirl ) = 0;
			virtual void OnSuspend() = 0;			
			virtual void OnResume() = 0;			
			virtual void OnEnd() = 0;

		public:

			virtual unify::Owner::ptr GetOwnership() = 0;

			virtual me::IGame * GetGame() = 0;

			virtual me::IOS * GetOS() = 0;

			virtual std::string GetName() const = 0;

			virtual size_t ObjectCount() const = 0;

			virtual int GetComponentCount() const = 0;

			virtual void AddComponent( ISceneComponent::ptr component ) = 0;

			virtual void RemoveComponent( ISceneComponent::ptr component ) = 0;

			virtual ISceneComponent* GetComponent( int index ) = 0;

			virtual ISceneComponent* GetComponent( std::string typeName ) = 0;

			virtual int FindComponent( std::string typeName ) const = 0;

			virtual IObjectAllocator * GetObjectAllocator() = 0;
			
			virtual object::Object * FindObject( std::string name ) = 0;

			virtual std::string SendCommand( std::string command, std::string extra ) = 0;

			template< typename T >
			T* GetComponentT();

			template< typename T >
			T* GetComponentT( int index );
		};

		template< typename T >
		T* IScene::GetComponentT()
		{
			auto component = GetComponent( T::Name() );
			return dynamic_cast< T* >( component );
		}

		template< typename T >
		T* IScene::GetComponentT( int index )
		{
			auto component = GetComponent( index );
			return dynamic_cast< T* >( component );
		}
	}
}
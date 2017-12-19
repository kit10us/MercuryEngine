// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/scene/GrowableObjectStack.h>
#include <me/render/Viewport.h>
#include <unify/Owner.h>
#include <unify/Range.h>
#include <list>
#include <memory>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	namespace game
	{
		class IGame;
	}

	struct HitInstance
	{
		object::Object * object;
		float distance;

		bool operator<( const HitInstance & b ) const
		{
			return distance < b.distance;
		}
	};

	namespace scene
	{
		class IScene
		{
		public:
			typedef std::shared_ptr< IScene > ptr;

		public:
			virtual void Component_OnBeforeStart() = 0;
			virtual void Component_OnAfterStart() = 0;

			virtual void Component_OnEarlyUpdate( const UpdateParams & params ) = 0;
			virtual void Component_OnUpdate( const UpdateParams & params ) = 0;
			virtual void Component_OnLateUpdate( const UpdateParams & params ) = 0;

			virtual void Component_OnRender( RenderGirl renderGirl ) = 0;
			virtual void Component_OnSuspend() = 0;
			virtual void Component_OnResume() = 0;
			virtual void Component_OnEnd() = 0;

			virtual void OnStart() = 0;			
			virtual void OnUpdate( const UpdateParams & params ) = 0;			
			virtual void OnRender( RenderGirl renderGirl ) = 0;
			virtual void OnSuspend() = 0;			
			virtual void OnResume() = 0;	
		
			virtual void OnEnd() = 0;

			virtual std::string SendCommand( size_t id, std::string extra ) = 0;

		public:

			virtual unify::Owner::ptr GetOwnership() = 0;

			virtual game::IGame * GetGame() = 0;

			virtual IOS * GetOS() = 0;

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

			virtual std::list< HitInstance > FindObjectsWithinRay( unify::Ray ray, float withinDistance ) const = 0;

			virtual std::list< HitInstance > FindObjectsWithinSphere( unify::BSphere< float > sphere ) const = 0;

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
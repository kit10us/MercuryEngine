// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/scene/component/ISceneComponent.h>
#include <me/scene/GrowableObjectStack.h>
#include <me/render/Viewport.h>
#include <unify/Owner.h>
#include <unify/Range.h>
#include <list>
#include <memory>

namespace me
{
	namespace game
	{
		class IGame;
	}

	namespace os
	{
		class IOS;
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
			/// <summary>
			/// This is peformed immediately before the user's OnStart.
			/// </summary>
			virtual void Component_OnBeforeStart() = 0;

			/// <summary>
			/// This is peformed immediately after the user's OnStart.
			/// </summary>
			virtual void Component_OnAfterStart() = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnEarlyUpdate( const UpdateParams & params ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnUpdate( const UpdateParams & params ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnLateUpdate( const UpdateParams & params ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnRender( RenderGirl renderGirl ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnSuspend() = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnResume() = 0;

			/// <summary>
			///
			/// </summary>
			virtual void Component_OnEnd() = 0;

			/// <summary>
			///
			/// </summary>
			virtual void OnStart() = 0;			

			/// <summary>
			/// User defined scene updates. Interval updates (lower resolution) can be facilitated by modulus for 0 and the FrameId, for example, every 60 ticks would be FrameID % 60 == 0.
			/// </summary>
			virtual void OnUpdate( const UpdateParams & params ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void OnRender( RenderGirl renderGirl ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void OnSuspend() = 0;

			/// <summary>
			///
			/// </summary>
			virtual void OnResume() = 0;
		
			/// <summary>
			///
			/// </summary>
			virtual void OnEnd() = 0;

			/// <summary>
			///
			/// </summary>
			virtual std::string SendCommand( size_t id, std::string extra ) = 0;

		public:

			/// <summary>
			///
			/// </summary>
			virtual unify::Owner::ptr GetOwnership() = 0;

			/// <summary>
			///
			/// </summary>
			virtual game::IGame * GetGame() = 0;

			/// <summary>
			///
			/// </summary>
			virtual os::IOS * GetOS() = 0;

			/// <summary>
			///
			/// </summary>
			virtual std::string GetName() const = 0;

			/// <summary>
			///
			/// </summary>
			virtual size_t ObjectCount() const = 0;

			/// <summary>
			///
			/// </summary>
			virtual int GetComponentCount() const = 0;

			/// <summary>
			///
			/// </summary>
			virtual void AddComponent( component::ISceneComponent::ptr component ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual void RemoveComponent( component::ISceneComponent::ptr component ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual component::ISceneComponent* GetComponent( int index ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual component::ISceneComponent* GetComponent( std::string typeName ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual int FindComponent( std::string typeName ) const = 0;

			/// <summary>
			///
			/// </summary>
			virtual IObjectAllocator * GetObjectAllocator() = 0;
			
			/// <summary>
			///
			/// </summary>
			virtual object::Object * FindObject( std::string name ) = 0;

			/// <summary>
			///
			/// </summary>
			virtual std::list< HitInstance > FindObjectsWithinRay( unify::Ray ray, float withinDistance ) const = 0;

			/// <summary>
			///
			/// </summary>
			virtual std::list< HitInstance > FindObjectsWithinSphere( unify::BSphere< float > sphere ) const = 0;

			/// <summary>
			/// Loads a resource file connected to the scene.
			/// </summary>
			virtual void AddResources( unify::Path path ) = 0;

			/// <summary>
			///
			/// </summary>
			template< typename T >
			T* GetComponentT();

			/// <summary>
			///
			/// </summary>
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
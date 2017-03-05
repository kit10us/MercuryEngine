// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
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
		// For backups
		enum RenderListStates
		{
			STATE_RENDER = 0,
			STATE_LIGHTING,
			STATE_ZWRITEENABLE,
			STATE_COUNT,
		};

		class Scene
		{
		public:
			typedef std::shared_ptr< Scene > ptr;

			Scene( IGame * game, std::string name );
			virtual ~Scene();

			std::string GetName() const;

			size_t ObjectCount() const;

			void OnInit();
			void OnStart();
			void Update( UpdateParams params );
			void Render( RenderParams params );
			void Suspend();
			void Resume();

			void SetSize( const unify::Size< float > & size );
			unify::Size< float > GetSize() const;
		    
			void SetPosition( const unify::V2< float > & position );
			unify::V2< float > GetPosition() const;
		    
			void SetZ( const unify::MinMax< float > & z );

			// Order amongst other scenes...
			void SetOrder( float order );
			float GetOrder() const;

			void SetEnabled( bool enabled );
			bool GetEnabled() const;

			/// <summary>
			/// Set if we want to render objects or not.
			/// </summary>
			void SetRenderObjects( bool enabled );
			
			/// <summary>
			/// Returns true if we want to render objects.
			/// </summary>
			bool GetRenderObjects() const;

			int ComponentCount() const;
			void AddComponent( ISceneComponent::ptr component );
			void RemoveComponent( ISceneComponent::ptr component );
			ISceneComponent::ptr GetComponent( int index );
			ISceneComponent::ptr GetComponent( std::string name, int startIndex = 0 );
			int FindComponent( std::string name, int startIndex = 0 ) const;
			
			Object * NewObject( std::string name );
			bool DestroyObject( Object * object );
			Object * CopyObject( Object * from, std::string name );
			void CollectObjects( std::vector< Object * > & objects );
			Object * FindObject( std::string name );

			Object * GetObject( size_t index ) const;

		private:
			IGame * m_game;
			std::string m_name;

			std::list< ISceneComponent::ptr > m_components;

			IObjectAllocator::ptr m_objectStack;
			CameraCache m_cameras;

			bool m_inited;
			bool m_started;

			// Flags...
			float m_order;
			bool m_enabled;
			bool m_renderObjects;

			// Viewport...
			me::Viewport m_viewport;
		};
	}
}
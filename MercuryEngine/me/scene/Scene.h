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

			int GetComponentCount() const;
			void AddComponent( ISceneComponent::ptr component );
			void RemoveComponent( ISceneComponent::ptr component );
			ISceneComponent::ptr GetComponent( int index );
			ISceneComponent::ptr GetComponent( std::string typeName, int startIndex = 0 );
			int FindComponent( std::string typeName, int startIndex = 0 ) const;
			
			Object * NewObject( std::string name );
			bool DestroyObject( Object * object );
			Object * CopyObject( Object * from, std::string name );
			void CollectObjects( std::vector< Object * > & objects );
			Object * FindObject( std::string name );	 
			Object * GetObject( size_t index ) const;
			size_t GetObjectCount() const;

			size_t GetRenderCount() const;

		private:
			IGame * m_game;
			std::string m_name;	 
			std::list< ISceneComponent::ptr > m_components;
			IObjectAllocator::ptr m_objectStack;		   
			bool m_inited;
			bool m_started;								   
			float m_order;
			bool m_enabled;
			size_t m_renderCount;
			me::Viewport m_viewport;
		};
	}
}
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

			Scene( IGame * game );
			virtual ~Scene();

			size_t ObjectCount() const;

			void OnInit();
			void OnStart();
			void Update( IRenderer * renderer, const RenderInfo & renderInfo );
			void Render( IRenderer * renderer, const RenderInfo & renderInfo );
			void Suspend();
			void Resume();

			unsigned int LastCullCount() const;
			void SetRenders( bool bSolids, bool bTrans );
			void SetCulling( bool bCulling );
			bool RayTest( const unify::V3< float > * pFirst, const unify::V3< float > * pSecond );
		    
			void SetDefaultLighting( bool lighting );
		    
			void SetDefaultZWriteEnable( bool zWriteEnable );
		    
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

		private:
			IGame * m_game;

			std::list< ISceneComponent::ptr > m_components;

			GrowableObjectStack m_objectStack;
			CameraCache m_cameras;

			bool m_inited;
			bool m_started;

			unsigned int m_lastCullCount;

			// Flags...
			float m_order;
			bool m_enabled;
			bool m_renderObjects;
			bool m_renderSolids;
			bool m_renderTrans;
			bool m_cullingEnabled;

			// States...
			DWORD m_defaultLighting;
			DWORD m_defaultZWriteEnable;

			// Viewport...
			me::Viewport m_viewport;
		};
	}
}
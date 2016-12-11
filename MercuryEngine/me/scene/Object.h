// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IOS.h>
#include <me/scene/IObjectComponent.h>
#include <me/scene/ComponentInstance.h>
#include <me/Geometry.h>
#include <me/RenderInfo.h>
#include <unify/FrameLite.h>
#include <list>

namespace me
{
	namespace scene
	{
		class Scene;

		/// <notes>
		/// Objects belong to Scenes. Scenes are collections of objects.
		/// </notes>
		class Object
		{
		public:
			Object();

			/// <summary>
			/// Copy object from another object
			/// </summary>
			void CopyFrom( std::string name, Object & objectFrom );

			virtual ~Object();

			void SetName( std::string name );
			std::string GetName() const;

			/// <summary>
			/// Tags allow us to 'tag' objects, so we can find or filter on them.
			/// </summary>
			void AddTag( std::string tag );

			/// <summary>
			/// Searches for a specific tag, ingnoring case.
			/// </summary>
			bool HasTag( std::string tag ) const;
			
			int ComponentCount() const;
			void AddComponent( IObjectComponent::ptr component );
			void RemoveComponent( IObjectComponent::ptr component );
			IObjectComponent::ptr GetComponent( int index );
			IObjectComponent::ptr GetComponent( std::string name, int startIndex = 0 );
			int FindComponent( std::string name, int startIndex = 0 ) const;

			void SetEnabled( bool enabled );
			bool IsEnabled() const;

			unify::FrameLite & GetFrame();
			const unify::FrameLite & GetFrame() const;
	
			void Update( IRenderer * renderer, const RenderInfo & renderInfo );

			/// <summary>
			/// Render or collect the geometry for optimized rendering later.
			/// </summary>
			void CollectRenderables( GeometryCache & cache, IRenderer * renderer, const RenderInfo & renderInfo );

			void OnSuspend();
			void OnResume();

			void SetScene( Scene * scene );
			Scene * GetScene();

			bool IsAlive() const;
			void SetAlive( bool alive );

		protected:
			bool m_alive;
			std::string m_name;
			Scene * m_scene;
			bool m_enabled;
			std::list< std::string > m_tags;
			std::list< ComponentInstance< IObjectComponent::ptr > > m_components;
			unify::FrameLite m_frame;
		};
	}
}
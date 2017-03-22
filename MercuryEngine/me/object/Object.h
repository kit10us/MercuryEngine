// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IOS.h>
#include <me/object/IObjectComponent.h>
#include <me/object/CameraComponent.h>
#include <me/scene/ComponentInstance.h>
#include <me/Geometry.h>
#include <me/RenderInfo.h>
#include <unify/FrameLite.h>
#include <list>

namespace me
{
	namespace scene
	{
		class IScene;
	}

	namespace object
	{
		struct FinalCamera
		{
			Object * object;
			CameraComponent * camera;
		};
		typedef std::list< FinalCamera > CameraCache;

		/// <notes>
		/// Objects belong to Scenes. Scenes are collections of objects.
		/// </notes>
		class Object
		{
			Object & operator=(Object const &);
		public:
			typedef std::shared_ptr< Object* > ptr;
			typedef std::weak_ptr< Object * > weak_ptr;

			Object();

			Object( Object && );

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
			
			int GetComponentCount() const;
			void AddComponent( IObjectComponent::ptr component );
			void RemoveComponent( IObjectComponent::ptr component );
			IObjectComponent::ptr GetComponent( int index );
			IObjectComponent::ptr GetComponent( std::string typeName );
			int FindComponent( std::string name ) const;

			void SetEnabled( bool enabled );
			bool IsEnabled() const;

			bool IsUpdateable() const;

			unify::FrameLite & GetFrame();
			const unify::FrameLite & GetFrame() const;
				  
			/// <summary>
			/// Initialize object and it's components.
			/// Collect updateables.
			/// Collect cameras.
			/// </summary>
			void Initialize( IObjectComponent::cache & updateables, CameraCache & cameras, UpdateParams params );

			void CollectGeometry( GeometryCache & geometries );


			void OnSuspend();
			void OnResume();

			void SetScene( scene::IScene * scene );
			scene::IScene * GetScene();

			bool IsAlive() const;
			void SetAlive( bool alive );

		protected:
			bool m_alive;
			std::string m_name;
			scene::IScene * m_scene;
			bool m_enabled;
			std::list< std::string > m_tags;
			std::list< ComponentInstance< IObjectComponent::ptr > > m_components;
			unify::FrameLite m_frame;

		public:
			template< typename T >
			T* GetComponentT( std::string typeName );

			template< typename T >
			T* GetComponentT( int index );
		};

		template< typename T >
		T* Object::GetComponentT( std::string typeName )
		{
			auto component = GetComponent( typeName ).get();
			return dynamic_cast< T* >( component );
		}

		template< typename T >
		T* Object::GetComponentT( int index )
		{
			auto component = GetComponent( index ).get();
			return dynamic_cast< T* >( component );
		}
	}
}
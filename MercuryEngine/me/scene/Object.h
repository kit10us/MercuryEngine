// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IOS.h>
#include <me/scene/IObjectComponent.h>
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
			typedef std::shared_ptr< Object > ptr;

			explicit Object( IOS * os );
			Object( IOS * os, Scene * scene );
			Object( const Object & object, std::string name );
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

			void SetSelectable( bool selectable );
			bool GetSelectable() const;
			
			void CheckFrame( bool checkFrame );
			bool CheckFrame() const;

			unify::FrameLite & GetFrame();
			const unify::FrameLite & GetFrame() const;
	
			void Update( const RenderInfo & renderInfo );
			void RenderSimple( const RenderInfo & renderInfo );
			void RenderHierarchical( const RenderInfo & renderInfo );
			void OnSuspend();
			void OnResume();
		    
			Object::ptr GetPrevious();
			const Object::ptr GetPrevious() const;

			Object::ptr GetNext();
			const Object::ptr GetNext() const;

			Object::ptr GetParent();
			const Object::ptr GetParent() const;

			Object::ptr GetFirstChild();
			const Object::ptr GetFirstChild() const;

			/// <summary>
			/// Add a child object.
			/// </summary>
			Object::ptr AddChild( std::string name );

			/// <summary>
			/// Duplicate ourselves as a sibling, with a new name.
			/// </summary>
			Object::ptr Duplicate( std::string name );

			Object::ptr FindObject( std::string name );
		
			std::list< Object * > AllChildren( bool recursive );

			Scene * GetScene();

		protected:
			std::string m_name;
			IOS * m_os;
			Scene * m_scene;
			std::list< std::string > m_tags;

			bool m_enabled;
			bool m_selectable;	  

			// TODO: Template, and move to seperate file?
			struct ComponentHolder
			{
				ComponentHolder()
					: initDone( false )
					, startDone( false )
				{
				}

				ComponentHolder( IObjectComponent::ptr component )
					: ComponentHolder()
				{
					c = component;
				}

				bool operator==( const ComponentHolder & ch )
				{
					return this->c.get() == ch.c.get();
				}

				bool initDone;
				bool startDone;
				IObjectComponent::ptr c;
			};
											 
			std::list< ComponentHolder > m_components;

			size_t m_lastFrameID;
			bool m_checkFrame;

			unify::FrameLite m_frame;

			Object::ptr m_previous;
			Object::ptr m_next;
			Object::ptr m_parent;
			Object::ptr m_firstChild;
		};
	}
}
// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/scene/IComponent.h>
#include <dxi/scene/GeometryComponent.h>
#include <dxi/Effect.h>
#include <dxi/RenderInfo.h>
#include <dxi/Geometry.h>
#include <dxi/controllers/IController.h>
#include <dxi/physics/IInstance.h>
#include <dxi/physics/IObjectSyncer.h>
#include <dxi/events/ListenerMap.h>
#include <unify/FrameLite.h>
#include <tuple>

namespace dxi
{
    namespace scene
    {
		/// <notes>
        /// Objects belong to Scenes. Scenes are collections of objects.
		/// </notes>
        class Object //: public physics::IObjectSyncer, public events::ListenerMapOwner
	    {
	    public:
		    typedef std::shared_ptr< Object > ptr;

            struct EventData // Grouping of event datas...
            {
				typedef std::tuple< Object *, unify::Seconds > OnUpdate;
                typedef std::tuple< Object *, float /*distance*/ > OnOverEnter;
                typedef std::tuple< Object * /*previous object*/, Object * /*new object*/ > OnOverExit;
                typedef std::tuple< Object *, float /*distance*/ > OnSelect;
                typedef std::tuple< Object *, unify::V3< float > /*original mouse coordinates*/ > OnDown;
                typedef std::tuple< Object *, unify::V3< float > /*final mouse coordinates*/ > OnUp;
                typedef std::tuple< Object *, unify::V3< float > /*new mouse coordiantes*/, unify::Ray< float >, bool /*once*/ > OnDrag;
                typedef std::tuple< Object *, unify::V3< float > /*new mouse coordiantes*/, unify::Ray< float > > OnDragStop;
            };

		    explicit Object( core::IOS * os );
			Object( core::IOS * os, Geometry::ptr geometry, std::shared_ptr< physics::IInstance > physics = std::shared_ptr< physics::IInstance >() );
			Object( core::IOS * os, Geometry::ptr geometry, const unify::V3< float > position );
		    virtual ~Object();

			void SetName( std::string name );
			std::string GetName() const;

			/// <summary>
			/// Tags allow us to 'tag' objects, so we can find or filter on them.
			/// </summary>
			std::map< std::string, std::string > & GetTags();

			const std::map< std::string, std::string > & GetTags() const;

			int ComponentCount() const;
			void AddComponent( IComponent::ptr component );
			void RemoveComponent( IComponent::ptr component );
			IComponent::ptr GetComponent( int index );
			IComponent::ptr GetComponent( std::string name, int startIndex = 0 );
			int FindComponent( std::string name, int startIndex = 0 ) const;

            void SetEnabled( bool enabled );
            bool IsEnabled() const;

            void SetSelectable( bool selectable );
            bool GetSelectable() const;
			
		    void CheckFrame( bool checkFrame );
		    bool CheckFrame() const;

		    unify::FrameLite & GetFrame();
		    const unify::FrameLite & GetFrame() const;

			void SetGeometry( Geometry::ptr geometry );	

			unify::Matrix & GetGeometryMatrix();

		    controllers::IController::shared_ptr GetController();
		    void SetController( controllers::IController::shared_ptr controller );
			
			void OnInit();
			void OnStart();
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

			Object::ptr AddChild( std::string name );

			Object::ptr FindObject( std::string name );

		protected:
			core::IOS * m_os;
			std::string m_name;
			std::map< std::string, std::string > m_tags;

            bool m_enabled;
            bool m_selectable;	  

			std::list< IComponent::ptr > m_components;

            // Event listeners...
            events::Listener::shared_ptr m_onUpdate;

		    size_t m_lastFrameID;
		    bool m_checkFrame;

		    unify::FrameLite m_frame;

		    controllers::IController::shared_ptr m_controller;

			Object::ptr m_previous;
			Object::ptr m_next;
			Object::ptr m_parent;
			Object::ptr m_firstChild;
	    };
    }
}
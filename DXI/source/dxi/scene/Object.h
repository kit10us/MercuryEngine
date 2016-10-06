// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

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
		    typedef std::shared_ptr< Object > shared_ptr;
		    typedef std::weak_ptr< Object > weak_ptr;

            struct EventData // Grouping of event datas...
            {
				typedef std::tuple< Object *, unify::Seconds, core::IInput & > OnUpdate;
                typedef std::tuple< Object *, float /*distance*/, core::IInput & > OnOverEnter;
                typedef std::tuple< Object * /*previous object*/, Object * /*new object*/ > OnOverExit;
                typedef std::tuple< Object *, float /*distance*/, core::IInput & > OnSelect;
                typedef std::tuple< Object *, unify::V3< float > /*original mouse coordinates*/ > OnDown;
                typedef std::tuple< Object *, unify::V3< float > /*final mouse coordinates*/ > OnUp;
                typedef std::tuple< Object *, unify::V3< float > /*new mouse coordiantes*/, unify::Ray< float >, bool /*once*/ > OnDrag;
                typedef std::tuple< Object *, unify::V3< float > /*new mouse coordiantes*/, unify::Ray< float > > OnDragStop;
            };

		    Object();
			explicit Object( Geometry::shared_ptr geometry, std::shared_ptr< physics::IInstance > physics = std::shared_ptr< physics::IInstance >() );
			Object( Geometry::shared_ptr geometry, const unify::V3< float > position );
		    virtual ~Object();

			void SetName( std::string name );
			std::string GetName() const;

            void SetEnabled( bool enabled );
            bool GetEnabled() const;

            void SetVisible( bool visible );
            bool GetVisible() const;

            void SetSelectable( bool selectable );
            bool GetSelectable() const;
			
		    void CheckFrame( bool checkFrame );
		    bool CheckFrame() const;
		    unify::FrameLite & GetFrame();
		    const unify::FrameLite & GetFrame() const;
		    void SetEffect( const Effect & pEffect );
		    Effect & GetEffect();

			Geometry::shared_ptr GetGeometry();
			void SetGeometry( Geometry::shared_ptr geometry );

		    std::shared_ptr< physics::IInstance > GetPhysics();
		    std::shared_ptr< const physics::IInstance > GetPhysics() const;
		    void SetPhysics( std::shared_ptr< physics::IInstance > physics );

		    controllers::IController::shared_ptr GetController();
		    void SetController( controllers::IController::shared_ptr controller );
			
			void Update( const RenderInfo & renderInfo, core::IInput & input );
		    void Render( const RenderInfo & renderInfo );
		    
            void RenderBBox( bool bTF );
		    bool RenderBBox() const;
		    unify::BBox< float > GetBBoxXFormed();
			const unify::BBox< float > & GetBBox();
		    void SyncFrame( const unify::Frame & frame );
		    void SyncBBox( const unify::BBox< float > & bbox );
		    
			unify::Matrix & GetGeometryMatrix();
			const unify::Matrix & GetGeometryMatrix() const;

		    GeometryInstanceData::shared_ptr GetGeometryInstanceData();

			/// <summary>
			/// Tags allow us to 'tag' objects, so we can find or filter on them.
			/// </summary>
			std::map< std::string, std::string > & GetTags();
			const std::map< std::string, std::string > & GetTags() const;

			Object::shared_ptr GetPrevious();
			const Object::shared_ptr GetPrevious() const;

			Object::shared_ptr GetNext();
			const Object::shared_ptr GetNext() const;

			Object::shared_ptr GetParent();
			const Object::shared_ptr GetParent() const;

			Object::shared_ptr GetFirstChild();
			const Object::shared_ptr GetFirstChild() const;

			Object::shared_ptr AddChild( std::string name );

			Object::shared_ptr FindObject( std::string name );

		protected:
			std::string m_name;
			std::map< std::string, std::string > m_tags;

            bool m_enabled;
            bool m_visible;
            bool m_selectable;

            // Event listeners...
            events::Listener::shared_ptr m_onUpdate;

		    size_t m_lastFrameID;
		    bool m_checkFrame;

		    unify::FrameLite m_frame;

		    unify::Matrix m_geometryMatrix;
			
			Geometry::shared_ptr m_geometry;
			GeometryInstanceData::shared_ptr m_geometryInstanceData;

			Effect m_effect; // Overrides geometry's effects (passed to)

		    std::shared_ptr< physics::IInstance > m_physics;
		    controllers::IController::shared_ptr m_controller;

		    // Blending backup...
		    bool		m_restoreBlend;
		    unsigned long	m_backup_AB;
		    unsigned long	m_backup_SB;
		    unsigned long	m_backup_DB;

		    bool m_renderBBox;
		    bool m_renderFrame;

			Object::shared_ptr m_previous;
			Object::shared_ptr m_next;
			Object::shared_ptr m_parent;
			Object::shared_ptr m_firstChild;
	    };
    }
}
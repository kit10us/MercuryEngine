// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/core/IGame.h>
#include <dxi/scene/Object.h>
#include <dxi/scene/Camera.h>
#include <dxi/physics/IScene.h>
#include <dxi/events/ListenerMap.h>
#include <dxi/Viewport.h>
#include <unify/MinMax.h>
#include <vector>
#include <map>

#pragma comment( lib, "winmm" )

namespace dxi
{
	// For backups
	enum RenderListStates
	{
		STATE_RENDER = 0,
		STATE_LIGHTING,
		STATE_ZWRITEENABLE,
		STATE_COUNT,
	};

    namespace scene
    {
        class Scene : public events::ListenerMapOwner
	    {
	    public:
		    typedef std::shared_ptr< Scene > shared_ptr;
		    typedef std::weak_ptr< Scene > weak_ptr;
		    typedef std::list< Object::ptr > ObjectList;
		    typedef std::map< std::string, Object::ptr > ObjectMap;

            struct EventData
            {
                typedef std::tuple< Scene * /*new scene*/, Scene * /*prev scene*/ > OnFocus;
                typedef std::tuple< Scene * /*prev scene*/ > OnFocusLost;
				typedef std::tuple< Scene *, ObjectList &, const RenderInfo & > OnUpdate;
            };

		    Scene( core::IGame * game );
		    virtual ~Scene();

			Object::ptr GetRoot();
			const Object::ptr GetRoot() const;

		    void SetPhysicsScene( std::shared_ptr< physics::IScene > scene );
		    physics::IScene * GetPhysicsScene();
		    const physics::IScene * GetPhysicsScene() const;

			RenderInfo & GetRenderInfo();

			void OnInit();
			void OnStart();
			void Update( const RenderInfo & renderInfo );
		    void Render( size_t renderer, const Viewport & viewport );
			void Suspend();
			void Resume();

		    Object::ptr FindObject( const std::string & name );
    
		    unsigned int LastCullCount() const;
		    void SetRenders( bool bSolids, bool bTrans );
		    void SetCulling( bool bCulling );
		    bool Intersect( const unify::V3< float > * pvStart, const unify::V3< float > * pvDirection, unify::V3< float > * pPoint = 0 );
		    bool RayTest( const unify::V3< float > * pFirst, const unify::V3< float > * pSecond );
		    
            void SetDefaultLighting( bool lighting );
		    
            void SetDefaultZWriteEnable( bool zWriteEnable );
		    
            void SetSize( const unify::Size< float > & size );
            unify::Size< float > GetSize() const;
		    
            void SetPosition( const unify::V2< float > & position );
            unify::V2< float > GetPosition() const;
		    
            void SetZ( const unify::MinMax< float > & z );
		    
            void SetColor( const unify::Color & color );

            // Order amongst other scenes...
            void SetOrder( float order );
            float GetOrder() const;

            void SetEnabled( bool enabled );
            bool GetEnabled() const;

            void SetFocus( bool focus );
            bool HasFocus() const;
            bool CantLoseFocus() const;

		    void SetRenderPhysics( bool renderPhysics );
		    bool GetRenderPhysics() const;
    		
            Object::ptr GetObjectOver() const;
            void SetObjectOver( Object::ptr objectOver );

	    private:
			core::IGame * m_game;
			Object::ptr m_root;
			bool m_inited;
			bool m_started;

			std::shared_ptr< physics::IScene > m_physicsScene;
		    unsigned int m_lastCullCount;

		    // Flags...
		    RenderInfo m_renderInfo;
		    bool m_renderSolids;
		    bool m_renderTrans;
		    bool m_cullingEnabled;

		    // States...
		    DWORD m_defaultLighting;
		    DWORD m_defaultZWriteEnable;

		    // Viewport...
			Viewport m_viewport;

		    unify::Color m_color;
		    bool m_renderPhysics;

            bool m_hasFocus;
            float m_order;
            bool m_enabled;

            Object::ptr m_objectOver;
            float m_mouseDownTimeLimit;
            float m_mouseDownTime;
            bool m_mouseDrag;
	    };
    }
}
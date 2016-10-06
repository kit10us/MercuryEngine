// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/scene/Object.h>
#include <dxi/scene/Camera.h>
#include <dxi/physics/IScene.h>
#include <dxi/events/ListenerMap.h>
#include <dxi/Viewport.h>
#include <unify/MinMax.h>
#include <vector>
#include <map>
#include <dxi/core/GameDependant.h>

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
        class Scene : public events::ListenerMapOwner, public core::GameDependant
	    {
	    public:
		    typedef std::shared_ptr< Scene > shared_ptr;
		    typedef std::weak_ptr< Scene > weak_ptr;
		    typedef std::list< Object::shared_ptr > ObjectList;
		    typedef std::map< std::string, Object::shared_ptr > ObjectMap;

            struct EventData
            {
                typedef std::tuple< Scene * /*new scene*/, Scene * /*prev scene*/ > OnFocus;
                typedef std::tuple< Scene * /*prev scene*/ > OnFocusLost;
				typedef std::tuple< Scene *, ObjectList &, const RenderInfo &, core::IInput & > OnUpdate;
            };

		    Scene( core::IGame * game );
		    virtual ~Scene();

			Object::shared_ptr GetRoot();
			const Object::shared_ptr GetRoot() const;

		    void SetPhysicsScene( std::shared_ptr< physics::IScene > scene );
		    physics::IScene * GetPhysicsScene();
		    const physics::IScene * GetPhysicsScene() const;

			void Update( const RenderInfo & renderInfo, core::IInput & input );
		    void UpdatePhysics();
		    RenderInfo & GetRenderInfo();
		    void Render();
		    Object::shared_ptr FindObject( const std::string & name );
    
			void SetCamera( const std::string & name );
            dxi::scene::Camera & GetCamera();
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

            void SetVisible( bool visible );
            bool GetVisible() const;

            void SetFocus( bool focus );
            bool HasFocus() const;
            bool CantLoseFocus() const;

		    void SetRenderPhysics( bool renderPhysics );
		    bool GetRenderPhysics() const;
    		
            Object::shared_ptr GetObjectOver() const;
            void SetObjectOver( Object::shared_ptr objectOver );

	    private:
			Object::shared_ptr m_root;
            std::string m_cameraName;
		    Camera m_camera;
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
            bool m_enabled; // Updates
            bool m_visible; // Renders

            Object::shared_ptr m_objectOver;
            float m_mouseDownTimeLimit;
            float m_mouseDownTime;
            bool m_mouseDrag;
	    };
    }
}
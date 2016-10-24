// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/core/IGame.h>
#include <dxi/scene/ISceneComponent.h>
#include <dxi/scene/Object.h>
#include <dxi/scene/CameraComponent.h>
#include <dxi/Viewport.h>
#include <unify/MinMax.h>
#include <list>
#include <memory>

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
        class Scene
	    {
	    public:
		    typedef std::shared_ptr< Scene > ptr;

		    Scene( core::IGame * game );
		    virtual ~Scene();

			Object::ptr GetRoot();
			const Object::ptr GetRoot() const;

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

            // Order amongst other scenes...
            void SetOrder( float order );
            float GetOrder() const;

            void SetEnabled( bool enabled );
            bool GetEnabled() const;

            void SetFocus( bool focus );
            bool HasFocus() const;
            bool CantLoseFocus() const;

            Object::ptr GetObjectOver() const;
            void SetObjectOver( Object::ptr objectOver );

			int ComponentCount() const;
			void AddComponent( ISceneComponent::ptr component );
			void RemoveComponent( ISceneComponent::ptr component );
			ISceneComponent::ptr GetComponent( int index );
			ISceneComponent::ptr GetComponent( std::string name, int startIndex = 0 );
			int FindComponent( std::string name, int startIndex = 0 ) const;

	    private:
			core::IGame * m_game;

			std::list< ISceneComponent::ptr > m_components;

			Object::ptr m_root;
			bool m_inited;
			bool m_started;

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
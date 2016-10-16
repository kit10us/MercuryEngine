// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Component.h>
#include <unify/Matrix.h>

namespace dxi
{
    namespace scene
    {
	    class Camera : public Component
	    {
	    public:
		    Camera( core::IOS * os );
		    virtual ~Camera();

			int GetRenderer() const;
			void SetRenderer( int index );

			/// <summary>
			/// Set a projection matrix to use with this camera.
			/// </summary>
			void SetProjection( const unify::Matrix & projection );
			
			unify::Matrix GetProjection() const;

			void OnInit( Object * object ) override;
			void OnStart( Object * object ) override;
			void Update( const RenderInfo & renderInfo ) override;
			void Render( const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResune() override;

	    protected:
			unify::Matrix m_projection;
			int m_renderer;
	    };
    }
}
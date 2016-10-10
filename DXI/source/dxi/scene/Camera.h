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
		    Camera();
		    virtual ~Camera();

			int GetRenderer() const;
			void SetRenderer( int index );

			/// <summary>
			/// Set a projection matrix to use with this camera.
			/// </summary>
			void SetProjection( const unify::Matrix & projection );
			
			unify::Matrix GetProjection() const;

			void OnStart() override;
			void Update( const RenderInfo & renderInfo, core::IInput & input ) override;
			void Render( const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResune() override;

	    protected:
			unify::Matrix m_projection;
			int m_renderer;
	    };
    }
}
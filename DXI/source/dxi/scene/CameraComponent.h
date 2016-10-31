// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/ObjectComponent.h>
#include <unify/Matrix.h>

namespace dxi
{
    namespace scene
    {
	    class CameraComponent : public ObjectComponent
	    {
		protected:
			CameraComponent( CameraComponent & component );

	    public:
			CameraComponent( core::IOS * os );
		    virtual ~CameraComponent();

			std::string GetName() const override;

			int GetRenderer() const;
			void SetRenderer( int index );

			/// <summary>
			/// Set a projection matrix to use with this camera.
			/// </summary>
			void SetProjection( const unify::Matrix & projection );
			
			unify::Matrix GetProjection() const;

			IObjectComponent * Duplicate() override;

	    protected:
			unify::Matrix m_projection;
			int m_renderer;
	    };
    }
}
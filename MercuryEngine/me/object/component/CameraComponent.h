// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/component/ObjectComponent.h>
#include <unify/Matrix.h>

namespace me
{
	namespace object
	{
		namespace component
		{
			class CameraComponent : public ObjectComponent
			{
			protected:
				CameraComponent( CameraComponent & component );

			public:
				CameraComponent();
				virtual ~CameraComponent();

				std::string GetWhat() const override;

				int GetRenderer() const;
				void SetRenderer( int index );

				/// <summary>
				/// Set a projection matrix to use with this camera.
				/// </summary>
				void SetProjection( const unify::Matrix & projection );

				unify::Matrix GetProjection() const;

				IObjectComponent::ptr Duplicate() override;

			protected:
				unify::Matrix m_projection;
				int m_renderer;
			};
		}
	}
}
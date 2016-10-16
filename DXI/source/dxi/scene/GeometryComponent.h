// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Component.h>
#include <dxi/Geometry.h>
#include <unify/Matrix.h>

namespace dxi
{
    namespace scene
    {
	    class GeometryComponent : public Component
	    {
	    public:
			GeometryComponent( core::IOS * os );
			GeometryComponent( core::IOS * os, Geometry::ptr geometry );
			virtual ~GeometryComponent();

			void SetGeometry( Geometry::ptr geometry );
			Geometry::ptr GetGeometry();
			const Geometry::ptr GetGeometry() const;

			/// <summary>
			/// Set a projection matrix to use with this camera.
			/// </summary>
			void SetModelMatrix( const unify::Matrix & projection );
			
			unify::Matrix & GetModelMatrix();
			const unify::Matrix & GetModelMatrix() const;

			void OnInit( Object * object ) override;
			void OnStart( Object * object ) override;
			void Update( const RenderInfo & renderInfo ) override;
			void Render( const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResune() override;

	    protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
			unify::Matrix m_modelMatrix;
	    };
    }
}
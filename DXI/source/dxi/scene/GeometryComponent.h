// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/ObjectComponent.h>
#include <dxi/Geometry.h>
#include <unify/Matrix.h>

namespace dxi
{
    namespace scene
    {
	    class GeometryComponent : public ObjectComponent
	    {
		protected:
			GeometryComponent( GeometryComponent & component );

	    public:
			GeometryComponent( core::IOS * os );
			GeometryComponent( core::IOS * os, Geometry::ptr geometry );
			virtual ~GeometryComponent();

			std::string GetName() const override;

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
			void OnUpdate( Object * object, const RenderInfo & renderInfo ) override;
			void OnRender( Object * object, const RenderInfo & renderInfo ) override;
			void OnSuspend( Object * object ) override;
			void OnResume( Object * object ) override;

			IObjectComponent * Duplicate();

	    protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
			unify::Matrix m_modelMatrix;
	    };
    }
}
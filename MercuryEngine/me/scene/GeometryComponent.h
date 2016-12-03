// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ObjectComponent.h>
#include <me/Geometry.h>
#include <unify/Matrix.h>

namespace me
{
    namespace scene
    {
	    class GeometryComponent : public ObjectComponent
	    {
		protected:
			GeometryComponent( GeometryComponent & component );

	    public:
			GeometryComponent( IOS * os );
			GeometryComponent( IOS * os, Geometry::ptr geometry );
			virtual ~GeometryComponent();

			std::string GetName() const override;

			bool Renderable() const { return true; }

			void SetGeometry( Geometry::ptr geometry );
			Geometry::ptr GetGeometry();
			const Geometry::ptr GetGeometry() const;

			/// <summary>
			/// Set a projection matrix to use with this camera.
			/// </summary>
			void SetModelMatrix( const unify::Matrix & projection );
			
			unify::Matrix & GetMatrix();
			const unify::Matrix & GetMatrix() const;

			void OnInit( Object * object ) override;
			void OnStart( Object * object ) override;
			void OnUpdate( Object * object, IRenderer * renderer, const RenderInfo & renderInfo ) override;
			
			void OnRender( Object * object, IRenderer * renderer, const RenderInfo & renderInfo, GeometryCache & cache, const unify::FrameLite * transform ) override;
			
			void OnSuspend( Object * object ) override;
			void OnResume( Object * object ) override;

			IObjectComponent * Duplicate();

	    protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
			unify::Matrix m_matrix;
	    };
    }

	scene::GeometryComponent * AddGeometryComponent( scene::Object * object, IOS * os, Geometry::ptr geometry );
}
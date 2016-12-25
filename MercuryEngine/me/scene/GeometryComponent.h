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

			void OnInit() override;
			void OnStart() override;
			void OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo ) override;
			
			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform ) override;
			
			void OnSuspend() override;
			void OnResume() override;

			IObjectComponent * Duplicate();

	    protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
	    };
    }

	scene::GeometryComponent * AddGeometryComponent( scene::Object * object, IOS * os, Geometry::ptr geometry );
}
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
			GeometryComponent();
			GeometryComponent( Geometry::ptr geometry );
			virtual ~GeometryComponent();

			std::string GetWhat() const override;

			bool Renderable() const override { return true; }

			void SetGeometry( Geometry::ptr geometry );
			Geometry::ptr GetGeometry();
			const Geometry::ptr GetGeometry() const;

			void OnInit() override;
			void OnStart() override;
			void OnUpdate( UpdateParams params ) override;
			
			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform ) override;
			
			void OnSuspend() override;
			void OnResume() override;

			IObjectComponent * Duplicate() override;

	    protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
	    };
    }

	scene::GeometryComponent * AddGeometryComponent( scene::Object * object, Geometry::ptr geometry );
}
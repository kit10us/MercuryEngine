// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/ObjectComponent.h>
#include <me/Geometry.h>
#include <unify/Matrix.h>

namespace me
{
    namespace object
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

			void OnStart() override;
			void OnUpdate( UpdateParams params ) override;

			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform ) override;

			void OnSuspend() override;
			void OnResume() override;

			IObjectComponent::ptr Duplicate() override;

		protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
		};
    }

	object::GeometryComponent * AddGeometryComponent( object::Object * object, Geometry::ptr geometry );
}
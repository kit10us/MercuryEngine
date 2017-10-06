// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/ObjectComponent.h>
#include <me/render/Geometry.h>
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
			GeometryComponent( render::Geometry::ptr geometry );
			GeometryComponent( render::Geometry::ptr geometry, unify::Matrix modelMatrix );
			virtual ~GeometryComponent();

			void SetGeometry( render::Geometry::ptr geometry );
			render::Geometry::ptr GetGeometry();
			const render::Geometry::ptr GetGeometry() const;

			void OnStart() override;
			void OnUpdate( UpdateParams params ) override;

			void CollectGeometry( render::GeometryCache & solids, render::GeometryCache & trans, const unify::FrameLite * transform ) override;
			
			void SetMatrix( unify::Matrix matrix );
			unify::Matrix GetMatrix() const;

		public: // IObjectComponent...
			IObjectComponent::ptr Duplicate() override;

		public: // IComponent...

		public: // IThing...

		private:
			render::Geometry::ptr m_geometry;
			render::GeometryInstanceData::ptr m_geometryInstanceData;
		};
    }

	object::GeometryComponent * AddGeometryComponent( object::Object * object, render::Geometry::ptr geometry, unify::Matrix matrix = unify::MatrixIdentity() );
}
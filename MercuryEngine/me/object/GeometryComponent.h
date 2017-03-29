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
			GeometryComponent( Geometry::ptr geometry, unify::Matrix matrix );
			virtual ~GeometryComponent();

			void SetGeometry( Geometry::ptr geometry );
			Geometry::ptr GetGeometry();
			const Geometry::ptr GetGeometry() const;

			void OnStart() override;
			void OnUpdate( UpdateParams params ) override;

			void CollectGeometry( GeometryCache & solids, GeometryCache & trans, const unify::FrameLite * transform ) override;
			
			void SetMatrix( unify::Matrix matrix );
			unify::Matrix GetMatrix() const;

		public: // IObjectComponent...
			IObjectComponent::ptr Duplicate() override;

		public: // IComponent...

		public: // IThing...

		private:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
		};
    }

	object::GeometryComponent * AddGeometryComponent( object::Object * object, Geometry::ptr geometry, unify::Matrix matrix = unify::MatrixIdentity() );
}
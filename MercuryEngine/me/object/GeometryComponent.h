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

			void SetGeometry( Geometry::ptr geometry );
			Geometry::ptr GetGeometry();
			const Geometry::ptr GetGeometry() const;

			void OnStart() override;
			void OnUpdate( UpdateParams params ) override;

			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform ) override;

			void OnSuspend() override;
			void OnResume() override;

		public: // IObjectComponent...
			IObjectComponent::ptr Duplicate() override;

		public: // IComponent...
			int GetValueCount() const override;
			bool ValueExists( std::string ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			std::string GetValue( std::string name ) const override;
			bool SetValue( int index, std::string value ) override;
			bool SetValue( std::string name, std::string value ) override;

		public: // IThing...
			std::string GetWhat() const override;

		protected:
			Geometry::ptr m_geometry;
			GeometryInstanceData::ptr m_geometryInstanceData;
		};
    }

	object::GeometryComponent * AddGeometryComponent( object::Object * object, Geometry::ptr geometry );
}
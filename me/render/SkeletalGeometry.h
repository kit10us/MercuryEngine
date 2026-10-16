// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

///<summary>
/// Geometry for an object
///
/// An object would have geometry to be rendered, as it's apearance. Presently other
/// objects inherit from Geometry that intend to render. Geometry doesn't contain a
/// location, that's held in the object. Geometry can be safely referenced by more than
/// on object.
///</summary>

#pragma once

#include <me/render/GeometryInstanceData.h>
#include <me/UpdateParams.h>
#include <me/render/RenderParams.h>
#include <me/render/MatrixFeed.h>
#include <me/render/Geometry.h>
#include <me/render/Skeleton.h>
#include <rm/IResource.h>
#include <unify/Unify.h>
#include <unify/BSphere.h>
#include <unify/BBox.h>
#include <unify/FrameLite.h>
#include <memory>

namespace me
{
	namespace render
	{
		/// <summary>
		// Abstract graphical object.
		class SkeletalGeometry : public Geometry
		{
		public:
			SkeletalGeometry() = default;
			virtual ~SkeletalGeometry() = default;

		public: // Geometry

			GeometryInstanceData::ptr CreateInstanceData() const override;

			void Update( const UpdateParams & params, GeometryInstanceData * instanceData ) override;

			void Render( const Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed ) override;

			bool IsTrans() const override;

		public: // rm::IResource
			std::string GetSource() const override {return "";}

		private:
			Skeleton::ptr m_skeleton;
			Geometry::ptr m_geometry;
		};
	}
}
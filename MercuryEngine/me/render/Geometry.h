// Copyright (c) 2002 - 2018, Evil Quail LLC
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
		class Geometry : public rm::IResource
		{
		public:
			typedef std::shared_ptr< Geometry > ptr;

			Geometry();
			virtual ~Geometry() {}

			virtual GeometryInstanceData * CreateInstanceData();

			virtual void Update( const UpdateParams & params, GeometryInstanceData * instanceData ) = 0;
			virtual void Render( const Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed ) = 0;

			unify::BBox< float > & GetBBox();
			const unify::BBox< float > & GetBBox() const;

			virtual unify::Path GetSource() const = 0;

			virtual bool IsTrans() const = 0;
		protected:
			unify::BBox< float > m_BBox;
		};
	}
}
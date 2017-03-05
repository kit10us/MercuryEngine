// Copyright (c) 2002 - 2013, Quentin S. Smith
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

#include <me/GeometryInstanceData.h>
#include <me/UpdateParams.h>
#include <me/RenderParams.h>
#include <me/MatrixFeed.h>
#include <unify/Unify.h>
#include <unify/BSphere.h>
#include <unify/BBox.h>
#include <unify/FrameLite.h>
#include <memory>

namespace me
{
	// Abstract graphical object.
	class Geometry
	{
	public:
		typedef std::shared_ptr< Geometry > ptr;

		Geometry();
		virtual ~Geometry() {}

		virtual GeometryInstanceData * CreateInstanceData();

		virtual void Update( UpdateParams params, GeometryInstanceData * instanceData ) = 0;
		virtual void Render( RenderParams params, GeometryInstanceData * instanceData, MatrixFeed & matrixFeed ) = 0;

		unify::BBox< float > & GetBBox();
		const unify::BBox< float > & GetBBox() const;
	protected:
		unify::BBox< float > m_BBox;
	};
}
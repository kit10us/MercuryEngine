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

#include <dxi/GeometryInstanceData.h>
#include <dxi/PrimitiveList.h>
#include <dxi/RenderInfo.h>
#include <dxi/scene/ObjectComponent.h>
#include <unify/Unify.h>
#include <unify/BSphere.h>
#include <unify/BBox.h>
#include <memory>

namespace dxi
{
	// Abstract graphical object.
	class Geometry
	{
	public:
		typedef std::shared_ptr< Geometry > ptr;

		Geometry();
		virtual ~Geometry() {}

		virtual GeometryInstanceData * CreateInstanceData();
		virtual void Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData ) = 0;
		virtual void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData ) = 0;

		unify::BBox< float > & GetBBox();
		const unify::BBox< float > & GetBBox() const;
	protected:
		unify::BBox< float > m_BBox;
	};
}
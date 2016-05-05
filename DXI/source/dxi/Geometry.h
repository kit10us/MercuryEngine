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
		typedef std::shared_ptr< Geometry > shared_ptr;
		typedef std::weak_ptr< Geometry > weak_ptr;

		Geometry();
		virtual ~Geometry() {}

		void Create();
		void Destroy();
		void Validate();
		void Invalidate();

		virtual GeometryInstanceData * CreateInstanceData();
		virtual void Update( unify::Seconds elapsed, GeometryInstanceData * instanceData ) = 0;
		virtual void Render( RenderInfo & renderInfo, GeometryInstanceData * instanceData ) = 0;
		const unify::BSphere & GetBSphere() const;
		const unify::BBox< float > & GetBBox() const;
		void SetBounds( const unify::BBox< float > & bbox );
		virtual const unify::BBox< float > & ComputeBounds();

	protected:
		bool m_created;
		bool m_valid;
		unify::BSphere m_BSphere;
		unify::BBox< float > m_BBox;
	};
}
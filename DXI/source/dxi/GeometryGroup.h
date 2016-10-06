// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A grouping of geometry. Allows grouping geometry, one geometry to hold many others.
/// GeometryGroup--->Geometry
///</summary>

#pragma once

#include <dxi/Geometry.h>

namespace dxi
{
	class GeometryGroup : public Geometry
	{
	public:
		GeometryGroup();
		~GeometryGroup();

		virtual void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );
		virtual void Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );
		virtual void Add( Geometry::shared_ptr geometry );
		virtual void Clear();

	protected:
		virtual const unify::BBox< float > & ComputeBounds();

		std::vector< Geometry::shared_ptr > m_geometryList;
	};
}
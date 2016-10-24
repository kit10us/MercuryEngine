// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A mesh geometry.
///	Supported file types: x, xml, ase
/// Supports Shape creation methods.
///</summary>

#pragma once

#include <dxi/Geometry.h>
#include <dxi/RenderMethod.h>
#include <dxi/RenderInfo.h>
#include <dxi/MeshInstanceData.h>
#include <dxi/scene/ObjectComponent.h>
#include <dxi/PrimitiveList.h>
#include <unify/unify.h>

namespace dxi
{
	class Mesh : public Geometry
	{
	public:
		Mesh( core::IRenderer * renderer );
		~Mesh();

		GeometryInstanceData * CreateInstanceData();

		void Destroy();

		// Overrides..
		void Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );
		void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );

		const unify::BBox< float > & ComputeBounds();
		PrimitiveList & GetPrimitiveList();

	private:
		PrimitiveList m_primitiveList;
	};
}
// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A mesh geometry.
///	Supported file types: x, xml, ase
/// Supports Shape creation methods.
///</summary>

#pragma once

#include <me/Geometry.h>
#include <me/RenderMethod.h>
#include <me/RenderInfo.h>
#include <me/MeshInstanceData.h>
#include <me/ObjectComponent.h>
#include <me/PrimitiveList.h>
#include <unify/unify.h>

namespace me
{
	class Mesh : public Geometry
	{
	public:
		Mesh( IRenderer * renderer );
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
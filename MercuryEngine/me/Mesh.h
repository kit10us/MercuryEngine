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
#include <me/object/ObjectComponent.h>
#include <me/PrimitiveList.h>
#include <unify/unify.h>

namespace me
{
	class Mesh : public Geometry
	{
	public:
		Mesh( const IRenderer * renderer );
		Mesh( unify::Path source, IRenderer * renderer );
		~Mesh();

		GeometryInstanceData * CreateInstanceData();

		void Destroy();

		void Update( UpdateParams params, GeometryInstanceData * instanceData ) override;

		void Render( RenderParams params, GeometryInstanceData * instanceData, MatrixFeed & matrixFeed ) override;

		std::string GetSource() const override;

		const unify::BBox< float > & ComputeBounds();
		PrimitiveList & GetPrimitiveList();

	private:
		unify::Path m_source;
		PrimitiveList m_primitiveList;
	};
}
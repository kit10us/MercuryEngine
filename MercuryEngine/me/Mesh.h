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
#include <me/RenderInstance.h>
#include <me/MeshInstanceData.h>
#include <me/scene/ObjectComponent.h>
#include <me/PrimitiveList.h>
#include <unify/unify.h>

namespace me
{
	class Mesh : public Geometry
	{
	public:
		Mesh( const IRenderer * renderer );
		~Mesh();

		GeometryInstanceData * CreateInstanceData();

		void Destroy();

		void Update( IRenderer * renderer, const RenderInfo & renderInfo, GeometryInstanceData * instanceData ) override;

		void Render( IRenderer * renderer, const RenderInfo & renderInfo, GeometryInstanceData * instanceData, const unify::FrameLite ** instances, const size_t instances_size ) override;

		void Render( IRenderer * renderer, const RenderInfo & renderInfo, GeometryInstanceData * instanceData, const InstancesSet * instancesList, const size_t instancesList_size ) override;

		const unify::BBox< float > & ComputeBounds();
		PrimitiveList & GetPrimitiveList();

	private:
		PrimitiveList m_primitiveList;
	};
}
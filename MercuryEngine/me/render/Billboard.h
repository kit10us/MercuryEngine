// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A mesh geometry.
///	Supported file types: x, xml, ase
/// Supports Shape creation methods.
///</summary>

#pragma once

#include <me/render/Geometry.h>
#include <me/render/RenderMethod.h>
#include <me/render/RenderInfo.h>
#include <me/render/MeshInstanceData.h>
#include <me/render/PrimitiveList.h>
#include <me/render/Skeleton.h>
#include <unify/unify.h>

namespace me
{
	namespace render
	{
		class Billboard : public Geometry
		{
		public:
			Billboard( IRenderer * renderer, float size, Effect::ptr effect );
			~Billboard();

			void Destroy();

			const unify::BBox< float > & ComputeBounds();

		public: // Geometry...
			GeometryInstanceData * CreateInstanceData() override;

			void Update( const UpdateParams & params, GeometryInstanceData * instanceData ) override;

			void Render( const render::Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed ) override;

			bool IsTrans() const override;

			unify::Path GetSource() const override;

		public: // IResource...
			bool Reload() override;

		private:
			float m_size;
			PrimitiveList m_primitiveList;
		};
	}
}
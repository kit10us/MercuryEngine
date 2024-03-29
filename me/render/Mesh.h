// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
		class Mesh : public Geometry
		{
		public:
			Mesh( IRenderer * renderer );
			Mesh( std::string source, IRenderer * renderer );
			~Mesh();

			void Destroy();

			const unify::BBox< float > & ComputeBounds();

			PrimitiveList & GetPrimitiveList();

			Skeleton * GetSkeleton();

			const Skeleton * GetSkeleton() const;

			void SetSkeletonEffect( Effect::ptr effect );

			/// <summary>
			/// For testing purposes.
			/// </summary>
			void SetRenderMesh( bool render );

		public: // Geometry...
			GeometryInstanceData::ptr CreateInstanceData() const override;

			void Update( const UpdateParams & params, GeometryInstanceData * instanceData ) override;

			void Render( const render::Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed ) override;

			std::string GetSource() const override;

			bool IsTrans() const override;

		public: // IResource...
			bool Reload() override;

		private:
			std::string m_source;
			PrimitiveList m_primitiveList;
			Skeleton m_skeleton;
			Effect::ptr m_skeletonEffect;
			frameanimation::AnimationSet::ptr m_animations;
			bool m_renderMesh;
		};
	}
}
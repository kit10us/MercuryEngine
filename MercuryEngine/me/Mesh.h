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
#include <me/PrimitiveList.h>
#include <me/Skeleton.h>
#include <unify/unify.h>

namespace me
{
	class Mesh : public Geometry
	{
	public:
		Mesh( IRenderer * renderer );
		Mesh( unify::Path source, IRenderer * renderer );
		~Mesh();

		void Destroy();

		const unify::BBox< float > & ComputeBounds();

		PrimitiveList & GetPrimitiveList();

		Skeleton * GetSkeleton();

		const Skeleton * GetSkeleton() const;

		void SetSkeletonEffect( Effect::ptr effect );

	public: // Geometry...
		GeometryInstanceData * CreateInstanceData() override;

		void Update( UpdateParams params, GeometryInstanceData * instanceData ) override;

		void Render( RenderParams params, GeometryInstanceData * instanceData, MatrixFeed & matrixFeed ) override;

		unify::Path GetSource() const override;

		bool IsTrans() const override;

	public: // IResource...
		bool Reload() override;

	private:
		unify::Path m_source;
		PrimitiveList m_primitiveList;
		Skeleton m_skeleton;
		Effect::ptr m_skeletonEffect;
		frameanimation::AnimationSet::ptr m_animations;
	};
}
// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

///<summary>
///</summary>

#pragma once

#include <me/render/RenderParams.h>
#include <me/render/Geometry.h>
#include <me/render/Effect.h>
#include <unify/Color.h>
#include <unify/unify.h>
#include <unify/Matrix.h>
#include <vector>
#include <map>
#include <memory>
#include <limits>

namespace me
{
	namespace render
	{
		class Skeleton
		{
		public:
			typedef std::shared_ptr< Skeleton > ptr;

			static const size_t Root{ std::numeric_limits< size_t >::max() };

			Skeleton();
			~Skeleton();

			void AddJoint( std::string name, unify::Matrix matrix, std::string parent = std::string() );
			size_t FindJointIndex( std::string name ) const;
			const unify::Matrix * FindJoint( std::string name ) const;

			bool Empty() const;

			void Render( render::Params params, render::MatrixFeed & matrixFeed, Effect::ptr effect, float size, unify::Color color );

		private:
			std::vector< unify::Matrix > m_jointMatrix;
			std::vector< unify::Matrix > m_jointFinalMatrix;
			std::vector< size_t > m_parent;
			std::map< std::string, size_t > m_names;

			Geometry::ptr m_jointGeo;
		};
	}
}
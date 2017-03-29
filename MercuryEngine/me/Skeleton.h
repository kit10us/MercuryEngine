// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
///</summary>

#pragma once

#include <me/RenderParams.h>
#include <me/Geometry.h>
#include <me/Effect.h>
#include <unify/unify.h>
#include <unify/Matrix.h>
#include <vector>
#include <map>
#include <memory>
#include <limits>

namespace me
{
	class Skeleton
	{
	public:
		typedef std::shared_ptr< Skeleton > ptr;

		static const size_t Root{ std::numeric_limits< size_t >::max() };

		Skeleton();
		~Skeleton();
		
		void AddJoint( std::string name, unify::Matrix matrix, std::string parent = std::string() );
		const unify::Matrix * FindJoint( std::string name ) const;

		bool Empty() const;

		void Render( RenderParams params, render::MatrixFeed & matrixFeed, Effect::ptr effect, float size, unify::Color color );

	protected:
		void Render( RenderParams params, unify::Matrix rootMatrix, size_t index, me::Geometry::ptr geometry );

		size_t FindJointIndex( std::string name ) const;

	private:
		std::vector< unify::Matrix > m_jointMatrix;
		std::vector< unify::Matrix > m_jointFinalMatrix;
		std::vector< size_t > m_parent;
		std::map< std::string, size_t > m_names;

		me::Geometry::ptr m_jointGeo;
	};
}
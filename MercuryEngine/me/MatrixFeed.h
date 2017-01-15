// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
#include <unify/FrameLite.h>

namespace me
{			
	class IMatrixSource
	{
	public:
		virtual ~IMatrixSource() {}

		virtual size_t Count() const = 0;
		virtual unify::Matrix GetMatrix( size_t i ) const = 0;
		virtual void CopyMatrices( unify::Matrix * matrices ) const = 0;
	};

	struct InstancesSet
	{
		const unify::FrameLite ** instances;
		const size_t instances_size;
	};		

	class MatrixFeed
	{
		const InstancesSet * _instancesList;
		const unify::Matrix * _matrices;
		const unify::FrameLite ** _instances;
		IMatrixSource * const * _sources;

		// Shared variables...
		const size_t _major_size;
		size_t _major_index;
		size_t _minor_index;
		size_t _matricesPerInstance; // guarantees multiples of _stride (for example, with animations that require a specific amount of matrices).

	public:
		MatrixFeed();
		MatrixFeed( const InstancesSet * instancesList, size_t instancesList_size, size_t matricesPerInstance = 1 );
		MatrixFeed( const unify::Matrix * matrices, size_t matrices_count, size_t matricesPerInstance = 1 );
		MatrixFeed( const unify::FrameLite ** instances, size_t instances_size, size_t matricesPerInstance = 1 );
		MatrixFeed( IMatrixSource * const * sources, size_t sources_size, size_t matricesPerInstance = 1 );

		size_t Consume( unify::Matrix * out, size_t max );
		size_t MatricesPerInstance() const;

		bool Done() const;

		void Restart();
	};
}
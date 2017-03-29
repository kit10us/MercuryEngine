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
		enum class Phase
		{
			InstancesSetPhase,
			MatrixPointerPhase,
			FramePointerPhase,
			IMatrixSourcePhase,
			MatrixArrayPhase,
			Done
		} _phase;

		// Phase 1
		const InstancesSet * _instancesList;
		size_t _instancesList_size;

		// Phase 2
		const unify::Matrix * _matrices;
		size_t _matrices_size;

		// Phase 3
		const unify::FrameLite ** _instances;
		size_t _instances_size;

		// Phase 4
		IMatrixSource * const * _sources;
		size_t _sources_size;

		// Shared variables...
		size_t _major_index;
		size_t _minor_index;
		size_t _stride; // guarantees multiples of _stride (for example, with animations that require a specific amount of matrices).


	public:
		MatrixFeed();
		MatrixFeed( const InstancesSet * instancesList, size_t instancesList_size, size_t matricesPerInstance = 1 );
		MatrixFeed( const unify::Matrix * matrices, size_t matrices_count, size_t matricesPerInstance = 1 );
		MatrixFeed( const unify::FrameLite ** instances, size_t instances_size, size_t matricesPerInstance = 1 );
		MatrixFeed( IMatrixSource * const * sources, size_t sources_size, size_t matricesPerInstance = 1 );

		/// <summary>
		/// Consume MAX amount of matrics at a time.
		/// </summary>
		size_t Consume( unify::Matrix * out, size_t max );
		size_t Stride() const;

		bool Done() const;

		void Restart();
	};
}
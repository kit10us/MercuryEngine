// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/MatrixFood.h>

namespace me
{			
	namespace render
	{
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
			} m_phase;

			size_t m_stride; // guarantees multiples of _stride (for example, with animations that require a specific amount of matrices).
			size_t m_major_index;
			size_t m_minor_index;
							 
			// Phase 1
			MatrixFood_InstancesSet m_instancesSet_food;

			// Phase 2
			MatrixFood_Matrices m_matrices_food;

			// Phase 3
			MatrixFood_Frames m_frames_food;

			// Phase 4
			MatrixFood_IMatrixSource m_matrixsource_food;

		public:
			MatrixFeed();
			MatrixFeed( MatrixFood_InstancesSet food, size_t stride = 1 );
			MatrixFeed( MatrixFood_Matrices food, size_t stride = 1 );
			MatrixFeed( MatrixFood_Frames food, size_t stride = 1 );
			MatrixFeed( MatrixFood_IMatrixSource food, size_t stride = 1 );

			/// <summary>
			/// Consume MAX amount of matrics at a time.
			/// </summary>
			size_t Consume( unify::Matrix * out, size_t max );
			size_t Stride() const;

			bool Done() const;

			void Restart();
		};
	}
}
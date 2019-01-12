// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/MatrixFood.h>

namespace me
{			
	namespace render
	{
		class MatrixFeed
		{
			const MatrixFoodBase * m_food;

		public:
			MatrixFeed( const MatrixFoodBase & food, size_t stride );

			/// <summary>
			/// Consume max amount of matrics at a time.
			/// </summary>
			size_t Consume( unify::Matrix * out, size_t max );

			/// <summary>
			/// The stride between each set of matrices. Basically it says how many matrics go into
			/// one chunk. For example, rendered geometry requires this much matrices.
			/// </summary>
			size_t Stride() const;
			
			/// <summary>
			/// Returns true when we have consumed all matrices.
			/// </summary>
			bool Done() const;

			/// <summary>
			/// Restrart the consumption from the beginning.
			/// </summary>
			void Restart();

		private:
			bool m_done;
			size_t m_stride;
			size_t m_major_index;
			size_t m_minor_index;
		};
	}
}
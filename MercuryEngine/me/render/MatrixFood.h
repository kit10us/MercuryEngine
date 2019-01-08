// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
#include <unify/FrameLite.h>
#include <me/render/FrameAndMatrix.h>

namespace me
{
	namespace render
	{
		/// <summary>
		/// An abstract matrix source, so that we can consume from anywhere/thing.
		/// </summary>
		class IMatrixSource
		{
		public:
			virtual ~IMatrixSource() {}

			virtual size_t Count() const = 0;
			virtual unify::Matrix GetMatrix( size_t i ) const = 0;
			virtual void CopyMatrices( unify::Matrix * matrices ) const = 0;
		};

		class MatrixFoodBase
		{
		protected:
			MatrixFoodBase( size_t size );

		public:
			/// <description>
			/// Read a matrix from our source, returning the number of matrices read.
			/// </description>
			virtual void ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const = 0;

			/// <description>
			///
			/// </description>
			size_t GetSize() const;

		private:
			size_t m_size;
		};
		
		struct MatrixFood_InstancesSet : public MatrixFoodBase
		{
			MatrixFood_InstancesSet( const InstancesSet * instancesList, size_t size );

			void ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const override;

			const InstancesSet * instancesList;
		};

		struct MatrixFood_Matrices : public MatrixFoodBase
		{
			MatrixFood_Matrices( const unify::Matrix * matrices, size_t size );

			void ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const override;

			const unify::Matrix * matrices;
		};

		struct MatrixFood_Frames : public MatrixFoodBase
		{
			MatrixFood_Frames( const unify::FrameLite ** instances, size_t size );

			void ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const override;
			
			const unify::FrameLite ** instances;
		};

		struct MatrixFood_IMatrixSource : public MatrixFoodBase
		{
			MatrixFood_IMatrixSource( IMatrixSource * const * sources, size_t size );

			void ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const override;

			IMatrixSource * const * sources;
		};
	}
}
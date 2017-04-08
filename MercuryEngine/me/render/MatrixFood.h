// Copyright (c) 2002 - 2018, Quentin S. Smith
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
		
		struct MatrixFood_InstancesSet
		{
			MatrixFood_InstancesSet( const InstancesSet * instancesList = 0, size_t size = 0 )
				: instancesList{ instancesList }
				, size{ size }
				{
				}

			const InstancesSet * instancesList;
			size_t size;
		};

		struct MatrixFood_Matrices
		{
			MatrixFood_Matrices( const unify::Matrix * matrices = 0,	size_t size = 0 )
				: matrices{ matrices }
				, size{ size }
			{
			}

			const unify::Matrix * matrices;
			size_t size;
		};

		struct MatrixFood_Frames
		{
			MatrixFood_Frames( const unify::FrameLite ** instances = 0, size_t size = 0 )
				: instances{ instances }
				, size{ size }
			{
			}
			
			const unify::FrameLite ** instances;
			size_t size;
		};

		struct MatrixFood_IMatrixSource
		{
			MatrixFood_IMatrixSource( IMatrixSource * const * sources = 0, size_t size = 0 )
				: sources{ sources }
				, size{ size }
			{
			}

			IMatrixSource * const * sources;
			size_t size;
		};
	}
}
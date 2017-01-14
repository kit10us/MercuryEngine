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
		IMatrixSource() {}

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
		const IMatrixSource * _sources;

		// Shared variables...
		const size_t _major_size;
		size_t _major_index;
		size_t _minor_index;
	public:
		MatrixFeed( const InstancesSet * instancesList, size_t instancesList_size );
		MatrixFeed( const unify::Matrix * matrices, size_t matrices_count );
		MatrixFeed( const unify::FrameLite ** instances, const size_t instances_size );
		MatrixFeed( const IMatrixSource * sources, const size_t sources_size );

		size_t ReadMatrix( unify::Matrix * out, size_t max );

		bool Done() const;

		void Restart();
	};
}
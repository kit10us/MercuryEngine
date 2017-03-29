// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/MatrixFeed.h>

using namespace me;

MatrixFeed::MatrixFeed()
	: _phase{ Phase::InstancesSetPhase }
	, _stride{ 1 }

	, _instancesList{ 0 }
	, _instancesList_size{ 0 }

	, _matrices{ 0 }
	, _matrices_size{ 0 }

	, _instances{ 0 }
	, _instances_size{ 0 }

	, _sources{ 0 }
	, _sources_size{ 0 }


	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( const InstancesSet * instancesList, size_t instancesList_size, size_t stride )
	: _phase{ Phase::InstancesSetPhase }
	, _stride{ stride }

	, _instancesList{ instancesList }
	, _instancesList_size{ instancesList_size }

	, _matrices{ 0 }
	, _matrices_size{ 0 }

	, _instances{ 0 }
	, _instances_size{ 0 }

	, _sources{ 0 }
	, _sources_size{ 0 }


	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( const unify::Matrix * matrices, size_t matrix_count, size_t stride )
	: _phase{ Phase::InstancesSetPhase }
	, _stride{ stride }

	, _instancesList{ 0 }
	, _instancesList_size{ 0 }

	, _matrices{ matrices }
	, _matrices_size{ matrix_count }

	, _instances{ 0 }
	, _instances_size{ 0 }

	, _sources{ 0 }
	, _sources_size{ 0 }

	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( const unify::FrameLite ** instances, size_t instances_size, size_t stride )
	: _phase{ Phase::InstancesSetPhase }
	, _stride{ stride }

	, _instancesList{ 0 }
	, _instancesList_size{ 0 }

	, _matrices{ 0 }
	, _matrices_size{ 0 }

	, _instances{ instances }
	, _instances_size{ instances_size }

	, _sources{ 0 }
	, _sources_size{ 0 }

	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( IMatrixSource * const * sources, size_t sources_size, size_t stride )
	: _phase{ Phase::InstancesSetPhase }
	, _stride{ stride }

	, _instancesList{ 0 }
	, _instancesList_size{ 0 }
	
	, _matrices{ 0 }
	, _matrices_size{ 0 }
	
	, _instances{ 0 }
	, _instances_size{ 0 }
	
	, _sources{ sources }
	, _sources_size{ sources_size }
	
	, _major_index{ 0 }
	, _minor_index{ 0 }	
{
}

size_t MatrixFeed::Consume( unify::Matrix * out, size_t max )
{
	size_t read = 0;

	// Align to the amount of reads/writes we actually want.
	max = max - (max % _stride );

	while( _phase != Phase::Done )
	{
		// Determine our current phase...
		if( _phase == Phase::InstancesSetPhase && ( !_instancesList || _major_index >= _instancesList_size ) )
		{
			_major_index = 0;
			_phase = Phase::MatrixPointerPhase;
		}
		if( _phase == Phase::MatrixPointerPhase && ( !_matrices || _major_index >= _matrices_size ) )
		{
			_major_index = 0;
			_phase = Phase::FramePointerPhase;
		}
		if( _phase == Phase::FramePointerPhase && ( !_instances || _major_index >= _instances_size ) )
		{
			_major_index = 0;
			_phase = Phase::IMatrixSourcePhase;
		}
		if( _phase == Phase::IMatrixSourcePhase && ( !_sources || _major_index >= _sources_size ) )
		{
			_major_index = 0;
			_phase = Phase::Done;
		}

		switch( _phase )
		{
		case Phase::InstancesSetPhase:
			while( _major_index < _instancesList_size && read < max )
			{
				auto && instances = _instancesList[_major_index];
				instances.instances[_minor_index++]->ReadMatrix( &out[read++] );
				if( _minor_index >= instances.instances_size )
				{
					_minor_index = 0;
					_major_index++;
				}
			}
			break;
		case Phase::MatrixPointerPhase:
			while( _major_index < _matrices_size && read < max )
			{
				out[read++] = _matrices[_major_index++];
			}
			break;
		case Phase::FramePointerPhase:
			while( _major_index < _instances_size && read < max )
			{
				_instances[_major_index++]->ReadMatrix( &out[read++] );
			}
			break;
		case Phase::IMatrixSourcePhase:
			while( _major_index < _sources_size && read < max )
			{
				_sources[_major_index]->CopyMatrices( &out[read] );
				read += _sources[_major_index++]->Count();
			}
			break;
		}

		if( _phase == Phase::Done || read == max )
		{
			return read;
		}
	}

	return 0;
}

size_t MatrixFeed::Stride() const
{
	return _stride;
}

bool MatrixFeed::Done() const
{
	return _phase == Phase::Done;
}

void MatrixFeed::Restart()
{
	_phase = Phase::InstancesSetPhase;
	_major_index = 0;
	_minor_index = 0;
	_stride = 1;
}
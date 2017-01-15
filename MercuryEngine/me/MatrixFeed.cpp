// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/MatrixFeed.h>

using namespace me;

MatrixFeed::MatrixFeed()
	: _instancesList{ 0 }
	, _matrices{ 0 }
	, _instances{ 0 }
	, _sources{ 0 }
	, _major_size{ 0 }
	, _major_index{ 0 }
	, _minor_index{ 0 }
	, _matricesPerInstance{ 1 }
{
}

MatrixFeed::MatrixFeed( const InstancesSet * instancesList, size_t instancesList_size, size_t matricesPerInstance )
	: _instancesList{ instancesList }
	, _matrices{ 0 }
	, _instances{ 0 }
	, _sources{ 0 }
	, _major_size{ instancesList_size }
	, _major_index{ 0 }
	, _minor_index{ 0 }
	, _matricesPerInstance{ matricesPerInstance }
{
}

MatrixFeed::MatrixFeed( const unify::Matrix * matrices, size_t matrix_count, size_t matricesPerInstance )
	: _instancesList{ 0 }
	, _matrices{ matrices }
	, _instances{ 0 }
	, _sources{ 0 }
	, _major_size{ matrix_count }
	, _major_index{ 0 }
	, _minor_index{ 0 }
	, _matricesPerInstance{ matricesPerInstance }
{
}

MatrixFeed::MatrixFeed( const unify::FrameLite ** instances, size_t instances_size, size_t matricesPerInstance )
	: _instancesList{ 0 }
	, _matrices{ 0 }
	, _instances{ instances }
	, _sources{ 0 }
	, _major_size{ instances_size }
	, _major_index{ 0 }
	, _minor_index{ 0 }
	, _matricesPerInstance{ matricesPerInstance }
{
}

MatrixFeed::MatrixFeed( IMatrixSource * const * sources, size_t sources_size, size_t matricesPerInstance )
	: _instancesList{ 0 }
	, _matrices{ 0 }
	, _instances{ 0 }
	, _sources{ sources }
	, _major_size{ sources_size }
	, _major_index{ 0 }
	, _minor_index{ 0 }
	, _matricesPerInstance{ matricesPerInstance }
{
}

size_t MatrixFeed::Consume( unify::Matrix * out, size_t max )
{
	size_t read = 0;

	// Align to the amount of reads/writes we actually want.
	max = max - (max % _matricesPerInstance);

	if ( _instancesList )
	{
		while ( _major_index < _major_size && read < max  )
		{
			auto && instances = _instancesList[_major_index];
			instances.instances[_minor_index++]->ReadMatrix( &out[read++] );
			if ( _minor_index >= instances.instances_size )
			{
				_minor_index = 0;
				_major_index++;
			}
		}
	}
	else if ( _matrices )
	{
		while ( _major_index < _major_size && read < max )
		{
			out[read++] = _matrices[_major_index++];
		}
	}
	else if ( _instances )
	{
		while ( _major_index < _major_size && read < max )
		{
			_instances[_major_index++]->ReadMatrix( &out[read++] );
		}
	}
	else if ( _sources )
	{
		while ( _major_index < _major_size && read < max )
		{
			_sources[_major_index]->CopyMatrices( &out[read] );
			read += _sources[_major_index++]->Count();
		}
	}
	return read;
}

size_t MatrixFeed::MatricesPerInstance() const
{
	return _matricesPerInstance;
}

bool MatrixFeed::Done() const
{
	return _major_index >= _major_size;
}

void MatrixFeed::Restart()
{
	_major_index = 0;
	_minor_index = 0;
}
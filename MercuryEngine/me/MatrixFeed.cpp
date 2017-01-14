// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/MatrixFeed.h>

using namespace me;

MatrixFeed::MatrixFeed( const InstancesSet * instancesList, size_t instancesList_size )
	: _instancesList{ instancesList }
	, _matrices{ 0 }
	, _instances{ 0 }
	, _sources{ 0 }
	, _major_size{ instancesList_size }
	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( const unify::Matrix * matrices, size_t matrix_count )
	: _instancesList{ 0 }
	, _matrices{ matrices }
	, _instances{ 0 }
	, _sources{ 0 }
	, _major_size{ matrix_count }
	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( const unify::FrameLite ** instances, const size_t instances_size )
	: _instancesList{ 0 }
	, _matrices{ 0 }
	, _instances{ instances }
	, _sources{ 0 }
	, _major_size{ instances_size }
	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( const IMatrixSource * sources, const size_t sources_size )
	: _instancesList{ 0 }
	, _matrices{ 0 }
	, _instances{ 0 }
	, _sources{ sources }
	, _major_size{ sources_size }
	, _major_index{ 0 }
	, _minor_index{ 0 }
{
}

size_t MatrixFeed::ReadMatrix( unify::Matrix * out, size_t max )
{
	size_t read = 0;

	if ( _instancesList )
	{
		while ( _major_index < _major_size && read < max )
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

	return read;
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
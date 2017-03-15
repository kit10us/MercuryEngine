// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/GeometryCache.h>
						
using namespace me;

GeometryCacheSummation::GeometryCacheSummation()
	: m_count{ 0 }
{
}

void GeometryCacheSummation::Add( Geometry * geometry, InstancesSet set )
{
	m_summation[ geometry ].push_back( set );
	m_count += set.instances_size;
}

void GeometryCacheSummation::Render( RenderParams params )
{
	for( auto && pair : m_summation )
	{
		pair.first->Render( params, 0, MatrixFeed( &pair.second[0], pair.second.size(), 1 ) );
	}
}

void GeometryCacheSummation::Reset()
{
	for( auto && pair : m_summation )
	{
		pair.second.clear();
	}
	m_count = 0;
}

size_t GeometryCacheSummation::Count() const
{
	return m_count;
}
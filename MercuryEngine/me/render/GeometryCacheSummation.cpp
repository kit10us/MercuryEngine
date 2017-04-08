// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/render/GeometryCache.h>
						
using namespace me;
using namespace render;

GeometryCacheSummation::GeometryCacheSummation()
	: m_count{ 0 }
{
}

void GeometryCacheSummation::Add( Geometry * geometry, render::InstancesSet set )
{
	m_summation[ geometry ].push_back( set );
	m_count += set.size;
}

void GeometryCacheSummation::Render( render::Params params )
{
	for( auto && pair : m_summation )
	{
		pair.first->Render( params, 0, render::MatrixFeed( { &pair.second[0], pair.second.size() }, 1 ) );
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
// Copyright (c) 2002 - 2018, Evil Quail LLC
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

void GeometryCacheSummation::Render( const render::Params & params )
{
	for( auto && pair : m_summation )
	{
		render::MatrixFood_InstancesSet food( &pair.second[0], pair.second.size() );
		render::MatrixFeed feed( food, 1 );
		pair.first->Render( params, 0, feed );
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
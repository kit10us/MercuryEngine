// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/GeometryCache.h>
						
using namespace me;

GeometryCache::GeometryCache()
{
}

void GeometryCache::Add( Geometry * geometry, const unify::FrameLite * instance )
{
	m_cache[ geometry ].push_back( instance );
}

void GeometryCache::Sum( GeometryCacheSummation & summation )
{
	for( auto && set : m_cache )
	{
		summation.Add( set.first, { &set.second[0], set.second.size() } );
	}
}




GeometryCacheSummation::GeometryCacheSummation()
{
}

void GeometryCacheSummation::Add( Geometry * geometry, InstancesSet set )
{
	m_summation[ geometry ].push_back( set );
}

void GeometryCacheSummation::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	for( auto && pair : m_summation )
	{
		pair.first->Render( renderer, renderInfo, 0, MatrixFeed( &pair.second[0], pair.second.size(), 1 ) );
	}
}

void GeometryCacheSummation::Reset()
{
	for( auto && pair : m_summation )
	{
		pair.second.clear();
	}
}

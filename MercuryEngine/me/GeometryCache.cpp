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

void GeometryCache::Reset()
{
	m_cache.clear();
}

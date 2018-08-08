// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/GeometryCache.h>
						
using namespace me;
using namespace render;

GeometryCache::GeometryCache()
{
}

void GeometryCache::Add( Geometry * geometry, FrameAndMatrix fm )
{
	assert( geometry );
	assert( fm.frame );

	// Push the FM into a vector for the associate geometry.
	m_cache[ geometry ].push_back( fm );
}

void GeometryCache::Sum( GeometryCacheSummation & summation )
{
	for( auto && set : m_cache )
	{
		Geometry*  geo = set.first;
		const FrameAndMatrix * first_fm_ptr = &set.second[0];
		size_t size = set.second.size();
		summation.Add( geo, { first_fm_ptr, size} );
	}
}

void GeometryCache::Reset()
{
	m_cache.clear();
}

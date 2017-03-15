// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/GeometryCacheSummation.h>
#include <me/Geometry.h>
#include <me/RenderParams.h>
#include <unify/FrameLite.h>
						
namespace me
{
	class GeometryCache
	{
	public:
		GeometryCache();
		void Add( Geometry * geometry, const unify::FrameLite * instance );
		void Sum( GeometryCacheSummation & summation );
		void Reset();

	private:
		std::map< Geometry *, std::vector< const unify::FrameLite * > > m_cache;
	};			  
}
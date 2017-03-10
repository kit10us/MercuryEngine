// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/RenderParams.h>
#include <unify/FrameLite.h>
						
namespace me
{
	typedef std::vector< const unify::FrameLite * > InstanceCache;

	class GeometryCache
	{
		friend class GeometryCacheSummation;
	public:
		GeometryCache();
		void Add( Geometry * geometry, const unify::FrameLite * instance );
		void Sum( GeometryCacheSummation & summation );
		void Reset();

	private:
		std::map< Geometry *, std::vector< const unify::FrameLite * > > m_cache;
	};			  

	class GeometryCacheSummation
	{
	public:
		GeometryCacheSummation();

		void Add( Geometry * geometry, InstancesSet set );

		void Render( RenderParams params );

		// Keeps geometry, resets count/instances.
		void Reset();

	private:
		std::map< Geometry *, std::vector< InstancesSet > > m_summation;
	};

}
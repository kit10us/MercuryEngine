								// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/RenderInstance.h>
#include <unify/FrameLite.h>
						
namespace me
{
	typedef std::map< Geometry *, std::vector< const unify::FrameLite * > > GeometryCache;
	// std::list< RenderSet >

	struct RenderSet
	{
		RenderSet( Geometry * _geo, unify::Matrix _world )
			: geo( _geo )
			, world( _world )
		{
		}

		Geometry * geo;
		unify::Matrix world;
	};
}
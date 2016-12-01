								// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/RenderInstance.h>
						
namespace me
{
	struct RenderSet
	{
		RenderSet( Geometry::ptr _geo, unify::Matrix _world )
			: geo( _geo )
			, world( _world )
		{
		}

		Geometry::ptr geo;
		unify::Matrix world;
	};
}
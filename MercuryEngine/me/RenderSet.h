								// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/RenderInstance.h>
						
namespace me
{
	struct RenderSet
	{
		RenderSet( Geometry::ptr _geo, RenderInstance _instance )
			: geo( _geo )
			, instance( _instance )
		{
		}

		Geometry::ptr geo;
		RenderInstance instance;
	};
}
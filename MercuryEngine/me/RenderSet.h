								// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/RenderInstance.h>
#include <unify/FrameLite.h>
						
namespace me
{
	typedef std::vector< const unify::FrameLite * > InstanceCache;
	typedef std::map< Geometry *, std::vector< const unify::FrameLite * > > GeometryCache;
}
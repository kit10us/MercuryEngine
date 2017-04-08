// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/V2.h>
#include <unify/Size.h>
#include <unify/Range.h>

namespace unify
{
	class Viewport
	{
	public:
		V2< unsigned long > topLeft;
		Size< unsigned long > size;
	    MinMax< float > clipVolume;
	};
	#include <unify/Viewport.inl>
}


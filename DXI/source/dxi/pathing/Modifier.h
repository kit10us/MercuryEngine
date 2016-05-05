// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <memory>	
#include <unify/Frame.h>

namespace dxi
{
	namespace pathing
	{
		// Inherit from this to modify frames.
		class Modifier
		{
		public:
			typedef std::shared_ptr< Modifier > shared_ptr;

			virtual ~Modifier() {}
			virtual void AtTime( float unitTime, unify::Frame * frame ) = 0;
		};
	}
}
// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/Game.h>
#include <dxi/pi/PhysicsInstance.h>
#include <memory>

namespace dxi
{
	namespace pi
	{
		// An environment presents physics effects to objects.
		class PhysicsEnvironment
		{
		public:
			typedef std::shared_ptr< PhysicsEnvironment > shared_ptr;
			PhysicsEnvironment();
			virtual ~PhysicsEnvironment();

			virtual void ApplyEffect( const RenderInfo & renderInfo, PhysicsInstance * object ) = 0;
		};
	} // namespace pi
} // namespace dxi
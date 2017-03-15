// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/Object.h>

namespace me
{
	namespace scene
	{
		class IOctreeNode
		{
		public:
			virtual ~IOctreeNode() {}

			virtual void Adopt( Object* object ) = 0;
		};
	}
}
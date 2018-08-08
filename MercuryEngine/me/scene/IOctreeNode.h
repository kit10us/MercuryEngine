// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <me/object/Object.h>

namespace me
{
	namespace scene
	{
		class IOctreeNode
		{
		public:
			virtual ~IOctreeNode() {}

			virtual void Adopt( object::Object* object ) = 0;
		};
	}
}
// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <me/scene/IOctreeNode.h>

namespace me
{
	namespace scene
	{
		class OctreeNode : public IOctreeNode
		{
		public:
			OctreeNode( IOctreeNode * parent );
			virtual ~OctreeNode();

			void Adopt( object::Object * object ) override;

		private:
			IOctreeNode * m_parent;
		};
	}
}
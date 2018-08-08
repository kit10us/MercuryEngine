// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <me/scene/IOctreeNode.h>

namespace me
{
	namespace scene
	{
		class OctreeNodeTerm : public IOctreeNode
		{
		public:
			OctreeNodeTerm( IOctreeNode * parent );
			virtual ~OctreeNodeTerm();

			void Adopt( object::Object * object ) override;

		private:
			IOctreeNode * m_parent;
			std::list< object::Object * > m_children;
		};
	}
}
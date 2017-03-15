// Copyright (c) 2002 - 2013, Quentin S. Smith
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

			void Adopt( Object* object ) override;

		private:
			IOctreeNode * m_parent;
			std::list< Object * > m_children;
		};
	}
}
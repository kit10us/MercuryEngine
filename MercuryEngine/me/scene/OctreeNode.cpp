// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/OctreeNode.h>

using namespace me;
using namespace scene;

OctreeNode::OctreeNode( IOctreeNode * parent )
	: m_parent{ parent }
{
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Adopt( Object* object )
{
}

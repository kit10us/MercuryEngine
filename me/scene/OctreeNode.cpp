// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/scene/OctreeNode.h>

using namespace me;
using namespace scene;
using namespace object;

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

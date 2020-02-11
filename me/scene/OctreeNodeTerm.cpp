// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/scene/OctreeNodeTerm.h>

using namespace me;
using namespace scene;
using namespace object;

OctreeNodeTerm::OctreeNodeTerm( IOctreeNode * parent )
	: m_parent{ parent }
{
}

OctreeNodeTerm::~OctreeNodeTerm()
{
}

void OctreeNodeTerm::Adopt( Object* object )
{
}

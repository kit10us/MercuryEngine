// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/OctreeNodeTerm.h>

using namespace me;
using namespace scene;

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

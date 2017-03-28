// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Geometry.h>

using namespace me;

Geometry::Geometry()
{
}

GeometryInstanceData * Geometry::CreateInstanceData()
{
	return 0;
}

unify::BBox< float > & Geometry::GetBBox()
{
	return m_BBox;
}

const unify::BBox< float > & Geometry::GetBBox() const
{
	return m_BBox;
}

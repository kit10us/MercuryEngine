// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/Geometry.h>

using namespace me;
using namespace render;

Geometry::Geometry()
{
}

GeometryInstanceData::ptr Geometry::CreateInstanceData() const
{
	return GeometryInstanceData::ptr {};
}

unify::BBox< float > & Geometry::GetBBox()
{
	return m_BBox;
}

const unify::BBox< float > & Geometry::GetBBox() const
{
	return m_BBox;
}

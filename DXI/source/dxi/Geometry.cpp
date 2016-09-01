// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Geometry.h>

using namespace dxi;

Geometry::Geometry()
{
}

void Geometry::Create()
{
	throw unify::Exception( "Called create from base Geometry!" );
}

void Geometry::Destroy()
{
	throw unify::Exception( "Called destroy from base Geometry!" );
}

void Geometry::Validate()
{
	throw unify::Exception( "Called validate from base Geometry!" );
}

void Geometry::Invalidate()
{
	throw unify::Exception( "Called invalidate from base Geometry!" );
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
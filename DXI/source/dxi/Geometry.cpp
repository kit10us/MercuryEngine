// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Geometry.h>

using namespace dxi;

Geometry::Geometry()
: m_BSphere( 0.0f )
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

const unify::BSphere & Geometry::GetBSphere() const
{
	return m_BSphere;
}

const unify::BBox< float > & Geometry::GetBBox() const
{
	return m_BBox;
}

void Geometry::SetBounds( const unify::BBox< float > & bbox )
{
	m_BBox = bbox;
	m_BSphere = unify::BSphere( m_BBox.Size().Length() );
}

const unify::BBox< float > & Geometry::ComputeBounds()
{
	m_BSphere = unify::BSphere( m_BBox.Size().Length() );
	return m_BBox;
}
// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pi/EnvironmentBox.h>

using namespace dxi;
using namespace pi;

EnvironmentBox::EnvironmentBox()
: m_force( unify::V3< float >( 0, 0, 0 ) )
{
}

EnvironmentBox::EnvironmentBox( const unify::Frame & frame, const unify::BBox< float > & bbox, const unify::V3< float > & force )
: m_frame( frame )
, m_bbox( bbox )
, m_force( force )
{
}

EnvironmentBox::~EnvironmentBox()
{
}

void EnvironmentBox::SetFrame( const unify::Frame & frame )
{
	m_frame = frame;
}

const unify::Frame & EnvironmentBox::GetFrame() const
{
	return m_frame;
}

void EnvironmentBox::SetBBox( const unify::BBox< float > & bbox )
{
	m_bbox = bbox;
}

const unify::BBox< float > & EnvironmentBox::GetBBox() const
{
	return m_bbox;
}

void EnvironmentBox::SetForce( const unify::V3< float > & force )
{
	m_force = force;
}

const unify::V3< float > & EnvironmentBox::GetForce() const
{
	return m_force;
}

void EnvironmentBox::ApplyEffect( float delta, PhysicsInstance * object )
{
	unify::Matrix matrix = m_frame.GetModelMatrix();
	matrix.Invert();
	unify::V3< float > point( object->GetFrame()->GetPosition() );
	matrix.TransformCoord( point );
	if( m_bbox.ContainsPoint( point ) )
	{
		object->ApplyForce( m_force * delta );
	}
}
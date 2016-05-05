// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pi/EnvironmentConstant.h>

using namespace dxi;
using namespace pi;

EnvironmentConstant::EnvironmentConstant()
: m_force( unify::V3< float >( 0, 0, 0 ) )
{
}

EnvironmentConstant::EnvironmentConstant( const unify::V3< float > & force )
: m_force( force )
{
}

EnvironmentConstant::~EnvironmentConstant()
{
}

void EnvironmentConstant::SetForce( const unify::V3< float > & force )
{
	m_force = force;
}

const unify::V3< float > & EnvironmentConstant::GetForce() const
{
	return m_force;
}

void EnvironmentConstant::ApplyEffect( float delta, PhysicsInstance * object )
{
	object->ApplyForce( m_force * delta );
}
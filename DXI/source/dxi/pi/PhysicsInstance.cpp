// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pi/PhysicsInstance.h>
#include <assert.h>

using namespace dxi;
using namespace pi;

PhysicsInstance::PhysicsInstance( physics::IObjectSyncer * syncer )
: m_syncer( syncer )
, m_frame( syncer->GetFrame() )
, m_bbox( syncer->GetBBox() )
, m_previousFrame( m_frame )
, m_moveable( false )
, m_physical( false )
, m_momentum( unify::V3< float >::V3Zero() )
, m_currentMomentum( unify::V3< float >::V3Zero() )
, m_isUpdating( false )
{
}

PhysicsInstance::~PhysicsInstance()
{
}

void PhysicsInstance::SyncObjectFromPhysics()
{
	if( m_syncer )
	{
		m_syncer->SyncFrame( m_frame );
		m_syncer->SyncBBox( m_bbox );
	}
}

void PhysicsInstance::SyncPhysicsFromObject()
{
	if( m_syncer )
	{
	}
}

bool PhysicsInstance::IsActive() const
{
	return true;
}

void PhysicsInstance::SetMoveable( bool moveable )
{
	m_moveable = moveable;
}

bool PhysicsInstance::GetMoveable() const
{
	return m_moveable;
}

void PhysicsInstance::SetPhysical( bool physical )
{
	m_physical = physical;
}

bool PhysicsInstance::GetPhysical() const
{
	return m_physical;
}

unify::Frame * PhysicsInstance::GetFrame()
{
	return &m_frame;
}

unify::Frame * PhysicsInstance::GetPreviousFrame()
{
	return &m_previousFrame;
}

void PhysicsInstance::StopMomentum()
{
	m_momentum = unify::V3< float >::V3Zero();
}

bool PhysicsInstance::IsUpdating() const
{
	return m_isUpdating;
}

void PhysicsInstance::BeginUpdate()
{
	assert( ! m_isUpdating );
	m_isUpdating = true;
	m_previousFrame = m_frame;
}

void PhysicsInstance::ApplyForce( const unify::V3< float > & force )
{
	m_momentum += force;
}

void PhysicsInstance::CaptureTotalForce()
{
	m_currentMomentum = m_momentum;
}

void PhysicsInstance::MunchAndPredict()
{
	m_tempFrame = m_frame;
	m_tempFrame.MoveBy( m_currentMomentum );
	m_currentMomentum = unify::V3< float >::V3Zero();
}

bool PhysicsInstance::StillSomeForce() const
{
	return ! m_currentMomentum.IsZero();
}

void PhysicsInstance::Collide( PhysicsInstance * other )
{
}

void PhysicsInstance::EndUpdate()
{
	m_frame = m_tempFrame;
	m_isUpdating = false;
}


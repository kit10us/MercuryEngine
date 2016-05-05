// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pi/PhysicsScene.h>
#include <boost/cast.hpp>
#include <mmsystem.h>

using namespace dxi;
using namespace pi;

PhysicsScene::PhysicsScene()
: m_lastTime( timeGetTime() )
, m_maxElapsed( 0.1f )
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::Add( std::shared_ptr< physics::IInstance > object )
{
	m_allObjects.push_back( object );
}

void PhysicsScene::AddEnvironment( PhysicsEnvironment::shared_ptr environment )
{
	m_environments.push_back( environment );
}

void PhysicsScene::Update( unify::Seconds elapsed, core::IInput & input )
{
	// Prevent massive physics build up times.
	if( elapsed > m_maxElapsed )
	{
		elapsed = m_maxElapsed;
	}

	std::list< PhysicsInstance * > allObjects;
	std::list< PhysicsInstance * > activeObjects;

	// Begin updating physics instances...
	for( std::list< std::shared_ptr< physics::IInstance > >::iterator itr = m_allObjects.begin(), end = m_allObjects.end(); itr != end; ++itr )
	{
		PhysicsInstance * instance = boost::polymorphic_downcast< PhysicsInstance * >( (*itr).get() );
		allObjects.push_back( instance );
		if( instance->GetMoveable() )
		{
			activeObjects.push_back( instance );
			instance->BeginUpdate();
		}
	}

	// Accumilate environmental effects...
	for( std::list< PhysicsInstance * >::iterator itr = activeObjects.begin(), end = activeObjects.end(); itr != end; ++itr )
	{
		for( std::list< PhysicsEnvironment::shared_ptr >::iterator environmentItr = m_environments.begin(), environmentEnd = m_environments.end(); environmentItr != environmentEnd; ++environmentItr )
		{
			PhysicsInstance * instance = *itr;
			(*environmentItr)->ApplyEffect( elapsed, instance );
		}
	}

	// Capture our total force...
	for( std::list< PhysicsInstance * >::iterator itr = activeObjects.begin(), end = activeObjects.end(); itr != end; ++itr )
	{
		PhysicsInstance * instance = *itr;
		instance->CaptureTotalForce();
	}

	while( ! activeObjects.empty() )
	{
		// Munch forces predicting new position...
		for( std::list< PhysicsInstance * >::iterator itr = activeObjects.begin(), end = activeObjects.end(); itr != end; ++itr )
		{
			PhysicsInstance * instance = *itr;
			instance->MunchAndPredict();
		}

		// Predict collisions...
		for( std::list< PhysicsInstance * >::iterator itr = activeObjects.begin(), end = activeObjects.end(); itr != end; ++itr )
		{
			PhysicsInstance * instance = *itr;
			if ( ! instance->GetPhysical() ) continue;
			for( std::list< PhysicsInstance * >::iterator itrOther = allObjects.begin(), end = allObjects.end(); itrOther != end; ++itrOther )
			{
				PhysicsInstance * otherInstance = *itr;
				if( instance == otherInstance || otherInstance->GetPhysical() ) continue;
				instance->Collide( otherInstance );
			}
		}

		// Remove inactive objects...
		{
			std::list< PhysicsInstance * >::iterator itr = activeObjects.begin(), end = activeObjects.end();
			while( itr != end )
			{
				PhysicsInstance * instance = *itr;
				if( instance->StillSomeForce() )
				{
					++itr;
				}
				else
				{
					itr = activeObjects.erase( itr );
				}
			}
		}

	}

	for( std::list< PhysicsInstance * >::iterator itr = allObjects.begin(), end = allObjects.end(); itr != end; ++itr )
	{
		PhysicsInstance * instance = *itr;
		if( instance->IsUpdating() )
		{
			instance->EndUpdate();
		}
	}
}

void PhysicsScene::Render()
{
}
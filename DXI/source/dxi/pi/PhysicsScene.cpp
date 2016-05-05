// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pi/PhysicsScene.h>
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
	for( auto object : m_allObjects )
	{
		auto instance = std::dynamic_pointer_cast< PhysicsInstance >(object);
		allObjects.push_back( instance.get() );
		if( instance->GetMoveable() )
		{
			activeObjects.push_back( instance.get() );
			instance->BeginUpdate();
		}
	}

	// Accumilate environmental effects...
	for( auto object : activeObjects )
	{
		for( auto environment : m_environments )
		{
			(environment)->ApplyEffect( elapsed, object );
		}
	}

	// Capture our total force...
	for( auto object : activeObjects )
	{
		object->CaptureTotalForce();
	}

	while( ! activeObjects.empty() )
	{
		// Munch forces predicting new position...
		for( auto object : activeObjects )
		{
			object->MunchAndPredict();
		}

		// Predict collisions...
		for(auto object : activeObjects )
		{
			if ( !object->GetPhysical() ) continue;

			for( auto other : allObjects )
			{
				if( object == other || other->GetPhysical() ) continue;
				object->Collide( other );
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

	for( auto object : allObjects )
	{
		if( object->IsUpdating() )
		{
			object->EndUpdate();
		}
	}
}

void PhysicsScene::Render()
{
}
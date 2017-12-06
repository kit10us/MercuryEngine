// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/object/CameraComponent.h>
#include <unify/Frustum.h>
#include <map>

using namespace me;
using namespace scene;
using namespace object;
using namespace render;

ObjectStack::ObjectStack( IScene * scene, size_t max )
	: ObjectAllocator( "ObjectStack" )
	, m_scene{ scene }
	, m_freeObjects{ (int)max }
	, m_count{ 0 }
	, m_nextObjectAvailable{ 0 }
	, m_lastObjectAlive{ 0 }
	, m_objects( max )
	, m_cache{ true }
	, m_resetCache{ false }
{
}

ObjectStack::~ObjectStack()
{
}

size_t ObjectStack::Count() const
{
	return m_count;
}

bool ObjectStack::Available() const
{
	return m_freeObjects > 0;
}

void ObjectStack::SetCache( bool cache )
{
	m_cache = cache;
}

bool ObjectStack::GetCache() const
{
	return m_cache;
}

void ObjectStack::ResetCache()
{
	m_resetCache = true;
}

Object * ObjectStack::NewObject( std::string name )
{
	if ( m_freeObjects == 0 ) return nullptr;

	// Get the next available object...
	Object * object = &m_objects[ m_nextObjectAvailable ];
	m_newObjects.push_back( object );

	m_lastObjectAlive = std::max( m_nextObjectAvailable, m_lastObjectAlive );

	object->SetAlive( true );
	object->SetScene( m_scene );
	object->SetAllocator( this );
	object->SetName( name );

	// Find the next available object...
	// 1. Ensure we are within capacity, or stop.
	// 2. Stop if we are within capacity, and found an available object.
	while( ++m_nextObjectAvailable < (int)m_objects.size() && m_objects[ m_nextObjectAvailable ].IsAlive() );

	m_freeObjects--;
	m_count++;

	return object;
}

bool ObjectStack::DestroyObject( Object * object )
{
	auto itr = std::find( m_oldObjects.begin(), m_oldObjects.end(), object );
	if( itr != m_oldObjects.end() )
	{
		int objectIndex = (Object*)&m_objects[0] - object;

		object->SetAlive( false );

		// Our next object available has to be lowest, so if we removed a lower object...
		m_nextObjectAvailable = std::min( m_nextObjectAvailable, objectIndex );

		// If we removed the object last in our 'alive' set, find the last object alive...
		if( objectIndex == m_lastObjectAlive )
		{
			// Find the last object alive in the list...
			for( int i = objectIndex; i >= 0 && !m_objects[i].IsAlive(); m_lastObjectAlive-- )
			{
				// Do nothing, loop increment steps handles the work.
			}
		}

		m_oldObjects.erase( itr );

		m_freeObjects++;
		return true;
	}
	else
	{
		return false;
	}
}

Object * ObjectStack::CopyObject( Object * from, std::string name )
{
	Object * newObject = NewObject( name );
	newObject->CopyFrom( name, *from );
	return newObject;
}

void ObjectStack::CollectObjects( std::vector< Object * > & objects )
{
	objects.insert( objects.end(), m_oldObjects.begin(), m_oldObjects.end() );
}

Object * ObjectStack::FindObject( std::string name )
{
	auto itr = std::find_if( m_oldObjects.begin(), m_oldObjects.end(), 
		[&]( auto & object ) { 
		return unify::StringIs( object->GetName(), name );
	} );

	if( itr != m_oldObjects.end() )
	{
		return *itr;
	}

	itr = std::find_if( m_newObjects.begin(), m_newObjects.end(),
		[&]( auto & object ) {
		return unify::StringIs( object->GetName(), name );
	} );

	return itr == m_newObjects.end() ? nullptr : *itr;
}

Object * ObjectStack::GetObject( size_t index )
{
	// Not possible if...
	if( (int)index > m_lastObjectAlive )
	{
		return nullptr;
	}

	// NOTE: We don't use our "alive" lists (old or new) because they are considered "unordered".

	// If index is within the unfragmented chunk, this is fastest...
	// This works because... NOA always points to the first gap in our list
	if ( (int)index < m_nextObjectAvailable )
	{
		return &m_objects[index];
	}

	// Brute force search...	
	size_t i = m_nextObjectAvailable + 1; // Only need to start at m_nextObjectAvailable + 1, since (see above) (also, NOA is N/A)
	size_t end = m_lastObjectAlive + 1; // No need searching beyond the last alive object.
	size_t count = m_nextObjectAvailable; // Add the number of objects in the first living chunk, PLUS ONE.
	for( ; i < end; i++ )
	{
		// If object is not alive, don't count it.
		if( !m_objects[i].IsAlive() ) continue;

		if( count == index )
		{
			return &m_objects[i];
		}
		else
		{
			count++;
		}
	}
	return nullptr;
}

void ObjectStack::DirtyObject( object::Object* object )
{
	// If we have no old objects, we just don't care.
	if( m_oldObjects.empty() ) return;

	// No way to know what is cached, so blow up caches...
	m_solids.Reset();
	m_trans.Reset();
	
	// All old objects become new objects...
	m_newObjects.merge( m_oldObjects );
}

void ObjectStack::Update( const UpdateParams & params )
{
	if( m_newObjects.size() )
	{
		for( auto && object : m_newObjects )
		{
			// Initialize
			object->Initialize( m_updatables, m_cameras, params );

			if( m_cache && !m_resetCache )
			{
				object->CollectGeometry( m_solids, m_trans );
			}
		}
		m_oldObjects.merge( m_newObjects );
	}

	if ( m_cache && m_resetCache )
	{
		m_solids.Reset();
		m_trans.Reset();
		for ( int i = 0; i <= m_lastObjectAlive; i++ )
		{
			auto && object = m_objects[ i ];
			if ( ! object.IsAlive() )
			{
				continue;
			}

			// Initialize
			object.CollectGeometry( m_solids, m_trans );
		}
		m_resetCache = false;
	}

	for( auto && updateable : m_updatables )
	{
		updateable->OnUpdate( params );
	}
}

void ObjectStack::CollectCameras( RenderGirl & renderGirl )
{
	for( auto camera : m_cameras )
	{
		renderGirl.AddCamera( camera );
	}
}

void ObjectStack::CollectRendering( render::Params params, const FinalCamera & camera, GeometryCacheSummation & solids, GeometryCacheSummation & trans )
{	
	if ( ! m_cache )
	{
		m_solids.Reset();
		m_trans.Reset();
		for ( int i = 0; i <= m_lastObjectAlive; i++ )
		{
			auto && object = m_objects[ i ];
			if ( !object.IsAlive() )
			{
				continue;
			}

			typedef unify::V3< float > V;
			V origin = camera.object->GetFrame().GetPosition();
			V forward = camera.object->GetFrame().GetForward();
			V position = object.GetFrame().GetPosition() - origin;
			unify::Angle difference( forward.DotAngle( position ) );
			if ( difference.ToDegrees() < 45.0f )
			{
				object.CollectGeometry( m_solids, m_trans );
			}
			else
			{
				int x( 0 ); x;
			}
		}
	}

	m_solids.Sum( solids );
	m_trans.Sum( trans );
}

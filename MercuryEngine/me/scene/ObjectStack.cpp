// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/object/CameraComponent.h>
#include <me/Frustum.h>
#include <map>

using namespace me;
using namespace scene;
using namespace object;

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
	// NOTE: Could optimize by storing object index in object.
	// Find index of object to remove...
	int objectIndex = 0;
	for( ; objectIndex < (int)m_objects.size(); ++objectIndex )
	{
		if ( &m_objects[ objectIndex ] == object )
		{
			break;
		}
	}

	// If we went past our object count, the object wasn't found.
	if ( objectIndex >= (int)m_objects.size() )
	{
		return false;
	}

	m_objects[ objectIndex ].SetAlive( false );
	
	if ( objectIndex < m_nextObjectAvailable ) m_nextObjectAvailable = objectIndex;
	m_lastObjectAlive = std::max( m_nextObjectAvailable, m_lastObjectAlive );

	// If we removed the object last in our 'alive' set...
	if ( objectIndex == m_lastObjectAlive )
	{
		// Find the last object alive in the list...
		for ( int i = objectIndex; i >= 0 && !m_objects[ i ].IsAlive(); m_lastObjectAlive-- )
		{
			// Do nothing, loop increment steps handles the work.
		}
	}
	
	m_freeObjects++;
	m_count--;

	return true;
}

Object * ObjectStack::CopyObject( Object * from, std::string name )
{
	Object * newObject = NewObject( name );
	newObject->CopyFrom( name, *from );
	return newObject;
}

void ObjectStack::CollectObjects( std::vector< Object * > & objects )
{
	for ( int i = 0; i <= m_lastObjectAlive; i++ )
	{
		auto && object = m_objects[ i ];
		if ( ! object.IsAlive() )
		{
			continue;
		}

		objects.push_back( &object );
	}
}

Object * ObjectStack::FindObject( std::string name )
{
	for( auto && object : m_objects )
	{
		if ( object.IsAlive() && unify::StringIs( object.GetName(), name ) )
		{
			return &object;
		}
	}
	return nullptr;
}

Object * ObjectStack::GetObject( size_t index )
{
	if ( index >= m_objects.size() )
	{
		return nullptr;
	}
	return &m_objects[ index ];
}

void ObjectStack::DirtyObject( object::Object* object )
{
	assert( 0 );
	/*
	// Remove from caches...
	if( ! m_solids )
	{
		m_trans.Remove( object );
	}
	*/
}

void ObjectStack::Update( UpdateParams params )
{
	for( auto && object : m_newObjects )
	{
		// Initialize
		object->Initialize( m_updatables, m_cameras, params );

		if ( m_cache && ! m_resetCache )
		{
			object->CollectGeometry( m_solids, m_trans );
		}
	}
	m_newObjects.clear();

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

void ObjectStack::CollectRendering( RenderParams params, const FinalCamera & camera, GeometryCacheSummation & solids, GeometryCacheSummation & trans )
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

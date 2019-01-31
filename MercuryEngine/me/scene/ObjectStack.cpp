// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/object/component/CameraComponent.h>
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
		// Find our object index by subtracting our pointer from the first item.
		int objectIndex = object - (Object*)&m_objects[0];

		object->SetAlive( false );
		object->ClearComponents();

		// Our next object available has to be lowest, so if we removed a lower object...
		m_nextObjectAvailable = std::min( m_nextObjectAvailable, objectIndex );

		// If we removed the object last in our 'alive' set, find the last object alive...
		if( objectIndex == m_lastObjectAlive )
		{
			// Find the last object alive in the list...
			for( m_lastObjectAlive; m_lastObjectAlive >= 0 && !m_objects[ m_lastObjectAlive ].IsAlive(); m_lastObjectAlive-- )
			{
				// Do nothing, loop increment steps handles the work.
			}
		}

		m_oldObjects.erase( itr );

		m_freeObjects++;

		m_resetCache = true;
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
		return unify::string::StringIs( object->GetName(), name );
	} );

	if( itr != m_oldObjects.end() )
	{
		return *itr;
	}

	itr = std::find_if( m_newObjects.begin(), m_newObjects.end(),
		[&]( auto & object ) {
		return unify::string::StringIs( object->GetName(), name );
	} );

	return itr == m_newObjects.end() ? nullptr : *itr;
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
	auto * debug = m_scene->GetOS()->Debug();
	debug->DebugTimeStampBegin( "CollectGeometries" );

	if( m_newObjects.size() )
	{
		for( auto && object : m_newObjects )
		{
			// Initialize
			object->Initialize( m_updatables, m_cameras, params );

			// If we are caching, and not going to do a reset later, then
			if( m_cache && !m_resetCache )
			{
				object->CollectGeometry( m_solids, m_trans );
			}
		}
		m_oldObjects.merge( m_newObjects );
	}

	for( auto && updateable : m_updatables )
	{
		updateable->OnUpdate( params );
	}

	debug->DebugTimeStampEnd( "CollectGeometries" );
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
	if ( ! m_cache || m_resetCache )
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

			// Attempt to cull objects behind our camera.
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
		m_resetCache = false;
	}

	// Add our render lists to our total render lists.
	m_solids.Sum( solids );
	m_trans.Sum( trans );
}

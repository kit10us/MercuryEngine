// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Frustum.h>
#include <map>

using namespace me;
using namespace scene;

ObjectStack::ObjectStack( Scene * scene, size_t max )
	: m_scene{ scene }
	, m_freeObjects{ max }
	, m_count{ 0 }
	, m_nextObjectAvailable{ 0 }
	, m_lastObjectAlive{ 0 }
	, m_objects( max )
{
}

ObjectStack::~ObjectStack()
{
}

bool ObjectStack::IsResizable() const
{
	return false;
}

size_t ObjectStack::Count() const
{
	return m_count;
}

bool ObjectStack::Available() const
{
	return m_freeObjects > 0;
}

Object * ObjectStack::NewObject( std::string name )
{
	if ( m_freeObjects == 0 ) return nullptr;

	// Get the next available object...
	Object * object = &m_objects[ m_nextObjectAvailable ];
	m_newObjects.push_back( object );

	object->SetAlive( true );
	object->SetScene( m_scene );
	object->SetName( name );

	// Find the next available object...
	// 1. Ensure we are within capacity, or stop.
	// 2. Stop if we are within capacity, and found an available object.
	while( ++m_nextObjectAvailable < m_objects.size() && m_objects[ m_nextObjectAvailable ].IsAlive() );

	m_freeObjects--;
	m_count++;

	return object;
}

bool ObjectStack::DestroyObject( Object * object )
{
	// TODO: Remove from cached lists 
	assert( 0 ); // Not supported at the moment.

	for( size_t i = 0; i < m_objects.size(); ++i )
	{
		if ( &m_objects[ i ] == object )
		{
			m_objects[ i ].SetAlive( false );
			if ( i < m_nextObjectAvailable ) m_nextObjectAvailable = i;
			return true;
		}
	}
	
	m_freeObjects++;
	m_count--;

	return false;
}

Object * ObjectStack::CopyObject( Object * from, std::string name )
{
	Object * newObject = NewObject( name );
	newObject->CopyFrom( name, *from );
	return newObject;
}

void ObjectStack::CollectObjects( std::vector< Object * > & objects )
{
	for( auto && object : m_objects )
	{
		if ( ! object.IsAlive() ) continue;
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

void ObjectStack::Update( UpdateParams params )
{
	for( auto && object : m_newObjects )
	{
		// Initialize
		object->Initialize( m_updatables, m_cameras, params );
		object->CollectGeometry( m_geometries );
	}
	m_newObjects.clear();

	for( auto && updateable : m_updatables )
	{
		updateable->OnUpdate( params );
	}
}

void ObjectStack::CollectRendering( RenderParams params, CameraCache & cameras, GeometryCacheSummation & summation )
{					  
	for( auto camera : m_cameras )
	{
		cameras.push_back( camera );
	}

	m_geometries.Sum( summation );
}

// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Frustum.h>
#include <map>

using namespace me;
using namespace scene;

ObjectStack::ObjectStack( Scene * scene, size_t max )
: m_scene( scene )
, m_nextObjectAvailable{ 0 }
, m_lastObjectAlive{ 0 }
, m_objects( max )
{
}

ObjectStack::~ObjectStack()
{
}

Object * ObjectStack::NewObject( std::string name )
{
	// Get the next available object...
	Object * object = &m_objects[ m_nextObjectAvailable ];
	m_newObjects.push_back( object );

	object->SetAlive( true );
	object->SetScene( m_scene );
	object->SetName( name );

	// Find the next available object...
	// 1. Ensure we are within capacity, or stop and grow.
	// 2. Stop if we are within capacity, and found an available object.
	while( ++m_nextObjectAvailable < m_objects.size() && m_objects[ m_nextObjectAvailable ].IsAlive() );
	
	// If we need to reallocate (note that we will be at the next available object too)...
	if ( m_nextObjectAvailable == m_objects.size() )
	{
		m_objects.resize( m_objects.size() * 2 );
	}

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

void ObjectStack::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
	for( auto && object : m_newObjects )
	{
		// Initialize
		object->Initialize( m_updatables, m_geometries, m_cameras,  renderer, renderInfo );
	}
	m_newObjects.clear();

	for( auto && updateable : m_updatables )
	{
		updateable->OnUpdate( renderer, renderInfo );
	}
}

void ObjectStack::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	// Render all geometry for each camera...
	for( auto camera : m_cameras )
	{
		if( camera.camera->GetRenderer() != renderer->GetIndex() ) continue;

		RenderInfo myRenderInfo( renderInfo );
		myRenderInfo.SetViewMatrix( camera.object->GetFrame().GetMatrix().Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		for( auto pair : m_geometries )
		{
			pair.first->Render( renderer, myRenderInfo, 0, pair.second );
		}
	}
}

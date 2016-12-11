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
, m_needUpdateCaching{ true }
, m_needRenderCaching{ true }
{
}

ObjectStack::~ObjectStack()
{
}

Object * ObjectStack::NewObject( std::string name )
{
	// Get the next available object...
	Object * object = &m_objects[ m_nextObjectAvailable ];
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

	m_needUpdateCaching = true;
	m_needRenderCaching = true;
	return object;
}

bool ObjectStack::DestroyObject( Object * object )
{
	for( size_t i = 0; i < m_objects.size(); ++i )
	{
		if ( &m_objects[ i ] == object )
		{
			m_objects[ i ].SetAlive( false );
			if ( i < m_nextObjectAvailable ) m_nextObjectAvailable = i;
			return true;
		}
	}
	m_needUpdateCaching = true;
	m_needRenderCaching = true;						  
	return false;
}

Object * ObjectStack::CopyObject( Object * from, std::string name )
{
	Object * newObject = NewObject( name );
	*newObject = *from;
	newObject->SetName( name );
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
	if ( m_needUpdateCaching )
	{
		CollectObjects( m_cached_updates );
		m_needUpdateCaching = false;
	}

	for( auto && object : m_cached_updates )
	{
		object->Update( renderer, renderInfo );
	}
}

void ObjectStack::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( m_needRenderCaching )
	{
		m_cached_cameraList.clear();
		m_cached_sorted.clear();

		std::list< RenderSet > renderList;

		// Accumulate objects for rendering, and cameras.
		std::vector< Object * > objects;
		CollectObjects( objects );
		for( auto && object : objects )
		{
			if ( ! object->IsEnabled() ) continue;

			// Check for a camera...
			CameraComponent * camera{};
			for( int i = 0; i < object->ComponentCount(); ++i )
			{
				IObjectComponent::ptr component = object->GetComponent( i );
				if( !component->IsEnabled() ) continue;

				camera = dynamic_cast< CameraComponent * >(component.get());
				if( camera != nullptr )
				{
					m_cached_cameraList.push_back( FinalCamera{ object, camera } );
				}
			}																		 

			object->CollectRenderables( m_cached_sorted, renderer, renderInfo );
		}

		if ( m_cached_cameraList.empty() ) return;

		m_needRenderCaching = false;
	}

	// Render all geometry for each camera...
	for( auto camera : m_cached_cameraList )
	{
		if( camera.camera->GetRenderer() != renderer->GetIndex() ) continue;

		RenderInfo myRenderInfo( renderInfo );
		myRenderInfo.SetViewMatrix( camera.object->GetFrame().GetMatrix().Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		for( auto pair : m_cached_sorted )
		{
			pair.first->Render( renderer, myRenderInfo, 0, pair.second );
		}
	}
}

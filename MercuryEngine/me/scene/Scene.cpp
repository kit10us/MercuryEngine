// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Frustum.h>
#include <map>

using namespace me;
using namespace scene;

Scene::Scene( IGame * game )
: m_game( game )
, m_inited( false )
, m_started( false )
, m_lastCullCount( 0 )
, m_order( 0.0f )
, m_enabled( true )
, m_renderObjects( true )
, m_renderSolids( true )
, m_renderTrans( true )
, m_cullingEnabled( true )
, m_defaultLighting( false )
, m_defaultZWriteEnable( true )
, m_nextObjectAvailable{ 0 }
, m_lastObjectAlive{ 0 }
, m_objects( 1000 )
, m_needRenderCaching{ true }
{
}

Scene::~Scene()
{
}

Object * Scene::FindObject( std::string name )
{
	for( auto && instance : m_objects )
	{
		if ( instance.alive && unify::StringIs( instance.object.GetName(), name ) )
		{
			return &instance.object;
		}
	}
	return nullptr;
}

void Scene::OnInit()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnInit( this );
		}
	}
}

void Scene::OnStart()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnStart( this );
		}
	}
}

void Scene::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( ! m_inited )
	{
		OnInit();
		m_inited = true;
	}

    if ( ! m_enabled )
    {
        return;
    }

	// On first update, call start ONCE.
	if ( ! m_started )
	{
		OnStart();
		m_started = true;
	}

	for( auto && component : m_components )
	{
		if ( component->IsEnabled( ) )
		{
			component->OnUpdate( this, renderer, renderInfo );
		}
	}
	
	// Object updating (animation, independant physics)...

	for( auto && instance : m_objects )
	{
		if ( instance.alive )
		{
			instance.object.Update( renderer, renderInfo );
		}
	}
}

void Scene::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	// Render scene components
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnRender( this, renderer, renderInfo );
		}
	}

	if ( m_renderObjects == false )
	{
		return;
	}

	if ( m_needRenderCaching )
	{
		m_cached_cameraList.clear();
		m_cached_sorted.clear();

		std::list< RenderSet > renderList;

		// Accumulate objects for rendering, and cameras.
		for( auto && instance : m_objects )
		{
			if ( ! instance.alive || ! instance.object.IsEnabled() ) continue;

			auto && object = instance.object;
		
			// Check for a camera...
			CameraComponent * camera{};
			for( int i = 0; i < object.ComponentCount(); ++i )
			{
				IObjectComponent::ptr component = object.GetComponent( i );
				if( !component->IsEnabled() ) continue;

				camera = dynamic_cast< CameraComponent * >(component.get());
				if( camera != nullptr )
				{
					m_cached_cameraList.push_back( FinalCamera{ &object, camera } );
				}
			}																		 

			object.CollectRenderables( m_cached_sorted, renderer, renderInfo );
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

void Scene::Suspend()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnSuspend();
		}
	}	

	for( auto && instance : m_objects )
	{
		if( instance.alive )
		{
			instance.object.OnSuspend();
		}
	}
}

void Scene::Resume()
{
	for( auto && instance : m_objects )
	{
		if( instance.alive )
		{
			instance.object.OnResume();
		}
	}

	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnResume();
		}
	}
}

void Scene::SetDefaultLighting( bool lighting )
{
	m_defaultLighting = lighting ? true : FALSE;
}

void Scene::SetDefaultZWriteEnable( bool zWriteEnable )
{
	m_defaultZWriteEnable = zWriteEnable ? true : FALSE;
}

unsigned int Scene::LastCullCount() const
{
	return m_lastCullCount;
}

void Scene::SetRenders( bool bSolids, bool bTrans ) 
{ 
	m_renderSolids = bSolids;
	m_renderTrans = bTrans; 
}

void Scene::SetCulling( bool enabled ) 
{ 
	m_cullingEnabled = enabled;
}

bool Scene::RayTest( const unify::V3< float > * pFirst, const unify::V3< float > * pSecond )
{
	return true;
}

void Scene::SetSize( const unify::Size< float > & size )
{
	m_viewport.SetSize( size );
}

unify::Size< float > Scene::GetSize() const
{
	return m_viewport.GetSize();
}

void Scene::SetPosition( const unify::V2< float > & position )
{
	m_viewport.SetUL( position );
}

unify::V2< float > Scene::GetPosition() const
{
	return m_viewport.GetUL();
}

void Scene::SetZ( const unify::MinMax< float > & z )
{
	m_viewport.SetMinDepth( z.Min() );
	m_viewport.SetMaxDepth( z.Max() );
}

void Scene::SetOrder( float order )
{
    m_order = order;
}

float Scene::GetOrder() const
{
    return m_order;
}

void Scene::SetEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool Scene::GetEnabled() const
{
    return m_enabled;
}

void Scene::SetRenderObjects( bool enabled )
{
	m_renderObjects = enabled;
}

bool Scene::GetRenderObjects() const
{
	return m_renderObjects;
}

int Scene::ComponentCount() const
{
	return (int)m_components.size();
}

void Scene::AddComponent( ISceneComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( component );
}

void Scene::RemoveComponent( ISceneComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

ISceneComponent::ptr Scene::GetComponent( int index )
{
	if ( index > (int)m_components.size() ) return ISceneComponent::ptr();

	int i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component;
		++i;
	}

	assert( 0 );
	return ISceneComponent::ptr(); // Should never hit here.
}

ISceneComponent::ptr Scene::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if ( index == -1 ) return ISceneComponent::ptr();
	return GetComponent( index );
}

int Scene::FindComponent( std::string name, int startIndex ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( i >= startIndex && unify::StringIs( component->GetName(), name ) ) return i;
		++i;
	}
	return -1;
}

Object * Scene::NewObject( std::string name )
{
	// Get the next available object...
	Object * object = &m_objects[ m_nextObjectAvailable ].object;
	m_objects[ m_nextObjectAvailable ].alive = true;

	object->SetScene( this );
	object->SetName( name );
	object->AddTag( "Object #" + unify::Cast< std::string >( m_nextObjectAvailable ) );

	// Find the next available object...
	// 1. Ensure we are within capacity, or stop and grow.
	// 2. Stop if we are within capacity, and found an available object.
	while( ++m_nextObjectAvailable < m_objects.size() && m_objects[ m_nextObjectAvailable ].alive );
	
	// If we need to reallocate (note that we will be at the next available object too)...
	if ( m_nextObjectAvailable == m_objects.size() )
	{
		m_objects.resize( m_objects.size() * 2 );
	}

	m_needRenderCaching = true;

	return object;
}

void Scene::DestroyObject( Object * object )
{
	for( size_t i = 0; i < m_objects.size(); ++i )
	{
		if ( &m_objects[ i ].object == object )
		{
			m_objects[ i ].alive = false;
			if ( i < m_nextObjectAvailable ) m_nextObjectAvailable = i;
			break;
		}
	}
	m_needRenderCaching = true;
}

Object * Scene::CopyObject( Object * from, std::string name )
{
	Object * newObject = NewObject( name );
	*newObject = *from;
	return newObject;
}

Object * Scene::CopyObject( std::string from, std::string name )
{
	Object * fromObject = FindObject( from );
	if ( ! fromObject ) return nullptr;

	return CopyObject( fromObject, name );
}

void Scene::CollectObjects( std::vector< Object * > objects )
{
	for( auto && instance : m_objects )
	{
		if ( ! instance.alive ) return;
		objects.push_back( &instance.object );
	}
}

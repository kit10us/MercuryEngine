// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Frustum.h>
#include <map>

using namespace me;
using namespace scene;

Scene::Scene( IGame * game, std::string name )
: m_game( game )
, m_name{ name }
, m_inited( false )
, m_started( false )
, m_order( 0.0f )
, m_enabled( true )
, m_renderObjects( true )
, m_objectStack{new GrowableObjectStack{ this, 4000} }
{
}

Scene::~Scene()
{
}

std::string Scene::GetName() const
{
	return m_name;
}

size_t Scene::ObjectCount() const
{
	return m_objectStack->Count();
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

void Scene::Update( UpdateParams params )
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
			component->OnUpdate( this, params );
		}
	}

	m_cameras.clear();
	m_objectStack->Update( params, m_cameras );
}

void Scene::Render( RenderParams params )
{
	// Render scene components
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnRender( this, params );
		}
	}

	if ( m_renderObjects == false )
	{
		return;
	}

	GeometryCacheSummation summation;
	m_objectStack->CollectRendering( params, summation );
						 
	// Render all geometry for each camera...
	for( auto camera : m_cameras )
	{
		if( camera.camera->GetRenderer() != params.renderer->GetIndex() ) continue;

		RenderInfo myRenderInfo( params.renderInfo );
		myRenderInfo.SetViewMatrix( camera.object->GetFrame().GetMatrix().Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		summation.Render( RenderParams{ params.renderer, myRenderInfo } );
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

	std::vector< Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnSuspend();
	}
}

void Scene::Resume()
{
	std::vector< Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnResume();
	}

	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnResume();
		}
	}
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
	Object * object = m_objectStack->NewObject( name );
	return object;
}

bool Scene::DestroyObject( Object * object )
{
	if ( ! m_objectStack->DestroyObject( object ) )
	{
		return false;
	}
	return true;
}

Object * Scene::CopyObject( Object * from, std::string name )
{
	return m_objectStack->CopyObject( from, name );
}

void Scene::CollectObjects( std::vector< Object * > & objects )
{
	m_objectStack->CollectObjects( objects );
}

Object * Scene::FindObject( std::string name )
{
	return m_objectStack->FindObject( name );
}

Object * Scene::GetObject( size_t index ) const
{
	return m_objectStack->GetObject( index );
}

size_t Scene::GetObjectCount() const
{
	return m_objectStack->Count();
}

// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/object/CameraComponent.h>
#include <me/scene/ObjectAllocatorComponent.h>
#include <me/Frustum.h>
#include <me/scene/RenderGirl.h>

using namespace me;
using namespace scene;
using namespace object;

Scene::Scene( IGame * game, std::string name )
: m_game( game )
, m_name{ name }
, m_inited( false )
, m_started( false )
, m_order( 0.0f )
, m_enabled( true )
, m_renderCount{ 0 }
{
	auto objectAllocatorComponent = new ObjectAllocatorComponent( game->GetOS() );
	AddComponent( ISceneComponent::ptr( objectAllocatorComponent ) );

	m_objectAllocator = objectAllocatorComponent->QueryInterfaceT< IObjectAllocator >( "IObjectAllocator" );
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
	return m_objectAllocator->Count();
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
}

void Scene::Render( RenderParams params )
{
	RenderGirl renderGirl;
	renderGirl.Begin( &params );
		
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnRender( this, renderGirl );
		}
	}

	m_renderCount = renderGirl.End();
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
}

void Scene::Resume()
{
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

int Scene::GetComponentCount() const
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

	return ISceneComponent::ptr();
}

ISceneComponent::ptr Scene::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if ( index == -1 ) return ISceneComponent::ptr();
	return GetComponent( index );
}

int Scene::FindComponent( std::string typeName, int startIndex ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( i >= startIndex && unify::StringIs( component->GetTypeName(), typeName ) ) return i;
		++i;
	}
	return -1;
}

IObjectAllocator * Scene::GetObjectAllocator()
{
	return m_objectAllocator;
}

Object * Scene::FindObject( std::string name )
{
	return m_objectAllocator->FindObject( name );
}

size_t Scene::GetRenderCount() const
{
	return m_renderCount;
}
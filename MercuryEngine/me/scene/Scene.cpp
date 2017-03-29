// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/SceneManager.h>
#include <me/object/CameraComponent.h>
#include <me/scene/ObjectAllocatorComponent.h>
#include <me/Frustum.h>
#include <me/scene/RenderGirl.h>

using namespace me;
using namespace scene;
using namespace object;

Scene::Scene( Game * game, std::string name )
: m_game( game )
, m_name{ name }
, m_ownership{ unify::Owner::Create( name ) }
, m_sceneManager{}
{
	auto objectAllocatorComponent = new ObjectAllocatorComponent( game->GetOS() );
	AddComponent( ISceneComponent::ptr( objectAllocatorComponent ) );

	m_objectAllocator = objectAllocatorComponent->QueryInterfaceT< IObjectAllocator >( "IObjectAllocator" );
}

Scene::~Scene()
{
}

unify::Owner::ptr Scene::GetOwnership()
{
	return m_ownership;
}

void Scene::Component_OnBeforeStart()
{
	for( auto && component : m_components )
	{

		if( component->IsEnabled() )
		{
			GetGame()->LogLine( "Component \"" + component->GetTypeName() + "\" OnBeforeStart Begin" );
			component->OnBeforeStart();
			GetGame()->LogLine( "Component \"" + component->GetTypeName() + "\" OnBeforeStart Done" );
		}
		else
		{
			GetGame()->LogLine( "Component \"" + component->GetTypeName() + "\" OnBeforeStart Skipped (not enabled)" );
		}
	}
}

void Scene::Component_OnAfterStart()
{
	for ( auto && component : m_components )
	{
		if (component->IsEnabled())
		{
			GetGame()->LogLine("Component \"" + component->GetTypeName() + "\" OnAfterStart Begin");
			component->OnAfterStart();
			GetGame()->LogLine("Component \"" + component->GetTypeName() + "\" OnAfterStart Done");
		}
		else
		{
			GetGame()->LogLine("Component \"" + component->GetTypeName() + "\" OnAfterStart Skipped (not enabled)");
		}
	}
}

void Scene::Component_OnEarlyUpdate( UpdateParams & params )
{
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnEarlyUpdate( params );
		}
	}
}

void Scene::Component_OnUpdate( UpdateParams & params )
{
	for( auto && component : m_components )
	{
		if ( component->IsEnabled( ) )
		{
			component->OnUpdate( params );
		}
	}
}

void Scene::Component_OnLateUpdate( UpdateParams & params )
{

	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnLateUpdate( params );
		}
	}
}

void Scene::Component_OnRender( RenderGirl renderGirl )
{
	// Collect cameras...
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->CollectCameras( renderGirl );
		}
	}

	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnRender( renderGirl );
		}
	}
}

void Scene::Component_OnSuspend()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnSuspend();
		}
	}	
}

void Scene::Component_OnResume()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnResume();
		}
	}
}

void Scene::Component_OnEnd()
{
	for (auto && component : m_components)
	{
		if (component->IsEnabled())
		{
			component->OnEnd();
		}
	}
}

me::Game * Scene::GetGame()
{
	return m_game;
}

me::IOS * Scene::GetOS()
{
	return m_game->GetOS();
}

std::string Scene::GetName() const
{
	return m_name;
}

size_t Scene::ObjectCount() const
{
	return m_objectAllocator->Count();
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

ISceneComponent* Scene::GetComponent( int index )
{
	if ( index > (int)m_components.size() ) return nullptr;

	int i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component.get();
		++i;
	}

	return nullptr;
}

ISceneComponent* Scene::GetComponent( std::string name )
{
	int index = FindComponent( name );
	if ( index == -1 ) return nullptr;
	return GetComponent( index );
}

int Scene::FindComponent( std::string typeName ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( unify::StringIs( component->GetTypeName(), typeName ) ) return i;
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

std::string Scene::SendCommand( std::string command, std::string extra )
{
	command; extra;
	return nullptr;
}

SceneManager* Scene::GetSceneManager()
{
	return m_sceneManager;
}
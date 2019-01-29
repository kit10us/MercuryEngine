// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/SceneManager.h>
#include <me/object/component/CameraComponent.h>
#include <me/scene/component/ObjectAllocatorComponent.h>
#include <me/scene/RenderGirl.h>
#include <me/debug/Block.h>
#include <unify/Frustum.h>

using namespace me;
using namespace scene;

Scene::Scene( game::Game * gameInstance, std::string name )
: m_game( gameInstance )
, m_name{ name }
, m_ownership{ unify::Owner::Create( name ) }
, m_sceneManager{}
{
	auto objectAllocatorComponent = new component::ObjectAllocatorComponent( gameInstance->GetOS() );
	AddComponent( component::ISceneComponent::ptr( objectAllocatorComponent ) );

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
	debug::Block block( GetGame()->Debug(), "Scene::OnBeforeStart" );

	for( auto && component : m_components )
	{
		debug::Block onBeforStartBlock( block, "Component \"" + component->GetTypeName() );
		if( component->IsEnabled() )
		{
			onBeforStartBlock.LogLine( "enabled" );
			component->OnBeforeStart();
		}
		else
		{
			onBeforStartBlock.LogLine( "disabled, skipping" );
		}
	}
}

void Scene::Component_OnAfterStart()
{
	debug::Block block( GetGame()->Debug(), "Scene::OnAfterStart" );

	for ( auto && component : m_components )
	{
		if (component->IsEnabled())
		{
			block.LogLine( "Component \"" + component->GetTypeName() + "\" OnAfterStart Begin" );
			component->OnAfterStart();
			block.LogLine( "Component \"" + component->GetTypeName() + "\" OnAfterStart Done" );
		}
		else
		{
			block.LogLine( "Component \"" + component->GetTypeName() + "\" OnAfterStart Skipped (not enabled)" );
		}
	}
}

void Scene::Component_OnEarlyUpdate( const UpdateParams & params )
{
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnEarlyUpdate( params );
		}
	}
}

void Scene::Component_OnUpdate( const UpdateParams & params )
{
	for( auto && component : m_components )
	{
		if ( component->IsEnabled( ) )
		{
			component->OnUpdate( params );
		}
	}
}

void Scene::Component_OnLateUpdate( const UpdateParams & params )
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

game::IGame *Scene::GetGame()
{
	return m_game;
}

me::os::IOS * Scene::GetOS()
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

void Scene::AddComponent( component::ISceneComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( component );
}

void Scene::RemoveComponent( component::ISceneComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

component::ISceneComponent* Scene::GetComponent( int index )
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

component::ISceneComponent* Scene::GetComponent( std::string name )
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
		if ( unify::string::StringIs( component->GetTypeName(), typeName ) ) return i;
		++i;
	}
	return -1;
}

IObjectAllocator * Scene::GetObjectAllocator()
{
	return m_objectAllocator;
}

object::Object * Scene::FindObject( std::string name )
{
	return m_objectAllocator->FindObject( name );
}

std::list< HitInstance > Scene::FindObjectsWithinRay( unify::Ray ray, float withinDistance ) const
{
	std::list< HitInstance > instances;

	std::vector< object::Object * > objects;
	m_objectAllocator->CollectObjects( objects );

	for( auto * object : objects )
	{
		float distance = ray.origin.DistanceTo( object->GetPosition() );
		if( distance > withinDistance )
		{
			continue;
		}

		unify::Ray rayXformed = object->GetFrame().GetMatrix().Inverse().TransformRay( ray );

		float d{ float() };
		if ( object->Intersects( rayXformed, d ) )
		{
			instances.push_back( { object, distance } );
		}
	}

	instances.sort();

	return instances;
}

std::list< HitInstance > Scene::FindObjectsWithinSphere( unify::BSphere< float > sphere ) const
{
	std::list< HitInstance > instances;

	std::vector< me::object::Object * > objects;
	m_objectAllocator->CollectObjects( objects );
	
	for( auto * object : objects )
	{
		unify::V3< float > position = object->GetFrame().GetPosition();
		auto objectSphere = object->GetBSphere();
		objectSphere = object->GetFrame().GetMatrix().TransformBSphere( objectSphere );

		if( sphere.Collides( objectSphere ) )
		{
			auto Ra = sphere.GetRadius();
			auto Rb = objectSphere.GetRadius();
			auto Rt = Ra + Rb;
			auto D = sphere.GetCenter().DistanceTo( objectSphere.GetCenter() );

			float distance = sphere.GetCenter().DistanceTo( objectSphere.GetCenter() );
			instances.push_back( { object, distance } );
		}
	}

	instances.sort();
	return instances;
}

void Scene::AddResources( unify::Path path )
{
	debug::Block block{ GetOS()->Debug(), "Scene::AddResources(" + path.ToString() + ")" };

	try
	{
		auto realPath = GetOS()->GetAssetPaths()->FindAsset( path );
		qxml::Document doc( realPath );

		for (auto & itr = doc.GetRoot()->Children( "asset" ).begin(); itr != doc.GetRoot()->Children().end(); ++itr)
		{
			auto type = (*itr).GetAttribute< std::string >( "type" );
			auto name = (*itr).GetAttributeElse< std::string >( "name", std::string() );
			unify::Path source{ (*itr).GetAttribute< std::string >( "source" ) };

			GetGame()->GetResourceHub().GetManagerRaw( type )->AddResource( name, source );
		}
	}
	catch( std::exception ex )
	{
		GetOS()->Debug()->ReportError( me::ErrorLevel::Critical, "Scene::AddResource(" + path.ToString() + ")", ex.what() );
	}
}

SceneManager* Scene::GetSceneManager()
{
	return m_sceneManager;
}
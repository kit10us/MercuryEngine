// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/GameComponent.h>
#include <mephy/collider/BoxCollider.h>
#include <mephy/Entity.h>
#include <mephy/SceneComponent.h>
#include <me/scene/SceneManager.h>

using namespace mephy;

char* GameComponent::Name()
{
	return "MEPhysics";
}

GameComponent::GameComponent()
	: me::GameComponent( Name() )
{
}

GameComponent::~GameComponent()
{																	 
}

MEPHYSICS_API me::object::IObjectComponent::ptr GameComponent::CreateBoxCollider( unify::V3< float > halfExt )
{									   
	auto sceneManager = GetGame()->GetComponentT< me::scene::SceneManager >();

	auto scene = sceneManager->GetCurrentScene();

	// Create the PhysicsSceneComponent only if we have colliders.
	auto physicsSceneComponent = scene->GetComponentT< mephy::SceneComponent >();
	if( !physicsSceneComponent )
	{
		physicsSceneComponent = new mephy::SceneComponent( GetGame()->GetOS() );
		scene->AddComponent( me::scene::ISceneComponent::ptr( physicsSceneComponent ) );
	}

	collider::ColliderBase * collider = new collider::BoxCollider( halfExt );
	physicsSceneComponent->AddCollider( collider );		
	return me::object::IObjectComponent::ptr( collider );
}

MEPHYSICS_API me::object::IObjectComponent::ptr GameComponent::CreateEntity()
{
	auto sceneManager = GetGame()->GetComponentT< me::scene::SceneManager >();

	auto scene = sceneManager->GetCurrentScene();

	// Create the PhysicsSceneComponent only if we have colliders.
	auto physicsSceneComponent = scene->GetComponentT< mephy::SceneComponent >();
	if( !physicsSceneComponent )
	{
		scene->AddComponent( me::scene::ISceneComponent::ptr( new mephy::SceneComponent( GetGame()->GetOS() ) ) );
	}

	Entity * entity = new Entity();
	physicsSceneComponent->AddEntity( entity );

	return me::object::IObjectComponent::ptr( entity );
}

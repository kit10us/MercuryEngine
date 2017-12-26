// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy2d/GameComponent.h>
#include <mephy2d/collider/RectangleCollider.h>
#include <mephy2d/collider/CircleCollider.h>
#include <mephy2d/SceneComponent.h>
#include <me/scene/SceneManager.h>

using namespace mephy;

char* GameComponent::Name()
{
	return "MEPhysics2D";
}

GameComponent::GameComponent()
	: me::game::GameComponent( Name() )
{
}

GameComponent::~GameComponent()
{																	 
}

MEPHYSICS2D_API me::object::IObjectComponent::ptr GameComponent::CreateRectangleCollider( unify::V3< float > halfExt )
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

	collider::ColliderBase * collider = new collider::RectangleCollider( halfExt );
	physicsSceneComponent->AddCollider( collider );		
	return me::object::IObjectComponent::ptr( collider );
}

MEPHYSICS2D_API me::object::IObjectComponent::ptr GameComponent::CreateEntity()
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

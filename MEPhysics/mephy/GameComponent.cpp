// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/GameComponent.h>
#include <mephy/collider/BoxCollider.h>
#include <mephy/SceneComponent.h>
#include <me/scene/SceneManager.h>

using namespace mephy;

char* GameComponent::Name()
{
	return "MEPhysics";
}

GameComponent::GameComponent()
	: me::game::GameComponent( Name() )
{
}

GameComponent::~GameComponent()
{																	 
}

MEPHYSICS_API me::object::component::IObjectComponent::ptr GameComponent::CreateBoxCollider( unify::V3< float > halfExt, bool moveable, float mass )
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

	me::object::component::IObjectComponent::ptr collider{ new collider::BoxCollider( halfExt, moveable, mass ) };
	physicsSceneComponent->AddCollider( collider );		
	return collider;
}

// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/SceneComponent.h>

using namespace mephy;

char* SceneComponent::Name()
{
	return "MEPhysicsSceneComponent";
}

SceneComponent::SceneComponent( me::IOS * os )
	: me::scene::SceneComponent( os, Name() )
{
}

SceneComponent::~SceneComponent()
{																	 
}

void SceneComponent::AddCollider( collider::ColliderBase * collider )
{
	m_colliders.push_back( collider );
}

void SceneComponent::AddEntity( Entity * entity )
{
	m_entities.push_back( entity );
}

void SceneComponent::OnEarlyUpdate( me::UpdateParams params )
{
}

void SceneComponent::OnLateUpdate( me::UpdateParams params )
{
	// Very basic.
	for( auto collider : m_colliders )
	{
		for( auto entity : m_entities )
		{
			collider->TestCollision( entity, params );			
		}
	}
}

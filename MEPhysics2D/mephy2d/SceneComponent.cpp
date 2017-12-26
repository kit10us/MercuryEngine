// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy2D/SceneComponent.h>

using namespace mephy2d;

char* SceneComponent::Name()
{
	return "MEPhysics2DSceneComponent";
}

SceneComponent::SceneComponent( me::IOS * os )
	: me::scene::SceneComponent( os, Name() )
{
}

SceneComponent::~SceneComponent()
{																	 
}

void SceneComponent::AddCollider( collider::ColliderBase2D * collider )
{
	m_colliders.push_back( collider );
}

void SceneComponent::AddEntity( Entity * entity )
{
	m_entities.push_back( entity );
}

void SceneComponent::OnEarlyUpdate( const me::UpdateParams & params )
{
	for( auto entity : m_entities )
	{
		entity->CaptureEarly();
	}
}

void SceneComponent::OnLateUpdate( const me::UpdateParams & params )
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

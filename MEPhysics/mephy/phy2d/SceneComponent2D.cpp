// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/phy2d/SceneComponent2D.h>

using namespace mephy;
using namespace phy2d;

MEPHYSICS_API char* SceneComponent::Name()
{
	return "MEPhysicsSceneComponent2D";
}

MEPHYSICS_API SceneComponent::SceneComponent( me::os::IOS * os )
	: me::scene::SceneComponent( os, Name() )
{
}

SceneComponent::~SceneComponent()
{																	 
}

MEPHYSICS_API Entity* SceneComponent::AddEntity( me::object::Object * object )
{
	auto * entity = new Entity();
	m_entities.push_back( entity );
	return entity;
}

void SceneComponent::OnEarlyUpdate( const me::UpdateParams & params )
{
	for( auto * entity : m_entities )
	{
		entity->UpdateEntities();
	}
}

void SceneComponent::OnLateUpdate( const me::UpdateParams & params )
{
}

// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <terrain/Map.h>
#include <TerrainSceneComponent.h>
#include <adv/PlayerMovement.h>

class MainScene : public me::scene::Scene
{
public:
	terrain::Map* m_map;
	unify::Size< int > m_mapSize;
	unify::Size< float > m_terraSize;
	TerrainSceneComponent::ptr m_mapSC;
	adv::PlayerMovement::ptr m_playerMovement;

public:
	MainScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;

	std::string SendCommand( size_t id, std::string extra ) override;
};

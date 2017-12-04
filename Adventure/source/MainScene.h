// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <terrain/Map.h>
#include <TerrainSceneComponent.h>

class MainScene : public me::scene::Scene
{
public:
	terrain::Map* m_map;
	unify::Size< int > m_mapSize;
	unify::Size< float > m_terraSize;
	TerrainSceneComponent::ptr m_mapSC;

public:
	MainScene( me::Game * game );

	void OnStart() override;
};

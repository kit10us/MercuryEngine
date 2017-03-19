// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <TerrainMap.h>
#include <TerrainSceneComponent.h>

class MainScene : public me::scene::Scene
{
	TerrainMap::ptr m_map;
	TerrainSceneComponent::ptr m_mapSC;
public:
	MainScene( me::Game * game );

	void OnStart() override;
};

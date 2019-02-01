// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <terrain/Map.h>
#include <TerrainSceneComponent.h>

class LoadScene : public me::scene::Scene
{
public:
	LoadScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
};

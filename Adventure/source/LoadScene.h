// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <terrain/Map.h>
#include <TerrainSceneComponent.h>

class LoadScene : public me::scene::Scene
{
public:
public:
	LoadScene( me::game::Game * gameInstance );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
};

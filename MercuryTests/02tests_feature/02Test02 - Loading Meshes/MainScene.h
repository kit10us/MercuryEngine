// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>

class MainScene : public me::scene::Scene
{
public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams & params ) override;
};

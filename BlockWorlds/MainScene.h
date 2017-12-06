// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <bw/World.h>

class MainScene : public me::scene::Scene
{
protected:
	std::shared_ptr< bw::World > m_world;

public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
};

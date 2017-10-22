// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/Effect.h>
#include <me/render/IVertexBuffer.h>

class MainScene : public me::scene::Scene
{
	me::render::Effect::ptr effect;
	me::render::IVertexBuffer::ptr vertexBuffer;
	unify::Quaternion q;

public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams & params ) override;
};

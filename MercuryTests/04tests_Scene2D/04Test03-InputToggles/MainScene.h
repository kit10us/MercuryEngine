// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/Effect.h>
#include <me/IVertexBuffer.h>

class MainScene : public me::scene::Scene
{
	me::Effect::ptr effect;
	me::IVertexBuffer::ptr vertexBuffer;
	unify::Quaternion q;
public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};

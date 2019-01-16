// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>

class MainScene : public me::scene::Scene
{
	me::render::Effect::ptr effectBorg;
	me::render::Effect::ptr effect4;
	me::render::IVertexBuffer::ptr vertexBuffer;
	unify::Quaternion q;

public:
	MainScene( me::game::Game * gameInstance );

public: // me::game::IGame
	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};

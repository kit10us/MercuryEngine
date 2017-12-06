// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>
#include <me/render/Mesh.h>

class MainScene : public me::scene::Scene
{
	std::shared_ptr< me::render::Mesh > mesh;
	unify::Quaternion q;
public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( const me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};

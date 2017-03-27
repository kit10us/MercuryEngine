// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/RenderMethod.h>
#include <me/VertexUtil.h>
#include <me/Mesh.h>

class MainScene : public me::scene::Scene
{
	std::shared_ptr< me::Mesh > mesh;
	unify::Quaternion q;
public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};

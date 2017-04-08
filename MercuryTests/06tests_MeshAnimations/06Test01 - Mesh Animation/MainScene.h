// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/render/Mesh.h>
#include <me/frameanimation/FrameAnimation.h>

class MainScene : public me::scene::Scene
{
	std::shared_ptr< me::render::Mesh > mesh;
	unify::Quaternion q;
	me::frameanimation::Animation::ptr m_animation;
public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams & params ) override;
	void OnRender( me::scene::RenderGirl renderGirl ) override;
};

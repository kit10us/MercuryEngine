// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Scene.h>
#include <me/Mesh.h>
#include <me/canvas/TextElement.h>

class MainScene : public me::scene::Scene
{
	me::object::Object * m_camera;
	me::canvas::TextElement * m_text;

public:
	MainScene( me::Game * game );

	void OnStart() override;
	void OnUpdate( me::UpdateParams & params ) override;
};

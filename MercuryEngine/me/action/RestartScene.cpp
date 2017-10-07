// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/action/RestartScene.h>
#include <me/scene/SceneManager.h>

using namespace me;
using namespace action;

RestartScene::RestartScene( IGame * game )
	: m_game{ game }
{
}

bool RestartScene::Perform()
{
	auto sceneManager = m_game->GetComponentT< scene::SceneManager >();
	if( sceneManager )
	{
		sceneManager->RestartScene();
	}
	return true;
}

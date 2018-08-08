// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/action/RestartScene.h>
#include <me/scene/SceneManager.h>

using namespace me;
using namespace action;

RestartScene::RestartScene( game::IGame * gameInstance )
	: m_game{ gameInstance }
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

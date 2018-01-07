// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/phy2d/GameComponent2D.h>
#include <me/scene/SceneManager.h>

using namespace mephy;
using namespace phy2d;

char* GameComponent::Name()
{
	return "MEPhysics2D";
}

GameComponent::GameComponent()
	: me::game::GameComponent( Name() )
{
}

GameComponent::~GameComponent()
{																	 
}
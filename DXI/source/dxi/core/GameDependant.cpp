// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/GameDependant.h>

using namespace dxi;
using namespace core;

GameDependant::GameDependant( IGame * game )
	: m_game( game )
{
}

GameDependant::~GameDependant()
{
}

IGame * GameDependant::Game()
{
	return m_game;
}
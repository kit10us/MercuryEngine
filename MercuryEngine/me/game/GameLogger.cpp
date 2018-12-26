// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/game/GameLogger.h>

using namespace me;
using namespace game;

GameLogger::GameLogger( debug::IDebug * debug ) : m_debug( debug )
{
}

void GameLogger::WriteLine( std::string section, std::string text )
{
	m_debug->LogSectionLine( section, text );
}
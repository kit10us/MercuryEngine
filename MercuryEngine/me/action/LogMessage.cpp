// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/action/LogMessage.h>

using namespace me;
using namespace action;

LogMessage::LogMessage( game::IGame * gameInstance, std::string message )
	: m_game{ gameInstance }
	, m_message{ message }
{
}

bool LogMessage::Perform()
{
	m_game->Debug()->LogLine( m_message, 1 );
	return true;
}

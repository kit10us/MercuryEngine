// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/action/LogMessage.h>

using namespace me;
using namespace action;

LogMessage::LogMessage( IGame * game, std::string message )
	: m_game{ game }
	, m_message{ message }
{
}

bool LogMessage::Perform()
{
	m_game->LogLine(m_message);
	return true;
}

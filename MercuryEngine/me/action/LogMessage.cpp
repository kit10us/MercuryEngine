// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/action/LogMessage.h>

using namespace me;
using namespace action;

LogMessage::LogMessage( game::IGame * gameInstance, std::string section, std::string message )
	: m_game{ gameInstance }
	, m_section{ section }
	, m_message{ message }
{
}

bool LogMessage::Perform()
{
	m_game->Debug()->LogSectionLine( m_section, m_message );
	return true;
}

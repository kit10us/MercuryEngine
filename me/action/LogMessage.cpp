// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/action/LogMessage.h>

using namespace me::action;

LogMessage::LogMessage( game::IGame * gameInstance, std::string section, std::string message )
	: m_game{ gameInstance }
	, m_section{ section }
	, m_message{ message }
{
}

bool LogMessage::Perform()
{
	m_game->Debug()->GetLogger()->Log( m_section + "::" + m_message );
	return true;
}

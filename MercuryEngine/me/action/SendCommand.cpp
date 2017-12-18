// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/action/SendCommand.h>

using namespace me;
using namespace action;

SendCommand::SendCommand( game::IGame * gameInstance, size_t id, std::string extra )
	: m_game{ gameInstance }
	, m_id{ id }
	, m_extra{ extra }
{
}

bool SendCommand::Perform()
{
	m_game->SendCommand( m_id, m_extra );
	return true;
}

// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/Block.h>


using namespace me;
using namespace debug;

Block::Block( kit::ILogger * logger, std::string name )
	: m_logger{ logger }
	, m_name{ name }
{
}

Block::~Block()
{
}

std::string Block::GetName() const
{
	return m_name;
}

void Block::Log( std::string line )
{
	m_logger->BlockLog( line );
}

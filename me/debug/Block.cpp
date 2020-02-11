// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/Block.h>
#include <me/debug/IDebug.h>

using namespace me;
using namespace debug;

Block::Block( IDebug * debug, std::string name )
	: m_debug{ debug }
	, m_name{ name }
{
	m_debug->PushBlock( this );
}

Block::~Block()
{
	m_debug->PopBlock( this );
}

std::string Block::GetName() const
{
	return m_name;
}

void Block::LogLine( std::string line )
{
	m_debug->LogLine( line );
}

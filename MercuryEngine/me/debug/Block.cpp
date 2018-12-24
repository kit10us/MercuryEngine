// Copyright (c) 2003 - 2011, Evil Quail LLC
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
	m_debug->PushBlock( m_name );
}

Block::Block( const Block & parent, std::string name )
	: m_debug{ parent.m_debug }
	, m_parent{ parent.GetResolvedName() }
	, m_name{ name }
{
	m_debug->PushBlock( GetResolvedName() );
}

Block::~Block()
{
	m_debug->PopBlock( GetResolvedName() );
}

std::string Block::GetResolvedName() const
{
	return (! m_parent.empty() ? "::" : "") + m_name;
}

void Block::LogLine( std::string line )
{
	m_debug->LogLine( line );
}

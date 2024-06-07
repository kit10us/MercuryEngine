// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Cast.h>
#include <me/debug/Block.h>
#include <unify/Exception.h>

using namespace me::debug;

Block::Block( kit::debug::ILogger* logger, std::string name, std::shared_ptr< bool > loggerIsAlive )
	: m_logger{ logger }
	, m_parent{}
	, m_name{ name }
	, m_loggerIsAlive{ loggerIsAlive }
{
}

Block::~Block()
{
	// Log that we are entering a new block so we need not manually comment when we are entering a block.
	if ( *m_loggerIsAlive == true )
	{
		m_logger->Log( "Entering block", "Block", (m_parent ? m_parent->GetName() : "") + "<<" + m_name );
	}
}

void Block::SetParent( kit::debug::IBlock* parent )
{
	m_parent = parent;
}

std::string Block::GetName() const
{
	return (m_parent ? ( m_parent->GetName() + "::" ) : "") + m_name;
}

void Block::Log( std::string text, std::string catagory )
{
	if ( *m_loggerIsAlive == true )
	{
		m_logger->Log( text, catagory, GetName());
	}
}

kit::debug::IBlock::ptr Block::SubBlock( std::string name )
{
	if ( *m_loggerIsAlive != true )
	{
		// throw unify::Exception( "Attempted to create sub-block where logger is null!" );
		return {};
	}

	auto block{ m_logger->CreateBlock( name ) };
	block->SetParent( this );

	// Log that we are entering a new block so we need not manually comment when we are entering a block.
	m_logger->Log(name, "Block", GetName() + " >> ");
	return block;
}

void Block::Exec( std::function< void( IBlock* )> functionBlock, bool timed )
{
	using namespace std::chrono;

	high_resolution_clock::time_point lastTime = high_resolution_clock::now();
	if (timed)
	{
		Log("Start timer", "Stats");
	}

	functionBlock( this );


	if (timed)
	{
		high_resolution_clock::time_point currentTime = high_resolution_clock::now();
		duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
		auto micro = duration_cast<microseconds>(currentTime - lastTime).count();
		float duration = micro * 0.000001f;
		Log("End timer at " + unify::Cast< std::string >(duration) + "s", "Stats");
	}
}


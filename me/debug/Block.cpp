// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

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
		m_logger->Log( (m_parent ? m_parent->GetName() : "") + " << " + m_name );
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

void Block::Log( std::string line )
{
	if ( *m_loggerIsAlive == true )
	{
		m_logger->Log( GetName() + ": " + line );
	}
}

kit::debug::IBlock::ptr Block::SubBlock( std::string name )
{
	if ( *m_loggerIsAlive != true )
	{
		throw unify::Exception( "Attempted to create sub-block where logger is null!" );
	}

	auto block{ m_logger->CreateBlock( name ) };
	block->SetParent( this );

	// Log that we are entering a new block so we need not manually comment when we are entering a block.
	m_logger->Log( GetName() + " >> " + name );
	return block;
}

void Block::Exec( std::function< void( IBlock* )> functionBlock )
{
	functionBlock( this );
}


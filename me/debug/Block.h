// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <kit/debug/IBlock.h>
#include <kit/debug/ILogger.h>

namespace me::debug
{
	/// <summary>
	/// Used to log code block entrance and exit.
	/// </summary>
	class Block : public kit::debug::IBlock
	{
		kit::debug::ILogger* m_logger;
		kit::debug::IBlock* m_parent;
		std::string m_name;
		std::shared_ptr< bool > m_loggerIsAlive;

	public:
		Block( kit::debug::ILogger* logger, std::string name, std::shared_ptr< bool > loggerIsAlive );
		~Block() override;

	public: // kit::debug::IBlock...
		void SetParent( kit::debug::IBlock* parent ) override;
		std::string GetName() const override;
		void Log(std::string text, std::string catagory) override;
		kit::debug::IBlock::ptr SubBlock( std::string name ) override;
		void Exec( std::function< void( IBlock* )> functionBlock, bool timed ) override;
	};
}
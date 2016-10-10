// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/input/IInputSource.h>
#include <dxi/core/Game.h>
#include <Xinput.h>
#include <map>

namespace dxigp
{
	class Gamepad : public dxi::input::IInputSource
	{
	public:
		Gamepad( dxi::core::Game * game );
		virtual ~Gamepad();

		std::string Name() const;

		void Update() override;

		size_t SubSourceCount() const override;

		dxi::input::State GetState( size_t subSource, std::string name, std::string condition ) const override;

		bool HasValue( size_t subSource, std::string name ) const override;

		float GetValue( size_t subSource, std::string name ) const override;

	private:
		dxi::core::Game * m_game;
		std::map< DWORD, XINPUT_STATE > m_states;
		std::map< DWORD, XINPUT_STATE > m_prevStates;

	};
}
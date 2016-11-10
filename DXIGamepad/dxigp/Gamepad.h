// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IInputSource.h>
#include <me/IGame.h>
#include <Xinput.h>
#include <map>

namespace dxigp
{
	class Gamepad : public me::IInputSource
	{
	public:
		Gamepad( me::IGame * game );
		virtual ~Gamepad();

		std::string Name() const;

		void Update() override;

		size_t SubSourceCount() const override;

		me::State GetState( size_t subSource, std::string name, std::string condition ) const override;

		bool HasValue( size_t subSource, std::string name ) const override;

		float GetValue( size_t subSource, std::string name ) const override;

		bool SetState( size_t subSource, std::string name, std::string condition, bool set ) override;

		bool SetValue( size_t subSource, std::string name, float value ) override;

	private:
		me::IGame * m_game;
		std::map< DWORD, XINPUT_STATE > m_states;
		std::map< DWORD, XINPUT_STATE > m_prevStates;
	};
}
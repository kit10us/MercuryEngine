// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>
#include <me/IGame.h>
#include <Xinput.h>
#include <map>

namespace dxigp
{
	class Gamepad : public me::input::IInputSource
	{
	public:
		Gamepad( me::IGame * game );
		virtual ~Gamepad();

		std::string Name() const;

		void Update() override;

		size_t SubSourceCount() const override;

		size_t InputCount( size_t subSource ) const override;

		std::string InputName( size_t subSource, size_t index ) const override;

		size_t InputIndex( size_t subSource, std::string name ) const override;

		me::input::InputType GetInputType( size_t subSource, size_t index ) const override;

		me::input::IData::ptr GetInputData( size_t subSource, size_t index ) const override;

		void SetInputData( size_t subSource, size_t index, me::input::IData::ptr dataIn ) override;

	private:
		me::IGame * m_game;
		std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_nameToIndex;
		std::map< DWORD, XINPUT_STATE > m_states;
		std::map< DWORD, XINPUT_STATE > m_prevStates;
	};
}
// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>
#include <me/IGame.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <atlbase.h>
#include <string>

namespace dxikeyboard
{
	enum KeyStatus
	{
		Up,
		Pressed,	// same as STATUS_JUSTUP
		Down,
		JustDown,
		COUNT
	};

	class Keyboard : public me::input::IInputSource
	{
	public:
		Keyboard( me::IGame * game );
		virtual ~Keyboard();

		std::string Name() const;

		void Acquire();

		void Update() override;

		size_t SubSourceCount() const override;

		size_t InputCount( size_t subSource ) const override;

		std::string InputName( size_t subSource, size_t index ) const override;

		size_t InputIndex( size_t subSource, std::string name ) const override;

		size_t InputConditionCount( size_t subSource, size_t index ) const override;

		std::string InputConditionName( size_t subSource, size_t index, size_t condition ) const override;

		size_t InputConditionIndex( size_t subSource, size_t index, std::string condition ) const override;

		me::input::State GetState( size_t subSource, size_t index, size_t condition ) const override;

		bool HasValue( size_t subSource, size_t index ) const override;

		float GetValue( size_t subSource, size_t index ) const override;

		bool SetState( size_t subSource, size_t index, size_t condition, bool set );

		bool SetValue( size_t subSource, size_t index, float value );

	private:
		me::IGame * m_game;

		CComPtr< IDirectInput > m_pdi;
		CComPtr< IDirectInputDevice > m_pdiKeyboard;

		std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_nameToIndex;

		unsigned char				m_diKeyState[256];

		bool m_hasFocus; // Does our window have focus?

		unsigned int m_uKeyStatusCount[ KeyStatus::COUNT ];
		unsigned int m_uTheKey[ KeyStatus::COUNT ];	// VERY simplied - Keeps the LAST key of a given status
		int m_iKeyDown;				// Simple input (not directx)
		int m_iKeyPressTimes[256];
		KeyStatus m_KeyStatus[256];

		bool m_capslock;
		bool m_numlock;
		bool m_scrolllock;
		bool m_insert;
	};
}
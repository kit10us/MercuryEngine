// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/input/IInputSource.h>
#include <dxi/core/IGame.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <atlbase.h>

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

	class Keyboard : public dxi::input::IInputSource
	{
	public:
		Keyboard( dxi::core::IGame * game );
		virtual ~Keyboard();

		std::string Name() const;

		void Acquire();

		void Update() override;

		size_t SubSourceCount() const override;

		dxi::input::State GetState( size_t subSource, std::string name, std::string condition ) const override;

		bool HasValue( size_t subSource, std::string name ) const override;

		float GetValue( size_t subSource, std::string name ) const override;

		bool SetState( size_t subSource, std::string name, std::string condition, bool set );

		bool SetValue( size_t subSource, std::string name, float value );

	private:
		dxi::core::IGame * m_game;

		CComPtr< IDirectInput > m_pdi;
		CComPtr< IDirectInputDevice > m_pdiKeyboard;
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
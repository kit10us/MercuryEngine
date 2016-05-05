// Copyright (c) 2002 Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IInput.h>
#include <dxi/core/IOS.h>
#include <unify/Flags.h>
#include <unify/V3.h>

namespace dxi
{
	// TODO: namespace win, and move to win folder.
	class Input : public core::IInput
	{
	public:
		Input( core::IOS & os );
		~Input();

		void Acquire();
		void CreateKeyboardDevice( HINSTANCE hInstance, HWND hWnd, bool fullScreen );
		void CreateMouseDevice( HINSTANCE hInstance, HWND hWnd, bool fullScreen );

		void Destroy();
		void ReleaseKeyboardDevice();
		void ReleaseMouseDevice();

		/// <summary>
		/// Is the input device ready.
		/// </summary>
		bool IsReady() const;

		void CallBeforeUpdate( const unify::Size< int > resolution, bool fullScreen );
		void CallAfterUpdate();

        // OS feedback...
        void SetMouseUnavailable();
        void SetMousePosition( unify::V2< int > position );
        void SetLeftMouse( bool down );
        void SetRightMouse( bool down );

		// Keyboard...
		bool KeyState( Key::TYPE key ) const;
		InputStatus KeyStatus( Key::TYPE key ) const;
		bool KeyJustDown( Key::TYPE key ) const;
		bool KeyDown( Key::TYPE key ) const;
		bool KeyPressed( Key::TYPE key ) const;
		void ClearKey( Key::TYPE key );
		bool AnyKeyStatus( InputStatus status );

		// TODO: Wrap inputs into seperate classes with access such as:
		// float Get( METHOD ) --->  float Get( "X" );
		// This would optimize scripted posibilities.
		// Also allow custom methods (X(), Y(), ChangeX(), KeyPressed(), etc.) but encourage their use of the standard access method Get()
		// Also allow a capabilities interface as such: CheckCapabilities( CAP_X | CAP_Y | CAP_Z | CAP_ASCII ), etc.

		// Mouse...
        bool IsMouseAvailable() const;
		int MouseChangeX() const;
		int MouseChangeY() const;
		int MouseChangeZ() const;
		
		int MouseX() const;
		int MouseY() const;
		int MouseZ() const;

		bool MouseJustDown( int iButton ) const;
		bool MousePressed( int iButton ) const;
		bool MouseDown( int iButton );

		bool AnyMouseStatus( InputStatus status );

		// Same as mouse down
		bool MouseButton( int iButton );

		bool Capslock() const;
		bool Numlock() const;
		bool Scrolllock() const;
		bool Insert() const;

	protected:
		core::IOS & m_os;

		// DirectInput...
		LPDIRECTINPUT				m_pdi;
		LPDIRECTINPUTDEVICE			m_pdiKeyboard;
		LPDIRECTINPUTDEVICE			m_pdiMouse;
		unsigned char				m_diKeyState[256];
		DIMOUSESTATE				m_diMouseState;

		bool m_hasFocus; // Does our window have focus?

		// Keyboard
		unsigned int m_uKeyStatusCount[NUM_STATUS];
		unsigned int m_uTheKey[NUM_STATUS];	// VERY simplied - Keeps the LAST key of a given status
		int m_iKeyDown;				// Simple input (not directx)
		int m_iKeyPressTimes[256];
		InputStatus m_KeyStatus[256];

		bool m_bCapslock;
		bool m_bNumlock;
		bool m_bScrolllock;
		bool m_bInsert;

		// Mouse
        bool m_mouseAvailable;
		unsigned int m_uMouseStatusCount[NUM_STATUS];
		InputStatus	m_mouseStatus[ 4 ];
        unify::V3< int > m_mouse;
	};

	char GetDIKChar( int p_iKey,  unsigned int dwShiftState = SHIFTSTATE_NONE );
}
// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Keys.h>
#include <unify/Size.h>
#include <unify/V2.h>
#include <dxi/core/IInput.h>

namespace dxi
{
	namespace null
	{
		class Input : public core::IInput
		{
		public:
			virtual bool IsReady() const { return true; }
			virtual void CallBeforeUpdate( const unify::Size< int > resolution, bool fullScreen ) {}
			virtual void CallAfterUpdate() {}

			// OS feedback...
			virtual void SetMousePosition( unify::V2< int > position ) { position; }
			virtual void SetMouseUnavailable() {};
			virtual void SetLeftMouse( bool down ) { down; }
			virtual void SetRightMouse( bool down ) { down; }

			// Keyboard...
			virtual bool KeyState( Key::TYPE key ) const { return false; }
			virtual InputStatus KeyStatus( Key::TYPE key ) const { return InputStatus::STATUS_UP; }
			virtual bool KeyJustDown( Key::TYPE key ) const { return false; }
			virtual bool KeyDown( Key::TYPE key ) const { return false; }
			virtual bool KeyPressed( Key::TYPE key ) const { return false; }
			virtual void ClearKey( Key::TYPE key ) {}
			virtual bool AnyKeyStatus( InputStatus status ) { return false; }

			// Mouse...
			virtual bool IsMouseAvailable( ) const { return false; }
			virtual int MouseChangeX() const { return 0; }
			virtual int MouseChangeY( ) const { return 0; }
			virtual int MouseChangeZ( ) const { return 0; }
			
			virtual int MouseX( ) const { return 0; }
			virtual int MouseY( ) const { return 0; }
			virtual int MouseZ( ) const { return 0; }

			virtual bool MouseJustDown( int iButton ) const { return false; }
			virtual bool MousePressed( int iButton ) const { return false; }
			virtual bool MouseDown( int iButton ) const { return false; }

			virtual bool AnyMouseStatus( InputStatus status ) { return false; }

			// Same as mouse down
			virtual bool MouseButton( int iButton ) { return false; }

			virtual bool Capslock( ) const { return false; }
			virtual bool Numlock( ) const { return false; }
			virtual bool Scrolllock( ) const { return false; }
		};
	}
}
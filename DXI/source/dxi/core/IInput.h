// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Keys.h>
#include <unify/Size.h>
#include <unify/V2.h>

namespace dxi
{
	namespace core
	{
		class IInput
		{
		public:
			virtual ~IInput() {}
			virtual bool IsReady() const = 0;
			virtual void CallBeforeUpdate( const unify::Size< int > resolution, bool fullScreen ) = 0;
			virtual void CallAfterUpdate() = 0;

			// OS feedback...
			virtual void SetMousePosition( unify::V2< int > position ) { position; }
			virtual void SetMouseUnavailable() {};
			virtual void SetLeftMouse( bool down ) { down; }
			virtual void SetRightMouse( bool down ) { down; }


			// Keyboard...
			virtual bool KeyState( Key::TYPE key ) const = 0;
			virtual InputStatus KeyStatus( Key::TYPE key ) const = 0;
			virtual bool KeyJustDown( Key::TYPE key ) const = 0;
			virtual bool KeyDown( Key::TYPE key ) const = 0;
			virtual bool KeyPressed( Key::TYPE key ) const = 0;
			virtual void ClearKey( Key::TYPE key ) = 0;
			virtual bool AnyKeyStatus( InputStatus status ) = 0;

			// Mouse...
			virtual bool IsMouseAvailable() const = 0;
			virtual int MouseChangeX() const = 0;
			virtual int MouseChangeY() const = 0;
			virtual int MouseChangeZ() const = 0;
			
			virtual int MouseX() const = 0;
			virtual int MouseY() const = 0;
			virtual int MouseZ() const = 0;

			template< typename T >
			unify::V2< T > MouseV2() const
			{
				return unify::V2< T >( static_cast< T >( MouseX() ), static_cast< T >( MouseY() ) );
			}

			template< typename T >
			unify::V3< T > MouseV3() const
			{
				return unify::V3< T >( static_cast< T >( MouseX() ), static_cast< T >( MouseY() ), static_cast< T >( MouseZ() ) );
			}

			virtual bool MouseJustDown( int iButton ) const = 0;
			virtual bool MousePressed( int iButton ) const = 0;
			virtual bool MouseDown( int iButton ) const = 0;

			virtual bool AnyMouseStatus( InputStatus status ) = 0;

			// Same as mouse down
			virtual bool MouseButton( int iButton ) = 0;

			virtual bool Capslock() const = 0;
			virtual bool Numlock() const = 0;
			virtual bool Scrolllock() const = 0;
		};
	} // namespace core
} // namespace dxi
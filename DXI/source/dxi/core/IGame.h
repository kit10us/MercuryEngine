// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/core/IInput.h>
#include <unify/TimeDelta.h>

namespace dxi
{
	namespace core
	{
		/// <summary>
		/// A game, inheriting from this class, enables support of the necessary major game events.
		/// </summary>
		class IGame
		{
		public:
			virtual ~IGame() {}

			/// <summary>
			/// Setup is the initial event that is called to setup the game. It is the earliest point to instigate configuration.
			/// </summary>
			virtual bool Setup( IOS * config ) = 0;

			/// <summary>
			/// Startup is called to load assets and setup before Updating and Rendering. It is called once.
			/// </summary>
			virtual void Startup() = 0;

			/// <summary>
			/// Called every game frame, Update allows processing input, and game time for physics and state changes.
			/// </summary>
			virtual bool Update( unify::Seconds elapsed, IInput & input ) = 0;

			virtual void BeforeRender() = 0;
			virtual void Render() = 0;
			virtual void AfterRender() = 0;
			virtual void Shutdown() = 0;

			virtual IOS * GetOS() = 0;
		};
	}
}
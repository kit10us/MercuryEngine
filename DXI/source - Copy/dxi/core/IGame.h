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
		class IGame
		{
		public:
			virtual ~IGame() {}

			virtual bool Setup( IOS & config ) = 0;
			virtual void Startup() = 0;
			virtual bool Update( unify::Seconds elapsed, IInput & input ) = 0;
			virtual void BeforeRender() = 0;
			virtual void Render() = 0;
			virtual void AfterRender() = 0;
			virtual void Shutdown() = 0;

			virtual IOS & GetOS() = 0;
		};
	} // namespace core
} // namespace dxi
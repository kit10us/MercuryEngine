// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IRenderer.h>
#include <unify/unify.h>
#include <unify/Size.h>
#include <vector>
#include <string>
#include <memory>

// Because Windows likes to define...
#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace dxi
{
	namespace core
	{
		/// <summary>
		/// Interface for Operating System specific data. This class is for abstraction from OS.
		/// </summary>
		class IOS
		{
		public:
			virtual ~IOS() { }

			/// <summary>
			/// Get a vector of command line arguments, where 0 is the first argument.
			/// </summary>
			virtual const std::vector< std::string > & GetCommandLine() const = 0;

			/// <summary>
			/// Get the system Renderer interface.
			/// </summary>
			virtual IRenderer * GetRenderer() const = 0;

			/// <summary>
			// Set the resolution.
			/// </summary>
			virtual void SetResolution( const unify::Size< unsigned int > & resolution ) = 0;

			/// <summary>
			/// Set fullscreen mode enabled or disabled.
			/// </summary>
			virtual void SetFullscreen( bool fullscreen ) = 0;

			/// <summary>
			/// Get the resolution.
			/// </summary>
			virtual unify::Size< unsigned int > GetResolution() const = 0;

			/// <summary>
			/// Get the default viewport.
			/// </summary>
			virtual const Viewport & GetDefaultViewport() const = 0;

			/// <summary>
			/// Get fullscreen mode.
			/// </summary>
			virtual bool GetFullscreen() const = 0;

			/// <summary>
			/// Set if we have focus. This is dependant on the OS, focus means the user's input
			/// is directed to us.
			/// </summary>
			virtual void SetHasFocus( bool hasFocus ) = 0;

			/// <summary>
			/// Get if we have focus. This is dependant on the OS, focus means the user's input
			/// is directed to us.
			/// </summary>
			virtual bool GetHasFocus() const = 0;

			/// <summary>
			/// Startup and initalize Operating System specific dependencies.
			/// </summary>
			virtual void Startup() = 0;

			/// <summary>
			/// Cleanup up and unitialize Operating System specific dependencies.
			/// </summary>
			virtual void Shutdown() = 0;

			/// <summary>
			/// Write debug text, no new line.
			/// </summary>
			virtual void DebugWrite( const std::string & text ) = 0;
			
			/// <summary>
			/// Write a debug message.
			/// </summary>
			virtual void DebugWriteLine( const std::string & line ) = 0;
		};
	}
}
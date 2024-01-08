// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/os/IEnvironment.h>
#include <me/render/IRendererFactory.h>
#include <me/render/Display.h>
#include <me/os/IExtension.h>
#include <me/os/OSParameters.h>
#include <me/debug/IDebug.h>
#include <rm/AssetPaths.h>
#include <unify/unify.h>
#include <unify/Size.h>
#include <vector>
#include <string>
#include <memory>

namespace me
{
	namespace game
	{
		class IGame;
	}

	namespace os
	{
		/// <summary>
		/// Interface for Operating System specific data. This class is for abstraction from OS.
		/// </summary>
		class IOS
		{
		public:
			typedef std::shared_ptr< IOS > ptr;

			virtual ~IOS() { }

			virtual IEnvironment::weak_ptr GetEnvironment() const = 0;

			/// <summary>
			/// Get the game instance.
			/// </summary>
			virtual game::IGame * GetGame() = 0;

			/// <summary>
			/// Get the debug interaface.
			/// </summary>
			virtual me::debug::IDebug * Debug() = 0;

			/// <summary>
			/// Get the debug interaface.
			/// </summary>
			virtual const me::debug::IDebug * Debug() const = 0;

			/// <summary>
			/// Feed data to the os. This allows interacing with the operating system without
			/// direct dependencies.
			/// </summary>
			virtual void * Feed( std::string target, void * data ) = 0;

			/// <summary>
			/// Add a display.
			/// </summary>	  
			virtual void AddDisplay( render::Display display ) = 0;

			/// <summary>
			/// Returns the number of renderers.
			/// </summary>
			virtual size_t RendererCount() const = 0;

			/// <summary>
			/// Get the system Renderer interface.
			/// </summary>
			virtual render::IRenderer * GetRenderer( size_t index ) const = 0;

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
			/// Set the render factory.
			/// </summary>
			virtual void SetRenderFactory( me::render::IRendererFactory::ptr renderFactory ) = 0;

			/// <summary>
			/// Build renderers.
			/// </summary>
			virtual void BuildRenderers( std::string title ) = 0;

			/// <summary>
			/// Asset path manager.
			/// </summary>
			virtual rm::AssetPaths::ptr GetAssetPaths() = 0;

			/// <summary>
			/// Returns the OS parameters, parameters passed in to the program from the OS.
			/// </sumamry>
			virtual const os::OSParameters * GetOSParameters() const = 0;

			/// <summary>
			/// Create an extension.
			/// </summary>
			virtual me::os::IExtension::ptr CreateExtension( unify::Path source, const qxml::Element * element ) = 0;

			/// <summary>
			/// Startup and initalize Operating System specific dependencies.
			/// </summary>
			virtual void Startup() = 0;

			/// <summary>
			/// Cleanup up and unitialize Operating System specific dependencies.
			/// </summary>
			virtual void Shutdown() = 0;
		};
	}
}
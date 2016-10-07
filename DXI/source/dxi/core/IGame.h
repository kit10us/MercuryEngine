// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/core/IInput.h>
#include <dxi/scripting/IScriptEngine.h>
#include <unify/TimeDelta.h>
#include <dxi/core/Extension.h>
#include <dxi/RenderInfo.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	namespace core
	{
		/// <summary>
		/// A game, inheriting from this class, enables support of the necessary major game events.
		/// </summary>
		class IGame
		{
		protected:
			/// <summary>
			/// Setup is the initial event that is called to setup the game. It is the earliest point to instigate configuration.
			/// </summary>
			virtual bool Setup( IOS & os ) = 0;

			/// <summary>
			/// Startup is called to load assets and create game objects before Updating and Rendering. It is called once.
			/// </summary>
			virtual void Startup() = 0;

			/// <summary>
			/// Update is called to enable objects to perform over-time operations.
			/// </summary>
			virtual bool Update( RenderInfo & renderInfo, IInput & input ) = 0;

			/// <summary>
			/// Render is called to draw graphics.
			/// </summary>
			virtual void Render( const RenderInfo & renderInfo ) = 0;

		public:
			virtual ~IGame() {}

			virtual void Tick() = 0;

			virtual void Draw() = 0;

			virtual void Shutdown() = 0;

			/// <summary>
			/// Get our OS interface.
			/// </summary>
			virtual IOS & GetOS() = 0;

			/// <summary>
			/// Get our RenderInfo.
			/// </summary>
			virtual const RenderInfo & GetRenderInfo() const = 0;

			/// <summary>
			/// Add a script engine.
			/// </summary>
			virtual void AddScriptEngine( std::string name, std::shared_ptr< scripting::IScriptEngine > se ) = 0;

			/// <summary>
			/// Get a script engine by name.
			/// </summary>
			virtual scripting::IScriptEngine * GetScriptEngine( std::string name ) = 0;

			/// <summary>
			/// Add an extension.
			/// </summary>
			virtual void AddExtension( std::shared_ptr< Extension > extension ) = 0;
		};
	}
}
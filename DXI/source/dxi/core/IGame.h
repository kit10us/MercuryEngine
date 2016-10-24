// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/scripting/IScriptEngine.h>
#include <unify/TimeDelta.h>
#include <dxi/core/Extension.h>
#include <dxi/RenderInfo.h>
#include <dxi/input/InputManager.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGameComponent.h>

namespace dxi
{
	enum class ErrorLevel
	{
		Critical, // System is left unstable, we should likely shutdown immediately.
		Failure, // We have a failure, the module that reported it is likely corrupted.
		Warning // Error might not be important - state is unknown.
	};

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
			virtual bool Setup( IOS * os ) = 0;

			/// <summary>
			/// Startup is called to load assets and create game objects before Updating and Rendering. It is called once.
			/// </summary>
			virtual void Startup() = 0;

			/// <summary>
			/// Update is called to enable objects to perform over-time operations.
			/// </summary>
			virtual bool Update( RenderInfo & renderInfo ) = 0;

			/// <summary>
			/// Render is called to draw graphics.
			/// </summary>
			virtual void Render( int renderer, const RenderInfo & renderInfo, const Viewport & viewport ) = 0;

			/// <summary>
			/// Called once when engine is shutting down, to allow user to release assets that require manual release/destroy.
			/// </summary>
			virtual void Shutdown() = 0;																				  

		public:
			virtual ~IGame() {}

			virtual void Tick() = 0;

			virtual void Draw() = 0;

			/// <summary>
			/// Get our OS interface.
			/// </summary>
			virtual IOS * GetOS() = 0;

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

			virtual void Quit() = 0;

			virtual bool IsQuitting() const = 0;

			virtual input::InputManager * GetInputManager() = 0;
			virtual const input::InputManager * GetInputManager() const = 0;

			/// <summary>
			/// This is our method of reporting issues, especially from modules.
			/// </summary>
			virtual void ReportError( ErrorLevel level, std::string source, std::string error ) = 0;

			virtual bool HadCriticalError() const = 0;

			virtual int ComponentCount() const = 0;
			virtual void AddComponent( IGameComponent::ptr component ) = 0;
			virtual void RemoveComponent( IGameComponent::ptr component ) = 0;
			virtual IGameComponent::ptr GetComponent( int index ) = 0;
			virtual IGameComponent::ptr GetComponent( std::string name, int startIndex ) = 0 ;
			virtual int FindComponent( std::string name, int startIndex ) const = 0;		

		};
	}
}
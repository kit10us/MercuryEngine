// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/IScriptEngine.h>
#include <unify/TimeDelta.h>
#include <me/Extension.h>
#include <me/RenderInfo.h>
#include <me/InputManager.h>
#include <rm/ResourceManagerSimple.h>
#include <me/IGameComponent.h>

namespace me
{
	enum class ErrorLevel
	{
		Critical, // System is left unstable, we should likely shutdown immediately.
		Failure, // We have a failure, the module that reported it is likely corrupted.
		Warning // Error might not be important - state is unknown.
	};

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
		virtual void Update( RenderInfo & renderInfo ) = 0;

		/// <summary>
		/// Render is called to draw graphics.
		/// </summary>
		virtual void Render( const RenderInfo & renderInfo, const me::Viewport & viewport ) = 0;

		/// <summary>
		/// Called once when engine is shutting down, to allow user to release assets that require manual release/destroy.
		/// </summary>
		virtual void Shutdown() = 0;																				  

	public:
		virtual ~IGame() {}

		virtual OSParameters GetOSParameters() const = 0;

		/// <summary>
		/// Feed messages to game, and it's sub-systems.
		/// </summary>
		virtual void * Feed( std::string target, void * data ) = 0;

		virtual void Tick() = 0;

		virtual void Draw() = 0;

		virtual void SetOS( me::IOS::ptr os ) = 0;

		/// <summary>
		/// Get our OS interface.
		/// </summary>
		virtual IOS * GetOS() = 0;

		/// <summary>
		/// Get our RenderInfo.
		/// </summary>
		virtual const RenderInfo & GetRenderInfo() const = 0;

		virtual void Quit() = 0;

		virtual bool IsQuitting() const = 0;

		virtual InputManager * GetInputManager() = 0;
		virtual const InputManager * GetInputManager() const = 0;

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


	/// <summary>
	/// Helper function to get a specific game component.
	/// </summary>
	template< typename T >
	T GetGameComponent( IGame * game, std::string name )
	{
		IGameComponent::ptr gc = game->GetComponent( name, 0 );
		if( !gc )
		{
			game->ReportError( ErrorLevel::Critical, "Mercury", "Couldn't find game component " + name + "!" );
			return T();
		}

		T c = dynamic_cast<T>(gc.get());
		if( !c )
		{
			game->ReportError( ErrorLevel::Critical, "Mercury", "Component " + name + " found, however, not expected type or version!" );
			return T();
		}
			
		return c;
	}
}
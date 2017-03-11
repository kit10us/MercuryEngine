// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IComponent.h>
#include <me/UpdateParams.h>
#include <me/RenderParams.h>
#include <string>
#include <memory>
					 
namespace me
{
	class IGame;

	/// <summary>
	/// Game components are components with global game life; they influnce the entire game.
	/// </summary>
	class IGameComponent : public IComponent
	{	
	public:
		typedef std::shared_ptr< IGameComponent > ptr;

		virtual ~IGameComponent() {}
									 
		/// <summary>
		/// Access to the game.
		/// </summary>
		virtual IGame * GetGame() = 0;

		/// <summary>
		/// Access to the game.
		/// </summary>
		virtual  const IGame * GetGame() const = 0;

		/// <summary>
		/// Called first, upon being attached to the game.
		/// </summary>
		virtual void OnAttach( IGame * game ) = 0;

		/// <summary>
		/// Called before our game's Startup.
		/// </summary>
		virtual void OnBeforeStartup() = 0;
		
		/// <summary>
		///  Called after our game's Startup.
		/// </summary>
		virtual void OnAfterStartup( ) = 0;

		/// <summary>
		/// Called during game updating.
		/// </summary>
		virtual void OnUpdate( UpdateParams params ) = 0;

		/// <summary>
		/// Called during game rendering.
		/// </summary>
		virtual void OnRender( RenderParams params ) = 0;

		/// <summary>
		/// Called last, to detach from the game.
		/// </summary>
		virtual void OnDetach( IGame * game ) = 0;
	};
}
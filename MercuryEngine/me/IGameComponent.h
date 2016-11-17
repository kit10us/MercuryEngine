// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>
#include <string>
#include <memory>
					 
namespace me
{
	class IGame;

	/// <summary>
	/// Game components are components with global game life; they influnce the entire game.
	/// </summary>
	class IGameComponent
	{	
	public:
		typedef std::shared_ptr< IGameComponent > ptr;

		virtual ~IGameComponent() {}

		virtual std::string GetName() const = 0;

		virtual void OnAttach( IGame * game ) = 0;

		virtual void OnUpdate( IGame * game, const IRenderer * renderer, const RenderInfo & renderInfo ) = 0;

		virtual void OnRender( IGame * game, const IRenderer * renderer, const RenderInfo & renderInfo ) = 0;

		virtual void OnDetach( IGame * game ) = 0;
	};
}
// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <memory>
					 
namespace dxi
{
	namespace core
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

			virtual void OnDetach( IGame * game ) = 0;
		};
	}
}
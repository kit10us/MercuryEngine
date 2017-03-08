// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGameComponent.h>

namespace me
{
	/// <summary> 
	/// Game data manager.
	/// </summary>
	class DataCore : public IGameComponent
	{
	public:
		DataCore();
		virtual ~DataCore();

		std::string GetName() const override;

		void OnAttach( IGame * game ) override;

		void OnBeforeStartup( IGame * game ) override;

		void OnAfterStartup( IGame * game ) override;

		void OnUpdate( IGame * game, UpdateParams params ) override;

		void OnRender( IGame * game, RenderParams params ) override;

		void OnDetach( IGame * game ) override;
	};
}
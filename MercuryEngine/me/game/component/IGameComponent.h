// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/IComponent.h>
#include <me/action/IAction.h>
#include <me/input/IInputAction.h>
#include <me/object/action/IObjectAction.h>
#include <me/UpdateParams.h>
#include <me/render/RenderParams.h>
#include <qxml/Element.h>
#include <string>
#include <memory>

namespace me
{
	namespace game
	{
		class IGame;

		namespace component
		{
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
				virtual void OnAttach( game::IGame * gameInstance ) = 0;

				/// <summary>
				/// Called before our game's Startup.
				/// </summary>
				virtual void OnBeforeStartup() = 0;

				/// <summary>
				///  Called after our game's Startup.
				/// </summary>
				virtual void OnAfterStartup() = 0;

				/// <summary>
				/// Called before OnUpdate.
				/// </summary>
				virtual void OnEarlyUpdate( const UpdateParams & params ) = 0;

				/// <summary>
				/// Called during game updating.
				/// </summary>
				virtual void OnUpdate( const UpdateParams & params ) = 0;

				/// <summary>
				/// Called after on OnUpdate.
				/// </summary>
				virtual void OnLateUpdate( const UpdateParams & params ) = 0;

				/// <summary>
				/// Called during game rendering.
				/// </summary>
				virtual void OnRender( const render::Params & params ) = 0;

				/// <summary>
				/// Called last, to detach from the game.
				/// </summary>
				virtual void OnDetach( game::IGame * gameInstance ) = 0;

				/// <summary>
				/// Create an Action from an XML node.
				/// </summary>
				virtual action::IAction::ptr CreateAction( const qxml::Element * element ) = 0;

				/// <summary>
				/// Create an Object Action from an XML node.
				/// </summary>
				virtual object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element * element ) = 0;

				/// <summary>
				/// Create an Input Action from an XML node.
				/// </summary>
				virtual input::IInputAction::ptr CreateInputAction( const qxml::Element * element ) = 0;

				/// <summary>
				/// Receive SendCommand events directly. Best to subscribe, but this is a more direct way that allows other features to work easier.
				/// </summary>
				virtual std::string SendCommand( size_t id, std::string extra ) = 0;
			};
		}
	}
}
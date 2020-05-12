// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/game/component/IGameComponent.h>

namespace me
{
	namespace game
	{
		namespace component
		{
			class GameComponent : public IGameComponent
			{
				IGame* m_game;
				std::string m_typeName;
				bool m_enabled;
				interop::Interop m_values;
				std::map< std::string, me::IThing*, unify::string::CaseInsensitiveLessThanTest > m_interfaceMap;
				kit::debug::IBlock::ptr m_block;

			public:
				GameComponent( std::string name );
				~GameComponent();

			protected:

				/// <summary>
				/// Returns the debug script block for this game component.
				/// </summary>
				kit::debug::IBlock* GetBlock();

			public:	 // IGameComponent...
				IGame * GetGame() override;
				const IGame * GetGame() const override;
				void OnAttach( game::IGame * gameInstance ) override;
				void OnBeforeStartup() override;
				void OnAfterStartup() override;
				void OnEarlyUpdate( const UpdateParams & params ) override;
				void OnUpdate( const UpdateParams & params ) override;
				void OnLateUpdate( const UpdateParams & params ) override;
				void OnRender( const render::Params & params ) override;
				void OnDetach( game::IGame * gameInstance ) override;
				action::IAction::ptr CreateAction( const qxml::Element * element ) override;
				object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element * element ) override;
				input::IInputAction::ptr CreateInputAction( const qxml::Element * element ) override;
				std::string SendCommand( size_t id, std::string extra ) override;

			public:	// me::IComponent...
				bool GameComponent::IsEnabled() const override;
				void GameComponent::SetEnabled( bool enabled ) override;
				interop::Interop * GetLookup() override;
				const interop::Interop * GetLookup() const override;

			protected: // me::IThing...
				void AddInterface( std::string name, me::IThing* ptr ) override;

			public: // me::IThing...
				std::string GetTypeName() const override;
				me::IThing* QueryInterface( std::string name ) override;
				std::string GetWhat() const override;
			};
		}
	}
}
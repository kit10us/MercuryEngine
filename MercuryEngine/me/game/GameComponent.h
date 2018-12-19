// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/game/IGameComponent.h>
#include <me/game/IGame.h>

namespace me
{
	namespace game
	{
		class GameComponent : public IGameComponent
		{
		public:
			GameComponent( std::string type );
			~GameComponent();

		protected:
			void AddInterface( std::string name, me::IThing* ptr );

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

		public:	// IComponent...
			bool GameComponent::IsEnabled() const;

			void GameComponent::SetEnabled( bool enabled );

			interop::Interop * GetLookup() override;
			const interop::Interop * GetLookup() const override;

		public: // me::IThing...
			std::string GetTypeName() const override;
			me::IThing* QueryInterface( std::string name ) override;
			std::string GetWhat() const override;

		protected:
			IGame * m_game;
			std::string m_typeName;
			bool m_enabled;
			interop::Interop m_values;
			std::map< std::string, me::IThing*, unify::string::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}
// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGameComponent.h>
#include <me/IGame.h>

namespace me
{
	class GameComponent : public IGameComponent
	{
	public:
		GameComponent( std::string type );
		~GameComponent();

		Lookup * GetLookup();
		const Lookup * GetLookup() const;

	protected:
		void AddInterface( std::string name, IUnknown* ptr );

	public:	 // IGameComponent...
		IGame * GetGame() override;
		const IGame * GetGame() const override;
		void OnAttach( IGame * game ) override;
		void OnBeforeStartup() override;
		void OnAfterStartup() override;
		void OnUpdate( UpdateParams params ) override;
		void OnRender( RenderParams params ) override;
		void OnDetach( IGame * game ) override;

	public:	// IComponent...
		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;
		int GetValueCount() const override;
		bool ValueExists( std::string ) const override;
		std::string GetValueName( int index ) const override;
		int FindValueIndex( std::string name ) const override;
		std::string GetValue( int index ) const override;
		std::string GetValue( std::string name ) const override;
		bool SetValue( int index, std::string value ) override;
		bool SetValue( std::string name, std::string value ) override;

	public: // IUnknown...
		std::string GetTypeName() const override;
		IUnknown* QueryInterface( std::string name ) override;

	protected:
		IGame * m_game;
		std::string m_typeName;
		bool m_enabled;
		Lookup m_values;
		std::map< std::string, IUnknown*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
	};
}
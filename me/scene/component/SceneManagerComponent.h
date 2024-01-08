// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/component/ISceneManagerComponent.h>
#include <me/scene/SceneManager.h>

namespace me::scene::component
{
	/// <summary>
	/// Default Scene Manager Component.
	/// </summary>
	class SceneManagerComponent : public ISceneManagerComponent
	{
		std::string m_typeName;
		bool m_enabled;
		interop::Interop m_values;
		std::map< std::string, me::IThing*, unify::string::CaseInsensitiveLessThanEqualTest > m_interfaceMap;
		SceneManager* m_sceneManager;

	protected:
		kit::debug::IBlock::ptr m_block;

		SceneManagerComponent( std::string typeName );
		void AddInterface( std::string name, me::IThing* ptr ) override;

	public: // me::scene::component::ISceneManagerComponent...
		ISceneComponent::ptr CreateSceneComponent( std::string type ) override;
		void OnAttach( SceneManager* sceneManager ) override;
		void OnDetach( SceneManager* sceneManager ) override;
		void OnSceneStart( IScene* scene ) override;
		void OnSceneEnd( IScene* scene ) override;
		SceneManager* GetSceneManager() override;

	public:	// me::IComponent...
		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;
		interop::Interop* GetLookup() override;
		const interop::Interop* GetLookup() const override;

	public: // me::IThing...
		std::string GetTypeName() const override;
		me::IThing* QueryInterface( std::string name ) override;
		std::string GetWhat() const override;
	};
}

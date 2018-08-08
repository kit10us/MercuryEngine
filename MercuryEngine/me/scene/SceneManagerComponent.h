// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/ISceneManagerComponent.h>
#include <me/scene/SceneManager.h>

namespace me
{
	namespace scene
	{
		class SceneManagerComponent : public ISceneManagerComponent
		{
		protected:
			SceneManagerComponent(std::string typeName );

			void AddInterface(std::string name, me::IThing* ptr);

		public: // ISceneManagerComponent...
			void OnAttach( SceneManager * sceneManager ) override;
			void OnDetach( SceneManager * sceneManager ) override;
			void OnSceneStart( IScene * scene ) override;
			void OnSceneEnd( IScene * scene ) override;

		public:	// IComponent...
			bool IsEnabled() const override;
			void SetEnabled(bool enabled) override;

			int GetValueCount() const override;
			bool ValueExists(std::string) const override;
			std::string GetValueName(int index) const override;
			int FindValueIndex(std::string name) const override;
			std::string GetValue(int index) const override;
			bool SetValue(int index, std::string value) override;

			// These are automatic from the above functions - don't bother overriding them further.
			std::string GetValue( std::string name ) const override;
			bool SetValue(std::string name, std::string value) override;

		public: // me::IThing...
			std::string GetTypeName() const override;
			me::IThing* QueryInterface(std::string name) override;
			std::string GetWhat() const override;

		private:
			std::string m_typeName;
			bool m_enabled;
			Lookup m_values;
			std::map< std::string, me::IThing*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}
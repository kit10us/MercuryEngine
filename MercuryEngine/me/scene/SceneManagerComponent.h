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

			interop::Interop * GetLookup() override;
			const interop::Interop * GetLookup() const override;

		public: // me::IThing...
			std::string GetTypeName() const override;
			me::IThing* QueryInterface(std::string name) override;
			std::string GetWhat() const override;

		private:
			std::string m_typeName;
			bool m_enabled;
			interop::Interop m_values;
			std::map< std::string, me::IThing*, unify::string::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}
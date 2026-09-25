// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/component/ISceneComponent.h>
#include <me/scene/IScene.h>
#include <me/os/IOS.h>

namespace me
{
	namespace scene
	{
		namespace component
		{
			class SceneComponent : public ISceneComponent
			{
			public:
				SceneComponent( os::IOS * os, std::string typeName );
				~SceneComponent();

				os::IOS * GetOS();
				const os::IOS * GetOS() const;

				IScene* GetScene();
				const IScene* GetScene() const;

			protected:
				void AddInterface( std::string name, me::IThing* ptr ) override;

			public: // ISceneComponent...
				unify::Result<> OnAttach( me::scene::IScene * scene ) override;
				unify::Result<> OnDetach( me::scene::IScene * scene ) override;
				unify::Result<> BeforeOnStart() override;
				unify::Result<> AfterOnStart() override;
			
				unify::Result<> BeforeOnUpdate( const UpdateParams & params ) override;
				unify::Result<> OnUpdate( const UpdateParams & params ) override;
				unify::Result<> AfterOnUpdate( const UpdateParams & params ) override;

				void CollectCameras( RenderGirl & renderGirl ) override;
				unify::Result<> OnRender( RenderGirl & renderGirl ) override;
				unify::Result<> OnSuspend() override;
				unify::Result<> OnResume() override;
				unify::Result<> OnEnd() override;

			public:	// IComponent...
				bool IsEnabled() const override;
				void SetEnabled( bool enabled ) override;
			
				interop::Interop * GetLookup() override;
				const interop::Interop * GetLookup() const override;

			public: // me::IThing...
				std::string GetTypeName() const override;
				me::IThing* QueryInterface( std::string name ) override;
				std::string GetWhat() const override;

			private:
				os::IOS * m_os;
				std::string m_typeName;
				bool m_enabled;
				IScene* m_scene;
				interop::Interop m_values;
				std::map< std::string, me::IThing*, unify::String::CaseInsensitiveLessThanEqualTest > m_interfaceMap;
			};
		}
	}
}
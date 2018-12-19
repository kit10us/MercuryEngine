// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/scene/IScene.h>
#include <me/os/IOS.h>

namespace me
{
	namespace scene
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
			void AddInterface( std::string name, me::IThing* ptr );

		public: // ISceneComponent...
			void OnAttach( me::scene::IScene * scene ) override;
			void OnDetach( me::scene::IScene * scene ) override;
			void OnBeforeStart() override;
			void OnAfterStart() override;
			
			void OnEarlyUpdate( const UpdateParams & params ) override;
			void OnUpdate( const UpdateParams & params ) override;
			void OnLateUpdate( const UpdateParams & params ) override;

			void CollectCameras( RenderGirl & renderGirl ) override;
			void OnRender( RenderGirl & renderGirl ) override;
			void OnSuspend() override;
			void OnResume() override;
			void OnEnd() override;

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
			std::map< std::string, me::IThing*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}
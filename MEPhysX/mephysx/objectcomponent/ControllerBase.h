// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/SceneComponent.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class ControllerBase : public me::scene::IObjectComponent
		{
		protected:
			ControllerBase( ControllerBase & controller );

		public:
			ControllerBase( me::IOS * os, SceneComponent * gameComponent );
			~ControllerBase();

			me::IOS * GetOS();
			const me::IOS * GetOS() const;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Updateable() const { return false; }
			bool Renderable() const { return false; }

			MEPHYSX_API physx::PxController * GetController();
			MEPHYSX_API const physx::PxController * GetController() const;

			MEPHYSX_API SceneComponent * GetScene();
			MEPHYSX_API const SceneComponent * GetScene() const;

			// Snubbed overrides...
			void OnDetach() override {}
			void OnInit() override {}
			void OnStart() override {}
			void OnUpdate( me::UpdateParams params ) override {}
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

			int GetValueCount() const override;
			bool ValueExists( std::string ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			std::string GetValue( std::string name ) const override;
			bool SetValue( int index, std::string value ) override;
			bool SetValue( std::string name, std::string value ) override;

		private:
			me::IOS * m_os;
			bool m_enabled;

		protected:
			SceneComponent * m_sceneComponent;
			std::shared_ptr< physx::PxController > m_controller;
		};
	}
}
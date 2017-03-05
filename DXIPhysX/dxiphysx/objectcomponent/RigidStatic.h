// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <me/scene/IObjectComponent.h>
#include <me/IOS.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class RigidStatic : public me::scene::IObjectComponent
		{
		protected:
			RigidStatic( RigidStatic & rigidStatic );

		public:
			DXIPHYSX_API RigidStatic( me::IOS * os, GameComponent * gameComponent );
			~RigidStatic();

			me::IOS * GetOS();
			const me::IOS * GetOS() const;

			std::string GetType() const override;
			std::string GetWhat() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Updateable() const { return false; }
			bool Renderable() const { return false; }
									   
			void OnAttach( me::scene::Object * object ) override;
			void OnDetach() override {}
			void OnInit() override {}
			void OnStart() override {}
			void OnUpdate( me::UpdateParams params ) override {}
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

			me::scene::IObjectComponent * Duplicate() override;

			DXIPHYSX_API physx::PxRigidStatic * GetRigidStatic();
			const physx::PxRigidStatic * GetRigidStatic() const;

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
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxRigidStatic > m_rigidStatic;
		};
	}
}
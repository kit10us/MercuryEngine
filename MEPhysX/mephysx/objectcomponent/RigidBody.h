// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/GameComponent.h>
#include <me/scene/IObjectComponent.h>
#include <me/IOS.h>
#include <PxPhysicsAPI.h>
#include <mephysx/Releaser.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class RigidBody : public me::scene::IObjectComponent
		{
		protected:
			RigidBody( RigidBody & rigidBody );

		public:
			MEPHYSX_API RigidBody( me::IOS * os, GameComponent * gameComponent );
			~RigidBody();

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

			MEPHYSX_API physx::PxRigidBody * GetRigidBody();
			const physx::PxRigidBody * GetRigidBody() const;

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
			std::shared_ptr< physx::PxRigidBody > m_rigidBody;
		};
	}
}
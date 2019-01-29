// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <me/object/component/ObjectComponent.h>
#include <me/render/GeometryCache.h>
#include <mephysx/GameComponent.h>
#include <me/os/IOS.h>
#include <PxPhysicsAPI.h>
#include <mephysx/Releaser.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class ColliderBase : public me::object::ObjectComponent
		{
		protected:
			ColliderBase( ColliderBase & colliderBase );

		public:
			ColliderBase( std::string typeName, GameComponent * gameComponent );
			~ColliderBase();

		public: // IObjectComponent...
			void OnAttach( me::object::Object * object ) override;
			void OnDetach( me::object::Object * object ) override;
			void OnStart() override {}
			void OnUpdate( me::UpdateParams params ) override {}
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

			MEPHYSX_API physx::PxShape * GetShape();
			MEPHYSX_API const physx::PxShape * GetShape() const;

		protected:
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxShape > m_shape;
			std::shared_ptr< physx::PxMaterial > m_material;
		};
	}
}
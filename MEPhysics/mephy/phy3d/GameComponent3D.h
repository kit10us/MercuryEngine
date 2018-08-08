// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEPhysics.h>
#include <me/game/GameComponent.h>
#include <me/object/component/IObjectComponent.h>
#include <mephy/phy3d/collider/ColliderBase3D.h>

namespace mephy
{
	namespace phy3d
	{
		class GameComponent : public me::game::GameComponent
		{
		public:
			MEPHYSICS_API static char* Name();

			GameComponent();
			~GameComponent();

		public:
			MEPHYSICS_API me::object::component::IObjectComponent::ptr CreateBoxCollider( unify::V3< float > halfExt, bool moveable = false, float mass = 0.0f );

		public: // IGameComponent...

		public: // IComponent...

		private:
		};
	}
}

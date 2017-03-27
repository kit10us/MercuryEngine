// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/GameComponent.h>
#include <me/object/IObjectComponent.h>
#include <mephy/collider/ColliderBase.h>

namespace mephy
{
	class GameComponent : public me::GameComponent
	{
	public:
		MEPHYSICS_API static char* Name();

		GameComponent();
		~GameComponent();

	public:
		MEPHYSICS_API me::object::IObjectComponent::ptr CreateBoxCollider( unify::V3< float > halfExt );
		MEPHYSICS_API me::object::IObjectComponent::ptr CreateEntity();

	public: // IGameComponent...

	public: // IComponent...

	private:
	};
}

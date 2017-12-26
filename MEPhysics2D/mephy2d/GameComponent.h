// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics2D.h>
#include <me/game/GameComponent.h>
#include <me/object/component/IObjectComponent.h>
#include <mephy2d/collider/ColliderBase.h>

namespace mephy
{
	class GameComponent : public me::game::GameComponent
	{
	public:
		MEPHYSICS2D_API static char* Name();

		GameComponent();
		~GameComponent();

	public:
		MEPHYSICS2D_API me::object::IObjectComponent::ptr CreateRectangleCollider( unify::V2< float > halfExt );
		MEPHYSICS2D_API me::object::IObjectComponent::ptr CreateCircleCollider( float radius );

	public: // IGameComponent...

	public: // IComponent...

	private:
	};
}

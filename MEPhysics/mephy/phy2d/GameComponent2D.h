// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/game/GameComponent.h>
#include <me/object/component/IObjectComponent.h>

namespace mephy
{
	namespace phy2d
	{
		class GameComponent : public me::game::GameComponent
		{
		public:
			/*MEPHYSICS_API*/ static char* Name();

			GameComponent();
			~GameComponent();

		public: // IGameComponent...

		public: // IComponent...

		private:
		};
	}
}

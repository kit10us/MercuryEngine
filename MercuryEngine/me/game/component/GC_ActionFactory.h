// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/GameComponent.h>

namespace me
{
	namespace game
	{
		namespace component
		{
			class ActionFactory : public me::GameComponent
			{
			public:
				ActionFactory();
				~ActionFactory();

			public:	 // IGameComponent...
				action::IAction::ptr CreateAction(const qxml::Element * element) override;
				object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element * element ) override;
				input::IInputAction::ptr CreateInputAction( const qxml::Element * element ) override;

			public:	// IComponent...

			public: // me::IThing...

			protected:
			};
		}
	}
}
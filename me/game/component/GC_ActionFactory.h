// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/game/component/GameComponent.h>

namespace me
{
	namespace game
	{
		namespace component
		{
			class ActionFactory : public GameComponent
			{
			public:
				static const char* Name();

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
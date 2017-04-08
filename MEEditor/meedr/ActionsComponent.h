// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/GameComponent.h>

namespace meedr
{
	class ActionsGameComponent : public me::GameComponent
	{
	public:
		static char* Name();

		ActionsGameComponent();

	public: // IGameComponent...
		me::action::IAction::ptr CreateAction(const qxml::Element * node) override;

	public: // IComponent...
		std::string GetWhat() const override;
	};
}

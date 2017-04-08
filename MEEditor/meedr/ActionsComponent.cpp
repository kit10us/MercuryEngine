// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <meedr/ActionsComponent.h>
#include <meedr/OpenEditorAction.h>

using namespace meedr;

char* ActionsGameComponent::Name()
{
	return "ActionsGameComponent";
}


ActionsGameComponent::ActionsGameComponent()
	: GameComponent( Name() )
{
}

me::action::IAction::ptr ActionsGameComponent::CreateAction(const qxml::Element * node)
{
	if ( unify::StringIs( node->GetAttribute< std::string >("name"), "OpenEditor" ) )
	{
		return me::action::IAction::ptr(new OpenEditorAction());
	}

	return me::action::IAction::ptr();
}

std::string ActionsGameComponent::GetWhat() const
{
	return "MEEditor";
}

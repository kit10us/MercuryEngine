// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ActionsComponent.h>
#include <meedr/OpenEditorAction.h>

using namespace meedr;

ActionsGameComponent::ActionsGameComponent()
	: GameComponent( "ActionsGameComponent" )
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

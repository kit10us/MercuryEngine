// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/input/InputType.h>

using namespace me;
using namespace input;

std::string me::input::InputTypeToString( InputType type )
{
	switch ( type )
	{
	default:
	case InputType::Invalid: return "Invalid";
	case InputType::Button: return "Button";
	case InputType::Toggle: return "Toggle";
	case InputType::Tracker: return "Tracker";
	case InputType::Trigger: return "Trigger";
	case InputType::Motion: return "Motion";
	case InputType::Stick: return "Stick";
	}
}

InputType me::input::StringToInputType(std::string type)
{
	if (unify::StringIs(type, "Button"))
	{
		return InputType::Button;
	}
	else if (unify::StringIs(type, "Toggle"))
	{
		return InputType::Toggle;
	}
	else if (unify::StringIs(type, "Tracker"))
	{
		return InputType::Tracker;
	}
	else if (unify::StringIs(type, "Trigger"))
	{
		return InputType::Trigger;
	}
	else if (unify::StringIs(type, "Motion"))
	{
		return InputType::Motion;
	}
	else if (unify::StringIs(type, "Stick"))
	{
		return InputType::Stick;
	}
	else
	{
		return InputType::Invalid;
	}
}

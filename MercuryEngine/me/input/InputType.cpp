// Copyright (c) 2003 - 2013, Quentin S. Smith
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
	case InputType::Pad: return "Pad";
	}
}

// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#include <meedr/OpenEditorAction.h>

using namespace meedr;

OpenEditorAction::OpenEditorAction()
{
}

bool OpenEditorAction::Perform()
{
	OpenEditor();
	return true;
}

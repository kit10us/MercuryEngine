// Copyright (c) 2002 - 2018, Quentin S. Smith
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

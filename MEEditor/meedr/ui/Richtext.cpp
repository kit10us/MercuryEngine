// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Richtext.h>
#include <Richedit.h>

using namespace meedr;
using namespace ui;

Richtext::Richtext( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Richtext::~Richtext()
{
}

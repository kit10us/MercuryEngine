// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <ui/Richtext.h>
#include <Richedit.h>

using namespace ui;

Richtext::Richtext( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Richtext::~Richtext()
{
}

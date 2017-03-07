// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Edit.h>

using namespace meedr;
using namespace ui;

Edit::Edit( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Edit::~Edit()
{
}

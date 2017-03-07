// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Treeview.h>
#include <Commctrl.h>

#pragma comment( lib, "Comctl32" )

using namespace meedr;
using namespace ui;

Treeview::Treeview( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Treeview::~Treeview()
{
}
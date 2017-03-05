// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/container/Container.h>
#include <cassert>

using namespace meedr;
using namespace ui;
using namespace container;

Container::Container( int width, int height )
	: Control( width, height )
{
}

Container::~Container()
{
}

HWND Container::GetParentHandle() const
{
	assert( 0 );
	return 0;
}

DWORD Container::GetWantedStyle() const
{
	assert( 0 );
	return 0;
}

std::wstring Container::GetType() const
{
	assert( 0 );
	return L"";
}
// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <create/container/Create_Container.h>
#include <cassert>

using namespace create;
using namespace container;

Container::Container( int width, int height )
	: Control( width, height )
	, m_parent{ nullptr }
{
}

Container::~Container()
{
}

void Container::SetParent( Container * parent )
{
	m_parent = parent;
}

Container * Container::GetParent()
{
	return m_parent;
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
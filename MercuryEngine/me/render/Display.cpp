// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/Display.h>

using namespace me;
using namespace render;

Display Display::CreateWindowedDirectXDisplay( unify::Size< float > size, unify::V2< float > position, void* handle, void* parent )
{
	Display display;
	display.m_handle = handle;
	display.m_parentHandle = parent;
	display.m_position = position;
	display.m_size = size;
	display.m_isFullscreen = false;
	return display;
}

Display Display::CreateFullscreenDirectXDisplay( unify::Size< float > size, void* handle )
{
	Display display;
	display.m_handle = handle;
	display.m_parentHandle = 0;
	display.m_position = unify::V2< float >(); // What ever - ignored later.
	display.m_size = size;
	display.m_isFullscreen = true;
	return display;
}

Display::Display()
: m_handle {}
, m_parentHandle {}
, m_position {}
, m_size {}
, m_isFullscreen {}
, m_nearZ{ 0.0f }
, m_farZ{ 1000.0f }
{
}

bool Display::IsFullscreen() const
{
	return m_isFullscreen;
}

unify::Size< float > Display::GetSize() const
{
	return m_size;
}

unify::V2< float > Display::GetPosition() const
{
	return m_position;
}

float Display::GetNearZ() const
{
	return m_nearZ;
}

float Display::GetFarZ() const
{
	return m_farZ;
}

void Display::SetNearZ( float z )
{
	m_nearZ = z;
}

void Display::SetFarZ( float z )
{
	m_farZ = z;
}

void* Display::GetHandle() const
{
	return m_handle;
}

void Display::SetHandle( void* handle )
{
	m_handle = handle;
}

void* Display::GetParentHandle() const
{
	return m_parentHandle;
}

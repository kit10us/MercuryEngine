// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Richtext.h>

using namespace meedr;
using namespace ui;

Richtext::Richtext( FillWidth, FillHeight, int id )
	: Control( id, FillWidth::Value(), FillHeight::Value() )
{
}
Richtext::Richtext( FillWidth, int height, int id )
	: Control( id, FillWidth::Value(), height )
{
}

Richtext::Richtext( int width, FillHeight, int id )
	: Control( id, width, FillHeight::Value() )
{
}
Richtext::Richtext( int width, int height, int id )
	: Control( id, width, height )
{
}

Richtext::~Richtext()
{
}

std::wstring Richtext::GetText()
{
	return m_text;
}

void Richtext::Create( HWND parent )
{
	HWND handle = CreateWindowW(
		L"Richtext",
		m_text.c_str(),
		WS_VISIBLE | WS_CHILD,
		GetActualX(),
		GetActualY(),
		GetActualWidth(),
		GetActualHeight(),
		parent,
		(HMENU)GetID(),
		0,
		0
	);
}

int Richtext::GetDefaultWidth() const
{

	return 160;
}

int Richtext::GetDefaultHeight() const
{
	return 20;
}

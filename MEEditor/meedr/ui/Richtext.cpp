// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Richtext.h>
#include <Richedit.h>

using namespace meedr;
using namespace ui;

Richtext::Richtext( FillWidth fillWidth, FillHeight fillHeight )
	: Control(FillWidth::Value(), FillHeight::Value() )
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
}
Richtext::Richtext( FillWidth fillWidth, int height )
	: Control(FillWidth::Value(), height )
{
	SetFillWidthWeight( fillWidth.weight );
}

Richtext::Richtext( int width, FillHeight fillHeight )
	: Control(width, FillHeight::Value() )
{
	SetFillHeightWeight( fillHeight.weight );
}
Richtext::Richtext( int width, int height )
	: Control(width, height )
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
	LoadLibrary( TEXT( "Msftedit.dll" ) );
	HWND handle = CreateWindowExW(
		0,
		MSFTEDIT_CLASS,
		m_text.c_str(),
		WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
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

	return 320;
}

int Richtext::GetDefaultHeight() const
{
	return 160;
}

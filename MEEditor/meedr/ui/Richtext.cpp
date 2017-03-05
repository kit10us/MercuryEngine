// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Richtext.h>
#include <Richedit.h>

using namespace meedr;
using namespace ui;

Richtext::Richtext( FillWidth fillWidth, FillHeight fillHeight )
	: Control(FillWidth::Value(), FillHeight::Value() )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ true }
	, m_wantedReadonly{ false }
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
	LoadLibrary( TEXT( "Msftedit.dll" ) );
}
Richtext::Richtext( FillWidth fillWidth, int height )
	: Control(FillWidth::Value(), height )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ true }
	, m_wantedReadonly{ false }
{
	SetFillWidthWeight( fillWidth.weight );
	LoadLibrary( TEXT( "Msftedit.dll" ) );
}

Richtext::Richtext( int width, FillHeight fillHeight )
	: Control(width, FillHeight::Value() )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ true }
	, m_wantedReadonly{ false }
{
	SetFillHeightWeight( fillHeight.weight );
	LoadLibrary( TEXT( "Msftedit.dll" ) );
}
Richtext::Richtext( int width, int height )
	: Control(width, height )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ true }
	, m_wantedReadonly{ false }
{
	LoadLibrary( TEXT( "Msftedit.dll" ) );
}

Richtext::~Richtext()
{
}

Richtext* Richtext::SetWantedAutoHScroll( bool autohscroll )
{
	if ( autohscroll && !GetWantedHScroll() )
	{
		SetWantedHScroll( true );
	}

	m_wantedAutoHScroll = autohscroll;
	return this;
}

bool Richtext::GetWantedAutoHScroll() const
{
	return m_wantedAutoHScroll;
}

Richtext* Richtext::SetWantedAutoVScroll( bool autovscroll )
{
	if ( autovscroll && !GetWantedVScroll() )
	{
		SetWantedVScroll( true );
	}

	m_wantedAutoVScroll = autovscroll;
	return this;
}

bool Richtext::GetWantedAutoVScroll() const
{
	return m_wantedAutoVScroll;
}

Richtext* Richtext::SetWantedMultiline( bool multiline )
{
	m_wantedMulitline = multiline;
	return this;
}

bool Richtext::GetWantedMultiline() const
{
	return m_wantedMulitline;
}

Richtext* Richtext::SetWantedReadonly( bool readonly )
{
	m_wantedReadonly = readonly;
	return this;
}

bool Richtext::GetWantedReadonly() const
{
	return m_wantedReadonly;
}

DWORD Richtext::GetWantedStyle() const
{
	DWORD style = Control::GetWantedStyle();
	style |= WS_BORDER | WS_TABSTOP;
	if ( GetWantedHScroll() )
	{
		style |= WS_HSCROLL;
	}
	if ( GetWantedVScroll() )
	{
		style |= WS_VSCROLL;
	}
	if ( GetWantedAutoHScroll() )
	{
		style |= ES_AUTOHSCROLL;
	}
	if ( GetWantedAutoVScroll() )
	{
		style |= ES_AUTOVSCROLL;
	}
	if ( GetWantedMultiline() )
	{
		style |= ES_MULTILINE;
	}
	if ( GetWantedReadonly() )
	{
		style |= ES_READONLY;
	}
	return style;
}

std::wstring Richtext::GetType() const
{
	return MSFTEDIT_CLASS;
}

void Richtext::Create( HWND parent )
{
	m_parentHandle = parent;
	m_handle = CreateWindowExW(
		0,
		GetType().c_str(),
		GetWantedText().c_str(),
		GetWantedStyle(),
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

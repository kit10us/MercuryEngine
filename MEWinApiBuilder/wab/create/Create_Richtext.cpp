// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Richtext.h>
#include <wab/ui/Richtext.h>
#include <Richedit.h>

using namespace create;

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

Richtext* Richtext::SetAutoHScroll( bool autohscroll )
{
	if ( autohscroll && !GetHScroll() )
	{
		SetHScroll( true );
	}

	m_wantedAutoHScroll = autohscroll;
	return this;
}

bool Richtext::GetAutoHScroll() const
{
	return m_wantedAutoHScroll;
}

Richtext* Richtext::SetAutoVScroll( bool autovscroll )
{
	if ( autovscroll && !GetVScroll() )
	{
		SetVScroll( true );
	}

	m_wantedAutoVScroll = autovscroll;
	return this;
}

bool Richtext::GetAutoVScroll() const
{
	return m_wantedAutoVScroll;
}

Richtext* Richtext::SetMultiline( bool multiline )
{
	m_wantedMulitline = multiline;
	return this;
}

bool Richtext::GetMultiline() const
{
	return m_wantedMulitline;
}

Richtext* Richtext::SetReadonly( bool readonly )
{
	m_wantedReadonly = readonly;
	return this;
}

bool Richtext::GetReadonly() const
{
	return m_wantedReadonly;
}

DWORD Richtext::GetWantedStyle() const
{
	DWORD style = Control::GetWantedStyle();
	style |= WS_BORDER | WS_TABSTOP;
	if ( GetHScroll() )
	{
		style |= WS_HSCROLL;
	}
	if ( GetVScroll() )
	{
		style |= WS_VSCROLL;
	}
	if ( GetAutoHScroll() )
	{
		style |= ES_AUTOHSCROLL;
	}
	if ( GetAutoVScroll() )
	{
		style |= ES_AUTOVSCROLL;
	}
	if ( GetMultiline() )
	{
		style |= ES_MULTILINE;
	}
	if ( GetReadonly() )
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
	HWND handle = CreateWindowExW(
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

	m_control.reset( new ui::Richtext( parent, handle, this ) );
}

int Richtext::GetDefaultWidth() const
{

	return 320;
}

int Richtext::GetDefaultHeight() const
{
	return 160;
}

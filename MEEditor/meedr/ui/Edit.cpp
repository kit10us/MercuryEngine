// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Edit.h>

using namespace meedr;
using namespace ui;

Edit::Edit( FillWidth fillWidth, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ false }
	, m_wantedReadonly{ false }
{
	SetFillWidthWeight( fillWidth.weight );
}
Edit::Edit( FillWidth fillWidth, FillHeight fillHeight )
	: Control(FillWidth::Value(), FillHeight::Value() )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ false }
	, m_wantedReadonly{ false }
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
}
Edit::Edit( FillWidth fillWidth, int height )
	: Control(FillWidth::Value(), height )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ false }
	, m_wantedReadonly{ false }
{
	SetFillWidthWeight( fillWidth.weight );
}

Edit::Edit( int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ false }
	, m_wantedReadonly{ false }
{
}
Edit::Edit( int width, FillHeight fillHeight )
	: Control(width, FillHeight::Value() )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ false }
	, m_wantedReadonly{ false }
{
	SetFillHeightWeight( fillHeight.weight );
}
Edit::Edit( int width, int height )
	: Control(width, height )
	, m_wantedAutoHScroll{ false }
	, m_wantedAutoVScroll{ false }
	, m_wantedMulitline{ false }
	, m_wantedReadonly{ false }
{
}

Edit::~Edit()
{
}

Edit* Edit::SetWantedAutoHScroll( bool autohscroll )
{
	if ( autohscroll && !GetWantedHScroll() )
	{
		SetWantedHScroll( true );
	}

	m_wantedAutoHScroll = autohscroll;
	return this;
}

bool Edit::GetWantedAutoHScroll() const
{
	return m_wantedAutoHScroll;
}

Edit* Edit::SetWantedAutoVScroll( bool autovscroll )
{
	if ( autovscroll && !GetWantedVScroll() )
	{
		SetWantedVScroll( true );
	}

	m_wantedAutoVScroll = autovscroll;
	return this;
}

bool Edit::GetWantedAutoVScroll() const
{
	return m_wantedAutoVScroll;
}

Edit* Edit::SetWantedMultiline( bool multiline )
{
	m_wantedMulitline = multiline;
	return this;
}

bool Edit::GetWantedMultiline() const
{
	return m_wantedMulitline;
}

Edit* Edit::SetWantedReadonly( bool readonly )
{
	m_wantedReadonly = readonly;
	return this;
}

bool Edit::GetWantedReadonly() const
{
	return m_wantedReadonly;
}

DWORD Edit::GetWantedStyle() const
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

std::wstring Edit::GetType() const
{
	return L"Edit";
}

int Edit::GetDefaultWidth() const
{
	return 320;
}

int Edit::GetDefaultHeight() const
{
	return 22 + (GetWantedHScroll() ? 16 : 0);
}

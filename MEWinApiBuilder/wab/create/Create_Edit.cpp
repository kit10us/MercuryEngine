// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Edit.h>
#include <wab/ui/Edit.h>

using namespace create;

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

Edit* Edit::SetAutoHScroll( bool autohscroll )
{
	if ( autohscroll && !GetHScroll() )
	{
		SetHScroll( true );
	}

	m_wantedAutoHScroll = autohscroll;
	return this;
}

bool Edit::GetAutoHScroll() const
{
	return m_wantedAutoHScroll;
}

Edit* Edit::SetAutoVScroll( bool autovscroll )
{
	if ( autovscroll && !GetVScroll() )
	{
		SetVScroll( true );
	}

	m_wantedAutoVScroll = autovscroll;
	return this;
}

bool Edit::GetAutoVScroll() const
{
	return m_wantedAutoVScroll;
}

Edit* Edit::SetMultiline( bool multiline )
{
	m_wantedMulitline = multiline;
	return this;
}

bool Edit::GetMultiline() const
{
	return m_wantedMulitline;
}

Edit* Edit::SetReadonly( bool readonly )
{
	m_wantedReadonly = readonly;
	return this;
}

bool Edit::GetReadonly() const
{
	return m_wantedReadonly;
}

DWORD Edit::GetWantedStyle() const
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
	return 22 + (GetHScroll() ? 16 : 0);
}

void Edit::Create( HWND parent )
{
	HWND handle = CreateWindowW(
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

	m_control.reset( new ui::Edit( parent, handle, this ) );
}
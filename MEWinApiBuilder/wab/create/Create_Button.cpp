// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Button.h>
#include <wab/ui/Button.h>

using namespace create;

Button::Button( std::wstring text, DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value(), text )
{
}
Button::Button( std::wstring text, DefaultWidth, FillHeight heightWeight )
	: Control(DefaultWidth::Value(), FillHeight::Value(), text )
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height, text )
{  
}

Button::Button( std::wstring text, FillWidth widthWeight, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value(), text )
{
	SetFillWidthWeight( widthWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, FillHeight heightWeight )
	: Control(FillWidth::Value(), FillHeight::Value(), text )
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, int height )
	: Control(FillWidth::Value(), height, text )
{
	SetFillWidthWeight( widthWeight.weight );
}

Button::Button( std::wstring text, SizeToContentWidth, DefaultHeight )
	: Control(SizeToContentWidth::Value(), DefaultHeight::Value(), text )
{
}
Button::Button( std::wstring text, SizeToContentWidth, FillHeight heightWeight )
	: Control(SizeToContentWidth::Value(), FillHeight::Value(), text )
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, SizeToContentWidth, int height )
	: Control(SizeToContentWidth::Value(), height, text )
{
}

Button::Button( std::wstring text, int width, DefaultHeight )
	: Control(width, DefaultHeight::Value(), text )
{
}
Button::Button( std::wstring text, int width, FillHeight heightWeight )
	: Control(width, FillHeight::Value(), text )
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, int width, int height )
	: Control(width, height, text )
{
}

Button::~Button()
{
}

DWORD Button::GetWantedStyle() const
{
	return Control::GetWantedStyle() | WS_TABSTOP | BS_DEFPUSHBUTTON;
}

std::wstring Button::GetType() const
{
	return L"Button";
}

int Button::GetDefaultWidth() const
{
	return 120;
}

int Button::GetDefaultHeight() const
{
	return 26;
}	 

void Button::ComputePass1()
{
	Control::ComputePass1();

	if ( SizeToContentWidth() == GetWantedWidth() )
	{
		int charWidth = LOWORD( GetDialogBaseUnits() ) + 2;
		m_actualWidth = charWidth * GetWantedText().size();
	}
}

void Button::Create( HWND parent )
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

	m_control.reset( new ui::Button( parent, handle, this ) );
}
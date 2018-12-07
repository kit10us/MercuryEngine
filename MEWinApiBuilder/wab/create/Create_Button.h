// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>
#include <string>

namespace create
{
	class Button : public Control
	{
	public:		   			
		Button( std::wstring text, DefaultWidth, DefaultHeight );
		Button( std::wstring text, DefaultWidth, FillHeight heightWeight );
		Button( std::wstring text, DefaultWidth, int height );
			
		Button( std::wstring text, FillWidth widthWeight, DefaultHeight );
		Button( std::wstring text, FillWidth widthWeight, FillHeight heigthWidth );
		Button( std::wstring text, FillWidth widthWeight, int height );

		Button( std::wstring text, SizeToContentWidth, DefaultHeight );
		Button( std::wstring text, SizeToContentWidth, FillHeight heigthWidth );
		Button( std::wstring text, SizeToContentWidth, int height );


		Button( std::wstring text, int width, DefaultHeight );
		Button( std::wstring text, int width, FillHeight heightWeight );
		Button( std::wstring text, int width, int height );

		virtual ~Button();

		DWORD GetWantedStyle() const override;
		std::wstring GetType() const override;

		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

	protected:
		void ComputePass1() override;
		void Create( HWND parent ) override;
	};
}
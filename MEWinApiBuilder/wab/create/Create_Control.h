// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_IControl.h>

namespace ui
{
	class Window;
}

namespace create
{
	class Control : public IControl
	{
		friend class ui::Window;

	protected:
		Control( int width, int height, std::wstring wantedText = L"" );

	public:
		virtual ~Control();

		// Default values...
		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

		// Wanted values...
		void SetFillWidthWeight( int weight ) override;
		int GetFillWidthWeight() const override;

		void SetFillHeightWeight( int weight ) override;
		int GetFillHeightWeight() const override;

		int GetWantedWidth() const override;
		int GetWantedHeight() const override;
		std::wstring GetWantedText() const override;

		Control* SetHScroll( bool hscroll );
		bool GetHScroll() const;
		Control* SetVScroll( bool vscroll );
		bool GetVScroll() const;

		DWORD GetWantedStyle() const override;

		// Actual values...
		int GetActualX() const override;
		int GetActualY() const override;
		int GetActualWidth() const override;
		int GetActualHeight() const override;

		void SetID( int id ) override;
		int GetID() const override;

		void SetName( std::string name ) override;
		std::string GetName() const override;

		ui::IControl * GetControl() override;

	protected:
		void ComputePass1() override;
		void ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight ) override;
		void ComputePass3( int x, int y ) override;

		container::Container * m_parent;
		int m_id;
		std::string m_name;
		int m_fillWidthWeight;
		int m_fillHeightWeight;
		int m_wantedWidth;
		int m_wantedHeight;
		std::wstring m_wantedText;
		bool m_wantedHScroll;
		bool m_wantedVScroll;
		int m_actualX;
		int m_actualY;
		int m_actualWidth;
		int m_actualHeight;

		ui::IControl::ptr m_control;
	};
}

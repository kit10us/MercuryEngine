// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <Windows.h>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

#include <wab/ui/IControl.h>

#include <string>
#include <cassert>
#include <memory>

namespace ui
{
	class IWindow;
}

namespace create
{
	struct DefaultWidth {
		bool operator==(int value){ return Value() == value; }
		static int Value() { return -1; }
	};
	struct DefaultHeight {
		bool operator==(int value){ return Value() == value; }
		static int Value() { return -1; }
	};

	struct FillWidth {
		FillWidth( int weight = 1 ) : weight{ weight } {}
		int weight;
		bool operator==(int value){ return Value() == value; }
		static int Value() { return -2; }
	};
	struct FillHeight {
		FillHeight( int weight = 1 ) : weight{ weight } {}
		int weight;
		bool operator==(int value){ return Value() == value; }
		static int Value() { return -2; }
	};

	struct SizeToContentWidth {
		SizeToContentWidth() {}
		bool operator==(int value){ return Value() == value; }
		static int Value() { return -3; }
	};
	struct SizeToContentHeight {
		SizeToContentHeight() {}
		bool operator==(int value){ return Value() == value; }
		static int Value() { return -3; }
	};

	namespace container
	{
		class Container;
		class Canvas;
		class StackPanel;
	}

	class IControl
	{
		friend class ui::IWindow;
		friend class container::Canvas;
		friend class container::StackPanel;
	public:
		typedef std::shared_ptr< IControl > ptr;

		virtual ~IControl() {}

		// Default values...
		virtual int GetDefaultWidth() const = 0;
		virtual int GetDefaultHeight() const = 0;
		virtual void SetFillWidthWeight( int weight ) = 0;
		virtual int GetFillWidthWeight() const = 0;
		virtual void SetFillHeightWeight( int weight ) = 0;
		virtual int GetFillHeightWeight() const = 0;

		// Wanted values...
		virtual int GetWantedWidth() const = 0;
		virtual int GetWantedHeight() const = 0;
		virtual std::wstring GetWantedText() const = 0;
		virtual DWORD GetWantedStyle() const = 0;
		virtual std::wstring GetType() const = 0;

		// Actual values...
		virtual int GetActualX() const = 0;
		virtual int GetActualY() const = 0;
		virtual int GetActualWidth() const = 0;
		virtual int GetActualHeight() const = 0;

		virtual void SetName( std::string name ) = 0;
		virtual std::string GetName() const = 0;

		virtual void SetID( int id ) = 0;
		virtual int GetID() const = 0;

		virtual ui::IControl * GetControl() = 0;

	protected:
		/*
		virtual void SetParent( container::Container * parent ) = 0;
		virtual container::Container * GetParent() const = 0;
		*/

		/// <summary>
		/// Compute constant/fixed sizes.
		/// </summary>
		virtual void ComputePass1() = 0;

		/// <summary>
		/// Compute fill sizes.
		/// </summary>
		virtual void ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight ) = 0;

		/// <summary>
		/// Compute  positions.
		/// </summary>
		virtual void ComputePass3( int x, int y ) = 0;

		/// <summary>
		/// Create all controls.
		/// </summary>
		virtual void Create( HWND parent ) = 0;
	};
}
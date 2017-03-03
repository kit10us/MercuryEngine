// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <Windows.h>
#include <string>
#include <cassert>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace meedr
{
	namespace ui
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
			friend class Builder;
			friend class container::Canvas;
			friend class container::StackPanel;
		public:
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
			virtual int GetID() const = 0;
			virtual std::string GetName() const = 0;

			// Actual values...
			virtual int GetActualX() const = 0;
			virtual int GetActualY() const = 0;
			virtual int GetActualWidth() const = 0;
			virtual int GetActualHeight() const = 0;

		protected:
			virtual void SetParent( container::Container * parent ) = 0;
			virtual container::Container * GetParent() const = 0;

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
}
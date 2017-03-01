// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <Windows.h>
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
			virtual int GetWidth() = 0;
			virtual int GetHeight() = 0;
			virtual int GetID() = 0;

			virtual int GetDefaultWidth() = 0;
			virtual int GetDefaultHeight() = 0;
			virtual void SetFillWidthWeight( int weight ) = 0;
			virtual int GetFillWidthWeight() = 0;
			virtual void SetFillHeightWeight( int weight ) = 0;
			virtual int GetFillHeightWeight() = 0;

		protected:
			virtual void Create( int x, int y, int parentWidth, int parentHeight, HWND parent ) = 0;
			virtual void SetParent( container::Container * parent ) = 0;
			virtual container::Container * GetParent() = 0;
		};
	}
}
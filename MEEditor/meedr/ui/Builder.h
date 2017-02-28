// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/container/Container.h>
#include <meedr/ui/container/Canvas.h>
#include <meedr/ui/container/StackPanel.h>
#include <meedr/ui/Static.h>
#include <meedr/ui/Button.h>
#include <meedr/ui/Combobox.h>
#include <meedr/ui/Listbox.h>

#include <map>

namespace meedr
{
	namespace ui
	{
		class Builder
		{
		public:
			Builder();
			~Builder();

			void AddContainer( container::Container * container );
			void AddControl( IControl * control );
			void StepDown();

			HWND Create( HWND parent, HINSTANCE hInstance, std::wstring className, std::wstring title, int x, int y, void * lparam = nullptr );

			HWND GetHandle() const;

		private:
			HWND m_handle;
			container::Container * m_rootContainer;
			container::Container * m_currentParent;
		};
	}
}
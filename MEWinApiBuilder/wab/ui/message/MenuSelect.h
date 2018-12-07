// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/POD.h>

#include <wab/ui/MenuItem.h>

namespace ui
{
	namespace message
	{
		enum class MenuFlags
		{
			Insert = MF_INSERT,
			Change = MF_CHANGE,
			Append = MF_APPEND,
			Delete = MF_DELETE,
			Remove = MF_REMOVE,
			Command = MF_BYCOMMAND,
			ByPosition = MF_BYPOSITION,
			Separator = MF_SEPARATOR,
			Enabled = MF_ENABLED,
			Grayed = MF_GRAYED,
			Disabled = MF_DISABLED,
			Unchecked = MF_UNCHECKED,
			Checked = MF_CHECKED,
			UseCHeckBits = MF_USECHECKBITMAPS,
			String = MF_STRING,
			Bitmap = MF_BITMAP,
			OwnerDraw = MF_OWNERDRAW,
			Popup = MF_POPUP,
			MenuBarBreak = MF_MENUBARBREAK,
			MenuBreak = MF_MENUBREAK,
			Unhilite = MF_UNHILITE,
			Hilite = MF_HILITE,
			Default = MF_DEFAULT,
			SysMenu = MF_SYSMENU,
			Help = MF_HELP,
			RightJustify = MF_RIGHTJUSTIFY,
			MouseSelect = MF_MOUSESELECT
		};

		struct MenuSelect
		{
			ui::MenuItem * item;
			ui::Menu * menu;
			WORD flags;

			bool IsFor( std::string _name ) const
			{
				return unify::StringIs( item->GetName(), _name );
			}

			bool IsGrayed() const
			{
				return ((flags & (int)MenuFlags::Grayed) == (int)MenuFlags::Grayed);
			}

			bool IsDisabled() const
			{
				return ((flags & (int)MenuFlags::Disabled) == (int)MenuFlags::Disabled);
			}

			bool IsChecked() const
			{
				return ((flags & (int)MenuFlags::Checked) == (int)MenuFlags::Checked);
			}

			bool IsBitmap() const
			{
				return ((flags & (int)MenuFlags::Bitmap) == (int)MenuFlags::Bitmap);
			}

			bool IsPopup() const
			{
				return ((flags & (int)MenuFlags::Popup) == (int)MenuFlags::Popup);
			}

			bool IsHelp() const
			{
				return ((flags & (int)MenuFlags::Help) == (int)MenuFlags::Help);
			}

			bool IsSysMenu() const
			{
				return ((flags & (int)MenuFlags::SysMenu) == (int)MenuFlags::SysMenu);
			}

			bool IsMouseSelect() const
			{
				return ((flags & (int)MenuFlags::MouseSelect) == (int)MenuFlags::MouseSelect);
			}
		};
	}
}
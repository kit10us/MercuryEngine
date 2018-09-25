// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/canvas/IElement.h>

namespace me
{
	namespace canvas
	{
		/// <summary>
		/// Used to represent an item in an Canvas list.
		/// </summary>
		class ListItem
		{
		public:
			ListItem( IElement::ptr item, std::string tag = std::string() );

			/// <summary>
			/// Get the IElement for this list item.
			/// </summary>
			IElement::ptr GetItem();

			/// <summary>
			/// Get the IElement for this list item.
			/// </summary>
			IElement::const_ptr GetItem() const;

			/// <summary>
			/// Get tag for this list item.
			/// </summary>
			std::string GetTag() const;

		private:
			IElement::ptr m_item;
			std::string m_tag;
		};
	}
}
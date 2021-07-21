// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/canvas/ListItem.h>

namespace me::canvas
{
	/// <summary>
	/// Create a simple menu system.
	/// </summary>
	class VList : public Element
	{
		bool m_changed;
		unify::RowColumn< size_t > m_rc;
		unify::Size< float > m_size;
		size_t m_count;
		std::vector< ListItem > m_items;
		size_t m_selected;

	public:
		VList( game::IGame * gameInstance, size_t count, unify::Size< float > size, Anchor anchor = Anchor::Center, unify::V2< float > offset = { 0, 0 } );

		/// <summary>
		/// Add an item to the menu.
		/// </summary>
		void AddItem( IElement::ptr item, std::string name = {}, std::string tag = {} );

	public: // Element...
		unify::Size< float > GetContentSize() const override;
		void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea ) override;
		void Update( const UpdateParams & params ) override;
		void Render( const render::Params & params ) override;
		void OnSuspend() override;
		void OnResume() override;
		Element::ptr FindItem( std::string name );
	};
}
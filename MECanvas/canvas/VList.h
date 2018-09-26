// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <canvas/Element.h>
#include <canvas/ListItem.h>

namespace canvas
{
	/// <summary>
	/// Create a simple menu system.
	/// </summary>
	class VList : public Element
	{
	public:
		VList( me::game::IGame * gameInstance, size_t count, unify::Size< float > size, Anchor anchor = Anchor::Center, unify::V2< float > offset = { 0, 0 } );

		unify::Size< float > GetContentSize() const;

		void UpdateLayout( me::UpdateParams params, unify::Rect< float > parentArea );

		void Update( const me::UpdateParams & params );

		void Render( const me::render::Params & params );

		void OnSuspend();

		void OnResume();

		void AddItem( IElement::ptr item, std::string name = std::string(), std::string tag = std::string() );

		Element::ptr FindItem( std::string name );

	private:
		bool m_changed;
		unify::RowColumn< size_t > m_rc;
		unify::Size< float > m_size;
		size_t m_count;
		std::vector< ListItem > m_items;
		size_t m_selected;
	};
}

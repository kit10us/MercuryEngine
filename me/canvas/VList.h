// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/canvas/ListItem.h>

namespace me
{
	namespace canvas
	{
		/// <summary>
		/// Create a simple menu system.
		/// </summary>
		class VList : public Element
		{
		public:
			VList( game::IGame * gameInstance, size_t count, unify::Size< float > size, Anchor anchor = Anchor::Center, unify::V2< float > offset = { 0, 0 } );

			unify::Size< float > GetContentSize() const;

			void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea );

			void Update( const UpdateParams & params );

			void Render( const render::Params & params );

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
}
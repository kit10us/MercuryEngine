// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>

namespace me
{
	namespace canvas
	{
		// TODO: Note sure if I want this.
		class ListItem
		{
		public:
			ListItem( IElement::ptr );

			IElement::ptr GetItem();

		private:
			IElement::ptr m_item;
		};

		class SelectedCB
		{
		public:
			SelectedCB( IElement::ptr item, size_t index )  
				: m_item{ item }
				, m_index{ index }
			{
			}

			IElement::ptr GetItem() const
			{
				return m_item;
			}

			size_t GetIndex() const 
			{
				return m_index;
			}

		private:
			IElement::ptr m_item;
			size_t m_index;
		};

		/// <summary>
		/// Create a simple menu system.
		/// </summary>
		class VList : public Element
		{
		public:
			VList( IGame * game, size_t count, unify::Size< float > size, Anchor anchor = Anchor::Center, unify::V2< float > offset = { 0, 0 } );

			unify::Size< float > GetContentSize() const;

			void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea );

			void Update( const UpdateParams & params );

			void Render( render::Params params );

			void OnSuspend();

			void OnResume();

			void AddItem( IElement::ptr item, std::string name = std::string() );

			Element::ptr FindItem( std::string name ) const;

		private:
			bool m_changed;
			unify::RowColumn< size_t > m_rc;
			unify::Size< float > m_size;
			size_t m_count;
			std::vector< IElement::ptr > m_items;
			size_t m_selected;
		};
	}
}
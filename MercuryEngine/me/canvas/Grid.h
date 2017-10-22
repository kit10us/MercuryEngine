// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>

namespace me
{
	namespace canvas
	{
		struct SizeTo
		{
			enum Type {			
				Pixels,
				Percent,
				Weighted
			};

			SizeTo( Type type, float value )
				: type{ type }
				, value{ value }
			{
			}

			Type type;
			float value;

			float GetSize( float parentSize, size_t totalWeight ) const
			{
				switch( type )
				{
				case Pixels:
					return value;
				case Percent:
					return parentSize * value;
				case Weighted:
					return (float)totalWeight / value * parentSize;
				}

			}
			
		};

		/// <summary>
		/// Create a simple menu system.
		/// </summary>
		class Grid : public Element
		{
		public:
			Grid( IGame * game, unify::RowColumn< size_t > rc, unify::Size< float > size, Anchor anchor = Anchor::Center, unify::V2< float > offset = { 0, 0 } );

			unify::Size< float > GetContentSize() const;

			void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea );

			void Update( UpdateParams params );

			void Render( render::Params params );

			void OnSuspend();

			void OnResume();

			void AddItem( IElement::ptr item );

		private:
			bool m_changed;
			unify::RowColumn< size_t > m_rc;
			unify::Size< float > m_size;
			std::vector< size_t > m_weightRows;
			std::vector< size_t > m_weightColumns;
			std::vector< std::vector< IElement::ptr > > m_items;
		};
	}
}
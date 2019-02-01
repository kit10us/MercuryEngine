// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <unify/V2.h>
#include <unify/Size.h>

namespace me
{
	namespace render
	{
		class Viewport
		{
		public:
			Viewport();
			Viewport( unify::V2< float > ul, unify::Size< float > size, float minDepth, float maxDepth );
			Viewport( float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth );

			void SetUL( unify::V2< float > ul );
			void SetSize( unify::Size< float > size );
			void SetMinDepth( float minDepth );
			void SetMaxDepth( float maxDepth );

			unify::V2< float > GetUL() const;
			unify::Size< float > GetSize() const;
			float GetMinDepth() const;
			float GetMaxDepth() const;
		private:
			unify::V2< float > m_ul;
			unify::Size< float > m_size;
			float m_minDepth;
			float m_maxDepth;
		};
	}
}
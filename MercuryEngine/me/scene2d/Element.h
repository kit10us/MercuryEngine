// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene2d/Anchor.h>
#include <me/scene2d/IElement.h>
#include <unify/Size.h>

namespace me
{
	namespace scene2d
	{
		class Element : public IElement
		{
		public:
			Element();

		private:
			Anchor m_anchor;
			unify::V2< float > m_position;
			unify::Size< float > m_size;
		};
	}
}
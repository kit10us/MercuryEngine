// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>

namespace dxi
{
	namespace font
	{
		class IText
		{
		public:
			virtual void Update( unify::Seconds elapsed ) = 0;
			virtual void Render( const RenderInfo & renderInfo, bool is3D = false ) = 0;
			virtual ~IText() {}

		protected:
			IText() {}

		};
	} // namespace font
} // namespace dxi
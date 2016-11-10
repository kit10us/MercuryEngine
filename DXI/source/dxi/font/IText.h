// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>

namespace dxi
{
	namespace font
	{
		class IText
		{
		public:
			virtual void Update( const me::RenderInfo & renderInfo ) = 0;
			virtual void Render( const me::RenderInfo & renderInfo, bool is3D = false ) = 0;
			virtual ~IText() {}

		protected:
			IText() {}

		};
	} // namespace font
} // namespace dxi
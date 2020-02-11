// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/Display.h>
#include <memory>

namespace me
{
	namespace render
	{
		class IRendererFactory
		{
		public:
			typedef std::shared_ptr< IRendererFactory > ptr;

			virtual ~IRendererFactory() {}

			virtual class IRenderer * Produce( me::render::Display display, size_t index ) = 0;
		};
	}
}


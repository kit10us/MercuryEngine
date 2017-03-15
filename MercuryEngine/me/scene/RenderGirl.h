// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
#include <me/scene/IObjectAllocator.h>

namespace me
{
	namespace scene
	{
		class RenderGirl
		{
			const RenderParams * m_params;

			size_t m_renderCount;
		public:
			RenderGirl();
			~RenderGirl();

			void Begin( const RenderParams * params );
			void Render( scene::IObjectAllocator * allocator );
			size_t End();
		};
	}
}
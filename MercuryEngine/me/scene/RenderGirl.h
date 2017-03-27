// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/IObjectAllocator.h>

namespace me
{
	namespace scene
	{
		class RenderGirl
		{
			const RenderParams * m_params;					
			size_t m_renderCount;
			object::CameraCache m_cameraCache;

		public:
			RenderGirl();
			~RenderGirl();

			const RenderParams * GetParams() const;

			/// <summary>
			/// Begin rendering.
			/// </summary>
			void Begin( const RenderParams * params );

			void AddCamera( object::FinalCamera camera );

			/// <summary>
			/// Begin rendering.
			/// </summary>
			void Render( scene::IObjectAllocator * allocator );

			/// <summary>
			/// End rendering.
			/// </summary>
			size_t End();
		};
	}
}
// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/scene/IObjectAllocator.h>

namespace me
{
	namespace scene
	{
		class RenderGirl
		{
			const render::Params * m_params;					
			size_t m_renderCount;
			object::CameraCache m_cameraCache;

		public:
			RenderGirl();
			~RenderGirl();

			const render::Params * GetParams() const;

			/// <summary>
			/// Begin rendering.
			/// </summary>
			void Begin( const render::Params * params );

			void AddCamera( object::FinalCamera camera );

			/// <summary>
			/// Begin rendering.
			/// </summary>
			void Render( scene::IObjectAllocator * allocator );

			/// <summary>
			/// End rendering. Returns the number of objects rendered.
			/// </summary>
			size_t End();
		};
	}
}
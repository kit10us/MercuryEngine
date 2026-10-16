// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/RenderInfo.h>
#include <unify/TimeDelta.h>

namespace me::render
{
	class IRenderer;
}

namespace me
{
	namespace render
	{
		/// <summary>
		/// Information used during rendering.
		/// </summary>
		struct Params
		{
			/// <summary>
			/// The active renderer.
			/// </summary>
			IRenderer * renderer;

			/// <summary>
			/// Rendering setup information.
			/// </summary>
			RenderInfo renderInfo;

			/// <summary>
			/// Frame delta for animations.
			/// </summary>
			unify::TimeDelta GetDelta() const { return renderInfo.GetDelta(); };
		};
	}
}